#include "authentication_repository.hpp"

#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <openssl/evp.h>

namespace squawkbus::server
{
  template<class T> struct deleter;

  template<> struct deleter<EVP_MD_CTX> { void operator()(EVP_MD_CTX *p) const { EVP_MD_CTX_free(p); } };

  template<class T> using openssl_unique_ptr = std::unique_ptr<T, deleter<T>>;

  using namespace std::string_literals;

  static char* uint32_to_b64(char *p, std::uint32_t v, std::size_t n)
  {
    static char itoa64[] = "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    while (n--)
    {
      *p++ = itoa64[v & 0x3f];
      v >>= 6;
    }

    return p;
  }

  std::string digest_to_string(const std::vector<unsigned char>& md5)
  {
    char buf[22];
    char* p = uint32_to_b64(buf, (md5[ 0]<<16) | (md5[ 6]<<8) | md5[12], 4);
    p =       uint32_to_b64(p,   (md5[ 1]<<16) | (md5[ 7]<<8) | md5[13], 4);
    p =       uint32_to_b64(p,   (md5[ 2]<<16) | (md5[ 8]<<8) | md5[14], 4);
    p =       uint32_to_b64(p,   (md5[ 3]<<16) | (md5[ 9]<<8) | md5[15], 4);
    p =       uint32_to_b64(p,   (md5[ 4]<<16) | (md5[10]<<8) | md5[ 5], 4);
    p =       uint32_to_b64(p,    md5[11],                               2);

    return std::string(buf, buf + sizeof(buf));
  }

  static std::string apr1_hash_password(const std::string& password, const std::string& salt)
  {
    unsigned int md5_digest_len = EVP_MD_size(EVP_md5());

    auto base_ctx = openssl_unique_ptr<EVP_MD_CTX>(EVP_MD_CTX_new());
    EVP_DigestInit_ex(base_ctx.get(), EVP_md5(), nullptr);
    EVP_DigestUpdate(base_ctx.get(), password.data(), password.size());
    EVP_DigestUpdate(base_ctx.get(), "$apr1$", sizeof("$apr1$") - 1);
    EVP_DigestUpdate(base_ctx.get(), salt.data(), salt.size());

    auto seed_ctx = openssl_unique_ptr<EVP_MD_CTX>(EVP_MD_CTX_new());
    EVP_DigestInit_ex(seed_ctx.get(), EVP_md5(), nullptr);
    EVP_DigestUpdate(seed_ctx.get(), password.data(), password.size());
    EVP_DigestUpdate(seed_ctx.get(), salt.data(), salt.size());
    EVP_DigestUpdate(seed_ctx.get(), password.data(), password.size());

    auto seed = std::vector<unsigned char>(std::size_t(md5_digest_len));
    unsigned int len;
    EVP_DigestFinal_ex(seed_ctx.get(), seed.data(), &len);
    if (len != md5_digest_len)
      throw std::runtime_error("failed to generate seed digest");

    for (int n = password.size(); n > 0; n -= md5_digest_len)
    {
      unsigned int seed_len = n > md5_digest_len ? md5_digest_len : n;
      EVP_DigestUpdate(base_ctx.get(), seed.data(), seed_len);
    }

    auto md5 = std::vector<unsigned char>(std::size_t(md5_digest_len), '\0');

    for (auto i = password.size(); i != 0; i >>= 1)
    {
      if (i & 1)
      {
        EVP_DigestUpdate(base_ctx.get(), md5.data(), 1);
      }
      else
      {
        EVP_DigestUpdate(base_ctx.get(), password.data(), 1);
      }
    }

    EVP_DigestFinal_ex(base_ctx.get(), md5.data(), &len);
    if (len != md5_digest_len)
      throw std::runtime_error("failed to generate base digest");

    for (int i = 0; i < 1000; ++i)
    {
      auto ctx = openssl_unique_ptr<EVP_MD_CTX>(EVP_MD_CTX_new());
      EVP_DigestInit_ex(ctx.get(), EVP_md5(), nullptr);

      if (i & 1)
      {
        EVP_DigestUpdate(ctx.get(), password.data(), password.size());
      }
      else
      {
        EVP_DigestUpdate(ctx.get(), md5.data(), md5_digest_len);
      }

      if (i % 3)
      {
        EVP_DigestUpdate(ctx.get(), salt.data(), salt.size());
      }

      if (i % 7)
      {
        EVP_DigestUpdate(ctx.get(), password.data(), password.size());
      }

      if (i & 1)
      {
        EVP_DigestUpdate(ctx.get(), md5.data(), md5_digest_len);
      }
      else
      {
        EVP_DigestUpdate(ctx.get(), password.data(), password.size());
      }

      EVP_DigestFinal_ex(ctx.get(), md5.data(), &len);
      if (len != md5_digest_len)
        throw std::runtime_error("failed to generate seed digest");
    }

    return digest_to_string(md5);
  }

  std::string apr1_extract_salt(const std::string& data)
  {
    auto salt_start = sizeof("$apr1$") - 1;
    auto salt_end = data.find('$', salt_start);
    if (salt_end == std::string::npos)
      throw std::runtime_error("missing salt");
    auto salt = data.substr(salt_start, salt_end - salt_start);
    if (salt.size() > 8)
      throw std::runtime_error("salt too long");
    return salt;
  }

  std::string encrypt_apr1_data(const std::string& password, const std::string& data)
  {
    auto salt = apr1_extract_salt(data);
    auto hash = apr1_hash_password(password, salt);

    return std::format("${}${}${}", "apr1", salt, hash);
  }

  std::string encrypt_data(const std::string& password, const std::string& data)
  {
    if (data.starts_with("$apr1$"))
      return encrypt_apr1_data(password, data);

    throw std::runtime_error("unhandled cipher");
  }

  bool AuthenticationRepository::authenticate(
    const std::string& username,
    const std::string& password) const
  {
    auto i_entry = entries_.find(username);
    if (i_entry == entries_.end())
      return false;

    auto encrypted = encrypt_data(password, i_entry->second);
    return encrypted == i_entry->second;
  }
}

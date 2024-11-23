#ifndef SQUAWKBUS_SERIALIZATION_BASIC_TOKEN_HPP
#define SQUAWKBUS_SERIALIZATION_BASIC_TOKEN_HPP

#include "serialization/b64.hpp"

namespace squawkbus::serialization
{

  inline std::string encode_basic_token(const std::string& username, const std::string& password)
  {
    auto s = username + ":" + password;
    auto buf = std::vector<unsigned char>(s.data(), s.data()+s.size());
    return base64_encode(buf);
  }

  inline std::pair<std::string, std::string> decode_basic_token(const std::string& token)
  {
    auto buf = base64_decode(token);
    auto s = std::string(buf.data(), buf.data() + buf.size());
    auto i = s.find(':');
    if (i == std::string::npos)
      throw std::runtime_error("token has no ':' separator");
    auto username = s.substr(0, i);
    auto password = s.substr(i+1);
    return std::make_pair(username, password);
  }

}

#endif // SQUAWKBUS_SERIALIZATION_BASIC_TOKEN_HPP

#ifndef SQUAWKBUS_SERIALIZATION_B64_HPP
#define SQUAWKBUS_SERIALIZATION_B64_HPP

#include <string>
#include <vector>

namespace squawkbus::serialization
{
  static std::string base64_encode(const std::vector<unsigned char> &in) {

      std::string out;

      int a = 0, b = -6;
      for (auto c : in) {
          a = (a << 8) + c;
          b += 8;
          while (b >= 0) {
              out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(a>>b)&0x3F]);
              b -= 6;
          }
      }
      if (b>-6)
        out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((a<<8)>>(b+8))&0x3F]);
      while (out.size()%4)
        out.push_back('=');
      return out;
  }

  static std::vector<unsigned char> base64_decode(const std::string &in) {

      std::vector<unsigned char> out;

      std::vector<int> T(256,-1);
      for (int i=0; i<64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;

      int a = 0, b = -8;
      for (unsigned char c : in) {
          if (T[c] == -1) break;
          a = (a << 6) + T[c];
          b += 6;
          if (b >= 0) {
              out.push_back(char((a>>b)&0xFF));
              b -= 8;
          }
      }
      return out;
  }

}

#endif // SQUAWKBUS_SERIALIZATION_B64_HPP

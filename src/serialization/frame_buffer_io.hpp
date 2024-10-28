#ifndef SQUAWKBUS_SERIALIZATION_FRAME_BUFFER_IO_HPP
#define SQUAWKBUS_SERIALIZATION_FRAME_BUFFER_IO_HPP

#include <cstdint>

#include <set>
#include <stdexcept>
#include <string>
#include <vector>

#include "serialization/frame_buffer.hpp"

namespace squawkbus::serialization
{
  // char

  inline FrameBuffer& operator<<(FrameBuffer& fbuf, char c)
  {
    fbuf.write(std::vector<char> {c});
    return fbuf;
  }

  inline FrameBuffer& operator>>(FrameBuffer& fbuf, char& c)
  {
    auto buf = fbuf.read(1);
    c = buf[0];
    return fbuf;
  }

  // bool

  inline FrameBuffer& operator<<(FrameBuffer& fbuf, bool b)
  {
    char c = (b ? 1 : 0);
    return fbuf << c;
  }

  inline FrameBuffer& operator>>(FrameBuffer& fbuf, bool& b)
  {
    char c;
    fbuf >> c;
    b = (c == 1);
    return fbuf;
  }

  // uint32

  inline FrameBuffer& operator<<(FrameBuffer& fbuf, std::uint32_t i) noexcept
  {
    auto buf = std::vector<char>(4, '\0');
    network_long_to_buf(i, buf.data());
    fbuf.write(std::move(buf));
    return fbuf;
  }

  inline FrameBuffer& operator>>(FrameBuffer& fbuf, std::uint32_t& u) noexcept
  {
    auto buf = fbuf.read(4);
    u = buf_to_network_long(buf.data());
    return fbuf;
  }

  // size_t

  inline FrameBuffer& operator<<(FrameBuffer& fbuf, std::size_t l)
  {
    if (l > std::numeric_limits<uint32_t>::max())
    {
      throw std::runtime_error("size is bigger than max uint32");
    }
    return fbuf << static_cast<std::uint32_t>(l);
  }

  inline FrameBuffer& operator>>(FrameBuffer& fbuf, std::size_t& l) noexcept
  {
    std::uint32_t u;
    fbuf >> u;
    l = static_cast<std::size_t>(u);
    return fbuf;
  }

  // int32

  inline FrameBuffer& operator<<(FrameBuffer& fbuf, std::int32_t i)
  {
    return fbuf << static_cast<uint32_t>(i);
  }

  inline FrameBuffer& operator>>(FrameBuffer& fbuf, int32_t& i)
  {
    std::uint32_t u;
    fbuf >> u;
    i = static_cast<std::int32_t>(u);
    return fbuf;
  }

  // string

  inline FrameBuffer& operator<<(FrameBuffer& fbuf, const std::string& s)
  {
    fbuf << s.size();
    auto buf = std::vector<char>(s.begin(), s.end());
    fbuf.write(buf);
    return fbuf;
  }

  inline FrameBuffer& operator>>(FrameBuffer& fbuf, std::string& s)
  {
    std::size_t l;
    fbuf >> l;
    auto buf = fbuf.read(l);
    s.reserve(l);
    s.assign(buf.begin(), buf.end());
    return fbuf;
  }

  // vector

  template <typename T>
  inline FrameBuffer& operator << (FrameBuffer& frame, const std::vector<T>& v)
  {
      frame << v.size();
      for (auto c : v)
          frame << c;
      return frame;
  }

  template <typename T>
  inline FrameBuffer& operator >> (FrameBuffer& frame, std::vector<T>& v)
  {
      std::size_t l;
      frame >> l;
      v.reserve(l);
      for (std::size_t i = 0; i < l; ++i)
      {
          T c;
          frame >> c;
          v.push_back(c);        
      }
      return frame;
  }

  // set

  template <typename T>
  inline FrameBuffer& operator << (FrameBuffer& frame, const std::set<T>& v)
  {
      frame << v.size();
      for (auto c : v)
          frame << c;
      return frame;
  }

  template <typename T>
  inline FrameBuffer& operator >> (FrameBuffer& frame, std::set<T>& v)
  {
      std::size_t l;
      frame >> l;
      for (std::size_t i = 0; i < l; ++i)
      {
          T c;
          frame >> c;
          v.insert(c);        
      }
      return frame;
  }

}

#endif // SQUAWKBUS_SERIALIZATION_FRAME_BUFFER_IO_HPP

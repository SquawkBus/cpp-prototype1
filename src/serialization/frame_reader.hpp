#ifndef SQUAWKBUS_SERIALIZATION_FRAME_READER_HPP
#define SQUAWKBUS_SERIALIZATION_FRAME_READER_HPP

#include <cstddef>
#include <deque>
#include <optional>
#include <vector>
#include <utility>

#include "frame_buffer.hpp"
#include "frame_buffer_io.hpp"

namespace squawkbus::serialization
{

  template <typename T>
  inline
  FrameBuffer& operator >> (FrameBuffer& fbuf, std::optional<T>& o)
  {
    T t;
    fbuf >> t;
    o = t;
    return fbuf;
  }

  class FrameReader
  {
  private:
    std::optional<std::size_t> len_;
    FrameBuffer fbuf_;
    std::deque<std::vector<char>> frames_;

  public:

    bool write(std::vector<char> buf)
    {
      fbuf_.write(std::move(buf));

      if (!len_ && fbuf_.size() >= 4)
        fbuf_ >> len_;

      if (len_ && fbuf_.size() >= *len_)
      {
        frames_.push_back(fbuf_.read(*len_));
        len_ = std::nullopt;
      }

      return !frames_.empty();
    }

    bool has_frame() const noexcept { return !frames_.empty(); }

    FrameBuffer read() noexcept
    {
      auto fbuf = FrameBuffer {};
      fbuf.write(std::move(frames_.front()));
      frames_.pop_front();
      return fbuf;
    }
  };

}

#endif // SQUAWKBUS_SERIALIZATION_FRAME_READER_HPP

#ifndef SQUAWKBUS_SERIALIZATION_FRAME_BUFFER_HPP
#define SQUAWKBUS_SERIALIZATION_FRAME_BUFFER_HPP

#include <cstdint>

#include <algorithm>
#include <deque>
#include <numeric>
#include <vector>

#include "network_bytes.hpp"

namespace squawkbus::serialization
{

  class FrameBuffer
  {
  private:
    std::deque<std::vector<char>> data_;
    std::size_t offset_ {0};

  public:

    std::size_t size() const noexcept
    {
      return
        std::accumulate(
          data_.begin(),
          data_.end(),
          0,
          [](auto lhs, auto rhs) { return lhs + rhs.size(); }) -
        offset_;
    }

    void write(std::vector<char> buf)
    {
      data_.push_back(std::move(buf));
    }

    std::vector<char> read(std::size_t len)
    {
      auto buf = std::vector<char>(len);

      auto i = std::size_t {0};
      while (i < len)
      {
        auto available = data_.front().size() - offset_;
        if (available == 0)
        {
          data_.pop_front();
          offset_ = 0;
        }

        auto count = std::min(len - i, available);
        std::copy(
          data_.front().begin() + offset_,
          data_.front().begin() + offset_ + count,
          buf.begin() + i);
        i += count;
        offset_ += count;
      }

      return buf;
    }

    std::vector<char> make_frame()
    {
      auto frame = std::vector<char>(4 + size());
      network_long_to_buf(size(), frame.data());
      auto offset = frame.begin() + 4;
      for (const auto& buf : data_)
      {
        std::copy(buf.begin(), buf.end(), offset);
        offset += buf.size();
      }
      return frame;
    }
  };

}

#endif // SQUAWKBUS_SERIALIZATION_FRAME_BUFFER_HPP

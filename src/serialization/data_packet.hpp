#ifndef SQUAWKBUS_SERIALIZATION_DATA_PACKET_HPP
#define SQUAWKBUS_SERIALIZATION_DATA_PACKET_HPP

#include <cstddef>
#include <set>
#include <vector>

#include "utils/utils.hpp"

#include "frame_buffer.hpp"
#include "frame_buffer_io.hpp"

namespace squawkbus::serialization
{
  class DataPacket
  {
  private:
    std::set<std::int32_t> entitlements_;
    std::vector<char> data_;

  public:
    DataPacket() noexcept
    {
    }

    DataPacket(
      const std::set<std::int32_t>& entitlements,
      const std::vector<char>& data) noexcept
      : entitlements_(entitlements),
        data_(data)
    {
    }

    DataPacket(
      std::set<std::int32_t>&& entitlements,
      std::vector<char>&& data) noexcept
      : entitlements_(std::move(entitlements)),
        data_(std::move(data))
    {
    }

    const std::set<std::int32_t>& entitlements() const noexcept { return entitlements_; }
    const std::vector<char>& data() const noexcept { return data_; }

    bool is_authorized(const std::set<std::int32_t> &granted_entitlements) const noexcept
    {
      return std::includes(
        granted_entitlements.begin(), granted_entitlements.end(),
        entitlements_.begin(), entitlements_.end());
    }

    bool operator==(const DataPacket& other) const noexcept
    {
      return
        entitlements_ == other.entitlements_ &&
        data_ == other.data_;
    }

    FrameBuffer& serialize(FrameBuffer& frame) const
    {
      return frame << entitlements_ << data_;
    }

    FrameBuffer& deserialize(FrameBuffer& frame)
    {
      return frame >> entitlements_ >> data_;
    }
  };

  inline std::ostream& operator << (std::ostream& os, const DataPacket& d)
  {
    return os
      << "DataPacket"
      << "(entitlements=" << ::to_string(d.entitlements())
      << ",data=" << ::to_string(d.data())
      << ")" ;
  }

  inline FrameBuffer& operator<<(FrameBuffer& frame, const DataPacket& d)
  {
    return d.serialize(frame);
  }

  inline FrameBuffer& operator>>(FrameBuffer& frame, DataPacket& p)
  {
    return p.deserialize(frame);
  }
  
}

#endif // SQUAWKBUS_SERIALIZATION_DATA_PACKET_HPP

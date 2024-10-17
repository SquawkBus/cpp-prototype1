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
  struct DataPacket
  {
    std::set<std::int32_t> entitlements;
    std::vector<char> data;

    DataPacket()
    {
    }

    DataPacket(const std::set<std::int32_t>& entitlements, const std::vector<char>& data)
      : entitlements(entitlements),
        data(data)
    {
    }

    DataPacket(std::set<std::int32_t>&& entitlements, std::vector<char>&& data)
      : entitlements(std::move(entitlements)),
        data(std::move(data))
    {
    }

    bool is_authorized(const std::set<std::int32_t> &granted_entitlements) const
    {
      return std::includes(
        granted_entitlements.begin(), granted_entitlements.end(),
        entitlements.begin(), entitlements.end());
    }
  };

  inline std::ostream& operator << (std::ostream& os, const DataPacket& d)
  {
    return os
      << "DataPacket"
      << "(entitlements=" << ::to_string(d.entitlements)
      << ",data=" << ::to_string(d.data)
      << ")" ;
  }

  inline bool operator == (const DataPacket& a, const DataPacket& b)
  {
    return a.entitlements == b.entitlements && a.data == b.data;
  }

  inline FrameBuffer& operator<<(FrameBuffer& frame, const DataPacket& d)
  {
    return frame << d.entitlements << d.data;
  }

  inline FrameBuffer& operator>>(FrameBuffer& frame, DataPacket& p)
  {
    return frame >> p.entitlements >> p.data;
  }
  
}

#endif // SQUAWKBUS_SERIALIZATION_DATA_PACKET_HPP

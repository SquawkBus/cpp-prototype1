#ifndef SQUAWKBUS_SERIALIZATION_NETWORK_BYTES_HPP
#define SQUAWKBUS_SERIALIZATION_NETWORK_BYTES_HPP

#include <arpa/inet.h>
#include <span>

namespace squawkbus::serialization
{
    void network_long_to_buf(uint32_t num, char buf[4])
    {
        *((uint32_t*)buf) = htonl(num);
    }

    uint32_t buf_to_network_long(const char buf[4])
    {
        return ntohl(*((uint32_t*)buf));
    }
}

#endif // SQUAWKBUS_SERIALIZATION_NETWORK_BYTES_HPP

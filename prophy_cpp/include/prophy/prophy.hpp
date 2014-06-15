#ifndef _PROPHY_PROPHY_HPP
#define _PROPHY_PROPHY_HPP

#include <stdint.h>

namespace prophy
{

inline void swap(uint8_t&)
{ }

inline void swap(uint16_t& in)
{
    in = (in << 8) | (in >> 8);
}

inline void swap(uint32_t& in)
{
    in = ((in << 8) & 0xFF00FF00) | ((in >> 8) & 0x00FF00FF);
    in = (in << 16) | (in >> 16);
}

inline void swap(uint64_t& in)
{
    in = ((in << 8) & 0xFF00FF00FF00FF00ULL ) | ((in >> 8) & 0x00FF00FF00FF00FFULL );
    in = ((in << 16) & 0xFFFF0000FFFF0000ULL ) | ((in >> 16) & 0x0000FFFF0000FFFFULL );
    in = (in << 32) | (in >> 32);
}

inline void swap(int8_t&)
{ }

inline void swap(int16_t& in)
{
    swap(*static_cast<uint16_t*>(static_cast<void*>(&in)));
}

inline void swap(int32_t& in)
{
    swap(*static_cast<uint32_t*>(static_cast<void*>(&in)));
}

inline void swap(int64_t& in)
{
    swap(*static_cast<uint64_t*>(static_cast<void*>(&in)));
}

inline void swap(float& in)
{
    swap(*static_cast<uint32_t*>(static_cast<void*>(&in)));
}

inline void swap(double& in)
{
    swap(*static_cast<uint64_t*>(static_cast<void*>(&in)));
}

} // namespace prophy

#endif  /* _PROPHY_PROPHY_HPP */

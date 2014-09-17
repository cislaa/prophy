#ifndef _PROPHY_DETAIL_CODEC_TRAITS_HPP_
#define _PROPHY_DETAIL_CODEC_TRAITS_HPP_

#include <stdint.h>

namespace prophy
{
namespace detail
{

template <typename T>
struct codec_traits
{
    enum { is_composite = true };
    enum { size = T::encoded_byte_size };
};

template <>
struct codec_traits<int8_t>
{
    enum { is_composite = false };
    enum { size = 1 };
};

template <>
struct codec_traits<int16_t>
{
    enum { is_composite = false };
    enum { size = 2 };
};

template <>
struct codec_traits<int32_t>
{
    enum { is_composite = false };
    enum { size = 4 };
};

template <>
struct codec_traits<int64_t>
{
    enum { is_composite = false };
    enum { size = 8 };
};

template <>
struct codec_traits<uint8_t>
{
    enum { is_composite = false };
    enum { size = 1 };
};

template <>
struct codec_traits<uint16_t>
{
    enum { is_composite = false };
    enum { size = 2 };
};

template <>
struct codec_traits<uint32_t>
{
    enum { is_composite = false };
    enum { size = 4 };
};

template <>
struct codec_traits<uint64_t>
{
    enum { is_composite = false };
    enum { size = 8 };
};

template <>
struct codec_traits<float>
{
    enum { is_composite = false };
    enum { size = 4 };
};

template <>
struct codec_traits<double>
{
    enum { is_composite = false };
    enum { size = 8 };
};

} // namespace detail
} // namespace prophy

#endif /* _PROPHY_DETAIL_CODEC_TRAITS_HPP_ */

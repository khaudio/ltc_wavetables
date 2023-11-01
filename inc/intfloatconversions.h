/*****************************************************************************
Numeric conversions for converting floating-point (-1.0 to 1.0)
to/from fixed-point integer values.

MIT License

Copyright (c) 2023 K Hughes Production LLC

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*****************************************************************************/

#ifndef INTFLOATCONVERSIONS_H
#define INTFLOATCONVERSIONS_H

#include <cmath>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <vector>

namespace LTC
{

template <typename T>
constexpr T get_zero()
{
    if (std::is_unsigned<T>())
    {
        return static_cast<T>((std::numeric_limits<T>::max() / 2) + 1);
    }
    return 0;
}

template <typename T>
constexpr inline T clip_float(T value)
{
    if (value > 1.0) return 1.0;
    else if (value < -1.0) return -1.0;
    return value;
}

template <typename T>
constexpr inline void clip_float(T* value)
{
    *value = clip_float<T>(*value);
}

template <typename T>
constexpr inline T round_float(T value)
{
    int truncated(static_cast<int>(value));
    T remainder(value - static_cast<T>(truncated));
    int roundValue(
            (remainder > 0.5)
            || ((remainder == 0.5) && !(static_cast<int>(value + 1) % 2))
        );
    return static_cast<T>(truncated + ((value < 0) ? -roundValue : roundValue));
}

template <typename F, typename I>
constexpr I float_to_int(F value)
{
    if (value == 0.0) return get_zero<I>();
    else if (std::is_unsigned<I>())
    {
        if (value < 0)
        {
            return static_cast<I>(round_float<F>(
                    static_cast<F>(get_zero<I>())
                    + (value * static_cast<F>(get_zero<I>()))
                ));
        }
        return static_cast<I>(
                (value * static_cast<F>(get_zero<I>() - 1))
                + static_cast<F>(get_zero<I>())
            );
    }
    else if (value < 0)
    {
        return static_cast<I>(
                round_float<F>(-value * static_cast<F>(std::numeric_limits<I>::min()))
            );
    }
    return static_cast<I>(
            round_float<F>(value * static_cast<F>(std::numeric_limits<I>::max()))
        );
}

template <typename F, typename I>
constexpr void float_to_int(I* converted, F* value)
{
    *converted = float_to_int<F, I>(*value);
}

template <typename F, typename I>
constexpr void float_to_int(I* converted, F* value, int length)
{
    for (int i(0); i < length; ++i)
    {
        float_to_int<F, I>(converted + i, value + i);
    }
}

template <typename F, typename I>
constexpr std::vector<I> float_to_int(std::vector<F> values)
{
    const int length = static_cast<int>(values.size());
    std::vector<I> converted;
    converted.reserve(length);
    for (int i(0); i < length; ++i)
    {
        converted.emplace_back(float_to_int<F, I>(values.at(i)));
    }
    return converted;
}

template <typename F, typename I>
constexpr void float_to_int(std::vector<I>* converted, std::vector<F>* values)
{
    const int length = static_cast<int>(values->size());
    for (int i(0); i < length; ++i)
    {
        float_to_int<F, I>(&((*converted)[i]), &((*values)[i]));
    }
}

template <typename F, typename I>
constexpr void float_to_int(std::vector<I>* converted, std::vector<F>* values, int length)
{
    for (int i(0); i < length; ++i)
    {
        float_to_int<F, I>(&((*converted)[i]), &((*values)[i]));
    }
}

template <typename I, typename F>
constexpr F int_to_float(I value)
{
    if (value == get_zero<I>()) return 0.0;
    else if (std::is_unsigned<I>())
    {
        if (value < get_zero<I>())
        {
            return (
                    static_cast<F>(value)
                    / static_cast<F>(get_zero<I>() - 1)
                );
        }
        return (
                static_cast<F>(value - get_zero<I>())
                / static_cast<F>(get_zero<I>())
            );
    }
    else if (value < 0)
    {
        return static_cast<F>(value) * static_cast<F>(std::numeric_limits<I>::max());
    }
    return static_cast<F>(value) * static_cast<F>(std::numeric_limits<I>::min());
}

template <typename I, typename F>
constexpr void int_to_float(F* converted, I* value)
{
    *converted = int_to_float<I, F>(*value);
}

template <typename I, typename F>
constexpr void int_to_float(F* converted, I* value, int length)
{
    for (int i(0); i < length; ++i)
    {
        int_to_float<I, F>(converted + i, value + i);
    }
}

template <typename I, typename F>
constexpr std::vector<F> int_to_float(std::vector<I> values)
{
    const int length = static_cast<int>(values.size());
    std::vector<F> converted;
    converted.reserve(length);
    for (int i(0); i < length; ++i)
    {
        converted.emplace_back(int_to_float<I, F>(values.at(i)));
    }
    return converted;
}

template <typename I, typename F>
constexpr void int_to_float(std::vector<F>* converted, std::vector<I>* values)
{
    const int length = static_cast<int>(values->size());
    for (int i(0); i < length; ++i)
    {
        int_to_float<I, F>(&((*converted)[i]), &((*values)[i]));
    }
}

template <typename I, typename F>
constexpr void int_to_float(std::vector<F>* converted, std::vector<I>* values, int length)
{
    for (int i(0); i < length; ++i)
    {
        int_to_float<I, F>(&((*converted)[i]), &((*values)[i]));
    }
}

template float get_zero<float>();
template double get_zero<double>();

template uint8_t get_zero<uint8_t>();
template int16_t get_zero<int16_t>();
template int32_t get_zero<int32_t>();

template float clip_float<float>(float);
template double clip_float<double>(double);

template void clip_float<float>(float*);
template void clip_float<double>(double*);

template float round_float<float>(float);
template double round_float<double>(double);

};

#endif

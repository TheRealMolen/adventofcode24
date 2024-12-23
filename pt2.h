#pragma once

#include <ostream>

using std::ostream;


template<typename T>
struct Pt2
{
    using el_type = T;
    using type = Pt2<el_type>;

    el_type x, y;

    template<typename otherT>
    explicit Pt2(const Pt2<otherT>& other) : x(el_type(other.x)), y(el_type(other.y))
    { /**/ }

    constexpr Pt2() = default;
    constexpr Pt2(el_type x, el_type y) : x(x), y(y)
    { /**/ }

    type& operator+=(const type& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }
    type& operator-=(const type& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    type operator+(const type& other) const
    {
        return type{ x + other.x, y + other.y };
    }
    type operator-(const type& other) const
    {
        return type{ el_type(x - other.x), el_type(y - other.y) };
    }
    type operator*(el_type scalar) const
    {
        return type{ x * scalar, y * scalar };
    }
    type operator-() const
    {
        return { -x, -y };
    }

    auto operator<=>(const type& rhs) const = default;

    void rotRight(int steps)
    {
        int sgn = steps > 0 ? 1 : -1;
        steps = abs(steps);
        for (int s = 0; s < steps; ++s)
        {
            swap(x, y);
            x *= sgn;
            y *= sgn * -1;
        }
    }

    type perp() const
    {
        return { y, -x };
    }

    el_type dot(const type& other) const
    {
        return (x * other.x) + (y * other.y);
    }

    el_type manhattan() const
    {
        return el_type(abs(x)) + el_type(abs(y));
    }
};

using Pt2i = Pt2<int>;
using Pt2d = Pt2<double>;
using Pt2i64 = Pt2<i64>;
using Pt2i16 = Pt2<int16_t>;
using Pt2u8 = Pt2<u8>;
using Pt2i8 = Pt2<i8>;

namespace std {
template<>
struct hash<Pt2i>
{
    size_t operator()(const Pt2i& p) const
    {
        return hash<int>{}(p.x) ^ hash<int>{}(p.y);
    }
};
template<>
struct hash<Pt2u8>
{
    size_t operator()(const Pt2u8& p) const
    {
        return hash<u16>{}(p.x | (u16(p.y) << 8));
    }
};
} // std

template<typename T>
ostream& operator<<(ostream& os, const Pt2<T>& pt)
{
    os << "(" << i64(pt.x) << ", " << i64(pt.y) << ")";
    return os;
}

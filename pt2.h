#pragma once

#include <ostream>

using std::ostream;


template<typename T>
struct Pt2
{
    using el_type = T;
    using type = Pt2<el_type>;

    el_type x, y;

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
        return type{ x - other.x, y - other.y };
    }
    type operator*(el_type scalar) const
    {
        return type{ x * scalar, y * scalar };
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
};

using Pt2i = Pt2<int>;
using Pt2d = Pt2<double>;
using Pt2i16 = Pt2<int16_t>;
using Pt2u8 = Pt2<u8>;

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
    os << "(" << pt.x << ", " << pt.y << ")";
    return os;
}

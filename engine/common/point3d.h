#ifndef POINT3D_H
#define POINT3D_H

#include <QHash>

struct Point
{
    int x, y, z;

    Point()
    {
        x = 0;
        y = 0;
        z = 0;
    }

    Point(int x, int y, int z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

};

inline Point operator+(const Point& p1, const Point& p2)
{
    return Point(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);
}

inline Point operator-(const Point& p1, const Point& p2)
{
    return Point(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
}

inline Point operator*(const Point& p1, const float f)
{
    return Point(p1.x * f, p1.y * f, p1.z * f);
}

inline Point operator*(const Point& p1, const Point& p2)
{
    return Point(p1.x * p2.x, p1.y * p2.y, p1.z * p2.z);
}

inline bool operator==(const Point& p1, const Point& p2)
{
    return (p1.x == p2.x && p1.y == p2.y && p1.z == p2.z);
}

inline uint qHash(const Point& p, uint seed)
{
        uint code = 0;

        code |= (p.x << 16);
        code |= (p.y & 0x0000ffff);
        code ^= (p.z << 16);
        code ^= (seed & 0x0000ffff);

        return code;
}

class Point3D
{
public:
    Point3D();
};

#endif // POINT3D_H

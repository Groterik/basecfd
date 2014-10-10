#ifndef BASECFD_VECTOR_GUARD_H
#define BASECFD_VECTOR_GUARD_H

#include <cmath>

namespace basecfd
{

class Vector
{
public:
    enum {XDIM = 0, YDIM, ZDIM, MAXDIM};
    explicit Vector(double X = 0, double Y = 0, double Z = 0)
    {
        p[0] = X;
        p[1] = Y;
        p[2] = Z;
    }
    inline double& operator[](dim_t dim)
    {
        return p[dim];
    }
    inline const double& operator[](dim_t dim) const
    {
        return p[dim];
    }
    inline double& x()
    {
        return p[XDIM];
    }
    inline const double& x() const
    {
        return p[XDIM];
    }
    inline double& y()
    {
        return p[YDIM];
    }
    const double& y() const
    {
        return p[YDIM];
    }
    double& z()
    {
        return p[ZDIM];
    }
    const double& z() const
    {
        return p[ZDIM];
    }
private:
    double p[MAXDIM];
};

inline bool equal(double a, double b, double maxRelDiff = 1e-2, double maxAbsDiff = 1e-5)
{
    return b == 0 ? fabs(a) <= maxAbsDiff : (fabs((a - b) / b) <= maxRelDiff) || (maxAbsDiff != 0 && fabs(a - b) <= maxAbsDiff);
}

inline bool equal(const Vector& a, const Vector& b, double maxRelDiff = 1e-2, double maxAbsDiff = 1e-5)
{
    return equal(a.x(), b.x(), maxRelDiff, maxAbsDiff)
           && equal(a.y(), b.y(), maxRelDiff, maxAbsDiff)
           && equal(a.z(), b.z(), maxRelDiff, maxAbsDiff);
}

inline Vector crossProduct(const Vector& a, const Vector& b)
{
    return Vector(a.y() * b.z() - a.z() * b.y(), a.z() * b.x() - a.x() * b.z(), a.x() * b.y() - a.y() * b.x());
}

inline double dotProduct(const Vector& a, const Vector& b)
{
    return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
}

inline double tripleProduct(const Vector& a, const Vector& b, const Vector& c)
{
    return dotProduct(crossProduct(a, b), c);
}

inline Vector operator-(const Vector& a, const Vector& b)
{
    return Vector(a.x() - b.x(), a.y() - b.y(), a.z() - b.z());
}

inline Vector operator+(const Vector& a, const Vector& b)
{
    return Vector(a.x() + b.x(), a.y() + b.y(), a.z() + b.z());
}

inline Vector operator/(const Vector& a, double d)
{
    return Vector(a.x() / d, a.y() / d, a.z() / d);
}

inline Vector operator*(const Vector& a, double m)
{
    return Vector(a.x() * m, a.y() * m, a.z() * m);
}

inline Vector& operator+=(Vector& lhs, const Vector& rhs)
{
    for (unsigned d = 0; d < Vector::MAXDIM; ++d)
    {
        lhs[d] += rhs[d];
    }
    return lhs;
}

inline Vector& operator-=(Vector& lhs, const Vector& rhs)
{
    for (unsigned d = 0; d < Vector::MAXDIM; ++d)
    {
        lhs[d] -= rhs[d];
    }
    return lhs;
}

inline Vector& operator/=(Vector& lhs, double rhs)
{
    for (unsigned d = 0; d < Vector::MAXDIM; ++d)
    {
        lhs[d] /= rhs;
    }
    return lhs;
}

inline void trim(Vector& v, dim_t dim)
{
    for (dim_t d = dim; d < Vector::MAXDIM; ++d)
    {
        v[d] = 0;
    }
}

inline double length(const Vector& v)
{
    return sqrt(dotProduct(v, v));
}

inline bool isFrom(const Vector& v, const Vector& a, const Vector& b)
{
    const double epsilon = 1e-6;
    const Vector fa(a.x() - fabs(a.x() * epsilon), a.y() - fabs(a.y() * epsilon), a.z() - fabs(a.z() * epsilon));
    const Vector fb(b.x() + fabs(b.x() * epsilon), b.y() + fabs(b.y() * epsilon), b.z() + fabs(b.z() * epsilon));
    return v.x() >= fa.x() && v.y() >= fa.y() && v.z() >= fa.z()
           && v.x() <= fb.x() && v.y() <= fb.y() && v.z() <= fb.z();
}

} // namespace basecfd

#endif //BASECFD_VECTOR_GUARD_H

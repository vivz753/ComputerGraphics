#include <cmath>
#include "Globals.h"
#include "Vector3.h"

#ifdef NDEBUG
#define Assert(expr) ((void)0)
#else
#define Assert(expr) { if(!(expr)) { std::cerr << "Assertion " << #expr << " failed in " << __FILE__ << " line " << __LINE__ << std::endl; } }
#endif // NDEBUG

template <typename T> class Point3 {
public:
	T x, y, z;

	Point3() { x = y = z = 0; }
	Point3(T x, T y, T z) : x(x), y(y), z(z) {
		Assert(!HasNaNs());
	}

	bool HasNaNs() const {
		return std::isnan(x) || std::isnan(y) || std::isnan(z);
	}

	// allow cast from integer to float, and vice versa, and convert a point to a vector
	// these require explicit casts, to make it clear when they are being used
	template <typename U> explicit Point3(const Point3<U>& p)
		: x((T)p.x), y((T)p.y), z((T)p.z) {
		Assert(!HasNaNs());
	}
	template <typename U> explicit operator Vector3<U>() const
	{
		return Vector3<U>(x, y, z);
	}
	
	// Vector addition to a point, offsetting it in the given direction to obtain a new point
	Point3<T> operator+(const Vector3<T>& v) const {
		return Point3<T>(x + v.x, y + v.y, z + v.z);
	}
	Point3<T>& operator+=(const Vector3<T>& v) {
		x += v.x; y += v.y; z += v.z;
		return *this;
	}

	// Point subtraction from another point, obtaining the vector btw them
	Vector3<T> operator-(const Point3<T>& p) const {
		return Vector3<T>(x - p.x, y - p.y, z - p.z);
	}

	// Vector subtraction of a point, obtaining another point
	Point3<T> operator-(const Vector3<T>& v) const {
		return Point3<T>(x - v.x, y - v.y, z - v.z);
	}
	Point3<T> operator-=(const Vector3<T>& v) {
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}

	// Assigning weights to points by a scalar or adding 2 points together doesn't mathematically make sense, but we can still multiply by scalar and add points to eachother, which is mathematically meaningful as long as the weights used all sum to one. These are identical to the corresponding functions for Vector3
	//Addition
	Point3<T> operator+(const Point3<T>& p) const {
		return Point3(x + p.x, y + p.y, z + p.z);
	}
	Point3<T>& operator+=(const Point3<T>& p) const {
		x += p.x; y += p.y; z += p.z;
		return *this;
	}
	// Subtraction
	Point3<T> operator-(const Point3<T>& p) const {
		return Point3(x - p.x, y - p.y, z - p.z);
	}
	Point3<T>& operator-=(const Point3<T>& p) const {
		x -= p.x; y -= p.y; z -= p.z;
		return *this;
	}
	// Multiplication by scalar
	Point3<T> operator*(T s) const {
		return Point3<T>(s * x, s * y, s * z);
	}
	Point3<T>& operator*=(T s) {
		x *= s; y *= s; z *= s;
		return *this;
	}


};

// Geometry Inline Functions
template <typename T> inline Float
Distance(const Point3<T>& p1, const Point3<T>& p2) {
	return (p1 - p2).Length();
}
template <typename T> inline Float
DistanceSquared(const Point3<T> &p1, const Point3<T> &p2) {
	return (p1 - p2).LengthSquared();
}

// Linear interpolation btw 2 points
// returns p0 at t = 0, p1 at t = 1
template <typename T> Point3<T>
Lerp(Float t, const Point3<T>& p0, const Point3<T>& p1) {
		return (1 - t) * p0 + t * p1;
}

// Min() and Max() return points representing the component-wise minimums and maximums of the 2 given points
template <typename T> Point3<T>
Min(const Point3<T> &p1, const Point3<T> &p2) {
    return Point3<T>(std::min(p1.x, p2.x), std::min(p1.y, p2.y), 
                     std::min(p1.z, p2.z));
}
template <typename T> Point3<T>
Max(const Point3<T> &p1, const Point3<T> &p2) {
    return Point3<T>(std::max(p1.x, p2.x), std::max(p1.y, p2.y), 
                     std::max(p1.z, p2.z));
}

template <typename T> Point3<T> Floor(const Point3<T>& p) {
	return Point3<T>(std::floor(p.x), std::floor(p.y), std::floor(p.z));
}
template <typename T> Point3<T> Ceil(const Point3<T>& p) {
	return Point3<T>(std::ceil(p.x), std::ceil(p.y), std::ceil(p.z));
}
template <typename T> Point3<T> Abs(const Point3<T>& p) {
	return Point3<T>(std::abs(p.x), std::abs(p.y), std::abs(p.z));
}

template <typename T> Point3<T>
Permute(const Point3<T>& p, int x, int y, int z) {
		return Point3<T>(p[x], p[y], p[z]);
}
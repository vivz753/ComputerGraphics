#pragma once
#include <iostream>
#include <cmath>
#include "Globals.h"

template <typename T> 
class Vector3 {
public: 

	// no need to hide
	T x, y, z;

	void print() {
		std::cout << x << " " << y << " " << z << std::endl;
	}

	// Index Accessors
	T operator[](int i) const {
		//Assert(i >= 0 && i <= 2);
		if (i == 0) return x;
		if (i == 1) return y;
		return z;
	}

	T &operator[](int i) {
		//Assert(i >= 0 && i <= 2);
		if (i == 0) return x;
		if (i == 1) return y;
		return z;
	}

Vector3() { x = y = z = 0; }
Vector3(T x, T y, T z)
	: x(x), y(y), z(z) {
	// check for NaN values; NaNs almost certainly indicate a bug in the system
	// checking with the Assert() macro, which will disappear from the code after compiled in optimized mode--this saves the expense of verifying this case
	//Assert(!HasNaNs());
}

bool HasNaNs() const {
	return std::isnan(x) || std::isnan(y) || std::isnan(z);
}

// Addition
Vector3<T> operator+(const Vector3<T>& v) const {
	return Vector3(x + v.x, y + v.y, z + v.z);
}
Vector3<T>& operator+=(const Vector3<T>& v) const {
	x += v.x; y += v.y; z += v.z;
	return *this;
}

// Subtraction
Vector3<T> operator-(const Vector3<T>& v) const {
	return Vector3(x - v.x, y - v.y, z - v.z);
}
Vector3<T>& operator-=(const Vector3<T>& v) const {
	x -= v.x; y -= v.y; z -= v.z;
	return *this;
}

// Multiplication by scalar
Vector3<T> operator*(T s) const {
	return Vector3<T>(s * x, s * y, s * z);
}
Vector3<T>& operator*=(T s) {
	x *= s; y *= s; z *= s;
	return *this;
}


// Division
// Beware of divisions by 0
// Calculate the reciprocal, then multiply by it--multiplying is faster than dividing for CPU's
Vector3<T> operator/(T f) const {
	Assert(f != 0);
	Float inv = (Float)1 / f;
	return Vector3<T>(x * inv, y * inv, z * inv);
}
Vector3<T>& operator/=(T f) {
	Assert(f != 0);
	Float inv = (Float)1 / f;
	x *= inv; y *= inv; z *= inv;
	return *this;
}

// Unary
// Allows us to create negative vectors
Vector3<T> operator-() const { return Vector3<T>(-x, -y, -z); }

// Dot Product (see below inline function)

// Cross Product (see below inline function)

Float LengthSquared() const { return x * x + y * y + z * z;  }
// Magnitude
Float Length() const { return std::sqrt(LengthSquared()); }



};

// Why Geometry Inline functions (?)

// Normalize
template <typename T> inline Vector3<T>
Normalize(const Vector3<T>& v) { return v / v.Length(); }

// Returns the smallest coord value
template <typename T> T
MinComponent(const Vector3<T>& v) {
	return std::min(v.x, std::min(v.y, v.z));
}

// Returns the largest coord value
template <typename T> T
MaxComponent(const Vector3<T>& v) {
	return std::max(v.x, std::max(v.y, v.z));
}

// Returns the index of the component w/ the largest value
template <typename T> int
MaxDimension(const Vector3<T>& v) {
	return (v.x > v.y) ? ((v.x > v.z) ? 0 : 2) :
		((v.y > v.z) ? 1 : 2);
}

// Returns min/max components of 2 vectors (?) Not sure why these are needed
template <typename T> Vector3<T>
Min(const Vector3<T>& p1, const Vector3<T>& p2) {
	return Vector3<T>(std::min(p1.x, p2.x), std::min(p1.y, p2.y),
		std::min(p1.z, p2.z));
}
template <typename T> Vector3<T>
Max(const Vector3<T>& p1, const Vector3<T>& p2) {
	return Vector3<T>(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z));
}

// Permutes the coordinate values according to the index values provided (?) I don't get what this is for
template <typename T> Vector3<T>
Permute(const Vector3<T>& v, int x, int y, int z) {
	return Vector3<T>(v[x], v[y], v[z]);
}

// Multiplication
template <typename T> inline Vector3<T> operator*(T s, const Vector3<T>& v) { return v * s; }

// Absolute Value
template <typename T> Vector3<T> Abs(const Vector3<T>& v) {
	return Vector3<T>(std::abs(v.x), std::abs(v.y), std::abs(v.z));
}

// Dot Product produces scalar value
// If 0, then v1 and v2 are perpandicular, assuming neither are degenerate, (0, 0, 0)
// It is also orthogonal - a set of 2 or more mutually perpendicular vectors
// A set of orthogonal unit vectors is called an orthonormal
// v1 dot v2 = ||v1|| ||v2|| cos (theta), where theta is the angle btw v1 and v2
// If v1 and v2 are normalized, then more simply, v1 dot v2 = cos (theta)
template <typename T> inline T
Dot(const Vector3<T>& v1, const Vector3<T>& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// Absolute value of the dot product
// This will be needed frequently
template <typename T>
inline T AbsDot(const Vector3<T>& v1, const Vector3<T>& v2) {
	// Why not just use the Abs() func declared above (?)
	return std::abs(Dot(v1, v2));
}

// Cross Product produces vector that is orthogonal/perpandicular to 2 vectors v and w
// || v cross w || = ||v|| ||w|| |sin (theta)| where theta is the angle btw v and w
// The cross product of 2 parallel vectors results in a degenerate vector (0, 0, 0)
// The cross product also computes the area of a parallelogram made with v and w,
// simply because height of parallelogram is equal to sin(theta)||v|| or sin(theta)||w||,
// and since base * height = A_parallelogram, then multiplying the height by ||w|| or ||v|| respectively is the same equation as above.
template <typename T> inline Vector3<T>
Cross(const Vector3<T>&v1, const Vector3<T> &v2) {
	// Convert to doubles for extra precision to prevent catastrophic cancellation when subtracting 2 close values due to rounding errors
	double v1x = v1.x, v1y = v1.y, v1z = v1.z;
	double v2x = v2.x, v2y = v2.y, v2z = v2.z;
	return Vector3<T>(
		(v1y * v2z) - (v1z * v2y),
		(v1z * v2x) - (v1x * v2z),
		(v1x * v2y) - (v1y * v2x));
}

// Create a Coordinate System given a single 3D vector
// This func assumes v1 has already been normalized
// It first constructs a perpendicular vector by zeroing one of the components of the original vector, swapping the remaining two, and negating one of them
// The dot product of v1 and v2 will result in zero, bc they are perpendicular
// The cross product gives the last perpendicular vector v3
template <typename T> inline void
CoordinateSystem(const Vector3<T>& v1, Vector3<T>* v2, Vector3<T>* v3) {
	if (std::abs(v1.x) > std::abs(v1.y))
		*v2 = Vector3<T>(-v1.z, 0, v1.x) /
		std::sqrt(v1.x * v1.x + v1.z * v1.z);
	else
		*v2 = Vector3<T>(0, v1.z, -v1.y) /
		std::sqrt(v1.y * v1.y + v1.z * v1.z);
	*v3 = Cross(v1, *v2);
}
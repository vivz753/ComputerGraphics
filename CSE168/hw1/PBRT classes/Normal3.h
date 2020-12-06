#pragma once
#include "Globals.h"
#include "Vector3.h"

class Vector3;

template <typename T> 
class Normal3 {
public:
	// Normal3 and Vector3 are very similar, they are both represented by x, y, z, can be added/subtracted to compute new normals, can be scaled and normalized. However, a normal can't be added to a point and a cross product cannot be taken of 2 normals. "Note that, in an unfortunate turn of terminology, normals are NOT necessarily normalized"
	// An extra constructor which initializes a Normal3 fro ma Vector3. Make it explicit, bc the 2 classes are diff in subtle ways, so we want to make sure this conversion doesn't happen when not intended
	explicit Normal3<T>(const Vector3<T>& v) : x(v.x), y(v.y), z(v.z) {
		Assert(!v.HasNaNs());
	}

	// Verify with Normal3.h on https://github.com/mmp/pbrt-v3/blob/master/src/core/geometry.h

	// Dot() and AbsDot() are overloaded to compute dot products between the various possible combos of normals & vectors

	// Implement methods from Vector3 that are relevant
};

// Geometry Inline Functions

// It's often necessary to flip a surface noraml so that it lies in the same hemisphere as a given vector--the surface normal that lies in the same hemisphere as an outgoing ray is frequently needed
template<typename T> inline Normal3<T>
Faceforward(const Normal3<T>& n, const Vector3<T>& v) {
	return (Dot(n, v) < 0.f) ? -n : n;
}


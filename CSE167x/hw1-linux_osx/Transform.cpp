// Transform.cpp: implementation of the Transform class.


#include "Transform.h"
#include <iostream>


using namespace glm;
//Please implement the following functions:

// Helper rotation function.  
mat3 Transform::rotate(const float degrees, const vec3& axis) {
	// YOUR CODE FOR HW1 HERE
	
	// print inputs
	std::cout << "Transform::rotate()" << std::endl;
	std::cout << "degrees: " << degrees << std::endl;
	std::cout << "axis: [" << axis.x << ", " << axis.y << ", " << axis.z << "]" << std::endl;


	// translate degrees to radians
	float const r = radians(degrees);
	std::cout << "radians: " << r << std::endl;

	// create matrices for rotations about the y or x axis
	// IMPORTANT: OpenGL matrices are ordered column-major!!
	
	mat3 rotationMatrix = mat3();
	float x = axis.x;
	float y = axis.y;
	float z = axis.z;


	rotationMatrix = cos(r)*mat3(1, 0, 0, 0, 1, 0, 0, 0, 1)
			+(1-cos(r))*mat3(x*x, x*y, x*z, x*y, y*y, y*z, x*z, y*z, z*z)
			+sin(r)*mat3(0, z, -y, -z, 0, x, y, -x, 0);

	return rotationMatrix;
}

// Transforms the camera left around the "crystal ball" interface
void Transform::left(float degrees, vec3& eye, vec3& up) {
	// YOUR CODE FOR HW1 HERE

	// translate the up vector to the origin (-eye)
	vec3 w = normalize(eye);
	vec3 u = normalize(cross(up, w));
	vec3 v = cross(w, u);
	//vec3 axis = mat

	mat3 rMatrix = rotate(degrees, v);
	
	// calculate the dist from eye to origin (should ALWAYS be the same)
	printf("ROTATING ON up (v): %.2f, %.2f, %.2f;", v.x, v.y, v.z);
	printf("horizontal (u): %.2f, %.2f, %.2f;", u.x, u.y, u.z);

	// update the eye vector after rotating it
	eye = rMatrix * eye;

	//printf("Coordinates: %.2f, %.2f, %.2f; distance: %.2f\n", eye.x, eye.y, eye.z, sqrt(pow(eye.x, 2) + pow(eye.y, 2) + pow(eye.z, 2)));


	// update the up vector
	// get u, which is orthogonal to up (v) and eye (w)
	w = normalize(eye);
	u = normalize(cross(up, w));
	v = cross(w, u);
	// IMPORTANT: up will not work if it does not result in a unit vector
	up = v;

}

// Transforms the camera up around the "crystal ball" interface
void Transform::up(float degrees, vec3& eye, vec3& up) {
	// YOUR CODE FOR HW1 HERE 

	vec3 w = normalize(eye);
	vec3 u = normalize(cross(up, w));
	vec3 v = cross(w, u);
	
	printf("up (v): %.2f, %.2f, %.2f;", v.x, v.y, v.z);
	printf("ROTATING ON horizontal (u): %.2f, %.2f, %.2f;", u.x, u.y, u.z);	

	mat3 rMatrix = rotate(-degrees, u);

	eye = rMatrix * eye;

	up = cross(normalize(eye), normalize(cross(up, normalize(eye))));
}

// Your implementation of the glm::lookAt matrix
mat4 Transform::lookAt(vec3 eye, vec3 up) {
	// YOUR CODE FOR HW1 HERE

	vec3 w = normalize(eye);
	vec3 u = normalize(cross(up, w));
	vec3 v = cross(w, u);

	mat4 translate = mat4(	1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				-eye.x, -eye.y, -eye.z, 1 );

	mat4 rotate = mat4(	u.x, v.x, w.x, 0,
				u.y, v.y, w.y, 0,
				u.z, v.z, w.z, 0,
				0, 0, 0, 1 	);

	// IMPORTANT: always translate before rotate for look at matrix
	// matrix multiplication goes from left -> right
	mat4 final = rotate * translate;


	return final;
}

Transform::Transform()
{

}

Transform::~Transform()
{

}

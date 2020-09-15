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

	mat3 rMatrix = rotate(degrees, up);
	
	// update the eye vector after rotating it
	eye = rMatrix * eye;
}

// Transforms the camera up around the "crystal ball" interface
void Transform::up(float degrees, vec3& eye, vec3& up) {
	// YOUR CODE FOR HW1 HERE 

	// u, v, w is essentially the camera's x, y, z, respectively.
	// Definitions: eye (w), up (v), horizontal/x-axis (u)

	// we need w to derive u
	vec3 w = normalize(eye);

	// we need u to rotate about u/the camera's x-axis
	vec3 u = normalize(cross(up, w));

	// we do not need to know v/up, since we are not rotating around the camera's y-axis
	//vec3 v = cross(w, u);
	
	//printf("up (v): %.2f, %.2f, %.2f;", v.x, v.y, v.z);
	//printf("ROTATING ON horizontal (u): %.2f, %.2f, %.2f;", u.x, u.y, u.z);	

	mat3 rMatrix = rotate(-degrees, u);

	eye = rMatrix * eye;

	// IMPORTANT: up will not work if it is not in the format of a unit vector

	// up needs to be updated since it changes when we rotate about u/horizontal-axis; u always gets calculated on the fly, so we don't need to update or store the value anywhere
	// up doesn't need to be updated in left() because it never changes when rotating about v/up
	up = cross(normalize(eye), u);
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
	// matrix multiplication goes from right -> left
	mat4 final = rotate * translate;


	return final;
}

Transform::Transform()
{

}

Transform::~Transform()
{

}

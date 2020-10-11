// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"

using namespace glm;

// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis) 
{
  mat3 ret;

	//translate degrees to radians
	float const r = radians(degrees);

	const vec3 normAxis = normalize(axis);

	float x = normAxis.x;
	float y = normAxis.y;
	float z = normAxis.z;

	// Angle-Axis rotation matrix
	ret = cos(r)*mat3(1, 0, 0, 0, 1, 0, 0, 0, 1)
		+(1-cos(r))*mat3(x*x, x*y, x*z, x*y, y*y, y*z, x*z, y*z, z*z)
		+sin(r)*mat3(0, z, -y, -z, 0, x, y, -x, 0);

  // YOUR CODE FOR HW2 HERE
  // Please implement this.  Likely the same as in HW 1.  
  return ret;
}

void Transform::left(float degrees, vec3& eye, vec3& up) 
{
  // YOUR CODE FOR HW2 HERE
  // Likely the same as in HW 1.
	
	mat3 rMatrix = rotate(degrees, up);
	eye = rMatrix * eye;  
}

void Transform::up(float degrees, vec3& eye, vec3& up) 
{
  // YOUR CODE FOR HW2 HERE 
  // Likely the same as in HW 1.
	vec3 z = normalize(eye);
	
	// IMPORTANT: cross product order matters (to get the correct u/x/horizontal-axis, cross(up, eye) not cross(eye, up)
	vec3 x = normalize(cross(up, z));

	mat3 rMatrix = rotate(-degrees, x);

	eye = rMatrix * eye;
	up = cross(normalize(eye), x);
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) 
{
  mat4 ret;
  // YOUR CODE FOR HW2 HERE
  // Likely the same as in HW 1.

	vec3 w = normalize(eye);
	vec3 u = normalize(cross(up, w));
	vec3 v = cross(w, u);

	// this matrix translates the world origin to where the camera is focused on
	mat4 translate = mat4(1, 0, 0, 0,
												0, 1, 0, 0,
												0, 0, 1, 0,
												-eye.x, -eye.y, -eye.z, 1);

	mat4 rotate = mat4(	u.x, v.x, w.x, 0,
											u.y, v.y, w.y, 0,
											u.z, v.z, w.z, 0,
											0, 0, 0, 1);

	// always translate before rotate for the look at matrix
	
	ret = rotate * translate;
  
  return ret;
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
  mat4 ret;
  // YOUR CODE FOR HW2 HERE
  // New, to implement the perspective transform as well.  

	// convert fovy to radians?
	
	float theta=radians(fovy)/2;

	// assuming y'=1
	float d = 1/(tan(theta));

	float f=zFar;
	float n=zNear;

	mat4 perspective = mat4(d/aspect, 0, 0, 0,
													0, d, 0, 0,
													0, 0, (-f+n)/(f-n), -1,
													0, 0, -2*f*n/(f-n), 0);
	// vec4 persp = vec4(x, y, z, 1);

  return perspective;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) 
{
  mat4 ret;
  // YOUR CODE FOR HW2 HERE
  // Implement scaling 

	ret = mat4(sx, 0, 0, 0,
						0, sy, 0, 0,
						0, 0, sz, 0,
						0, 0, 0, 1);

  return ret;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) 
{
  mat4 ret;
  // YOUR CODE FOR HW2 HERE
  // Implement translation 
	
	ret = mat4(1, 0, 0, 0,
						0, 1, 0, 0,
						0, 0, 1, 0,
						tx, ty, tz, 1);

  return ret;
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) 
{
  vec3 x = glm::cross(up,zvec); 
  vec3 y = glm::cross(zvec,x); 
  vec3 ret = glm::normalize(y); 
  return ret; 
}


Transform::Transform()
{

}

Transform::~Transform()
{

}

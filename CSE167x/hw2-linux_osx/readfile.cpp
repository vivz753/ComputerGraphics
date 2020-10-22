/*****************************************************************************/
/* This is the program skeleton for homework 2 in CSE167 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

/*****************************************************************************/
// This file is readfile.cpp.  It includes helper functions for matrix 
// transformations for a stack (matransform) and to rightmultiply the 
// top of a stack.  These functions are given to aid in setting up the 
// transformations properly, and to use glm functions in the right way.  
// Their use is optional in your program.  


// The functions readvals and readfile do basic parsing.  You can of course 
// rewrite the parser as you wish, but we think this basic form might be 
// useful to you.  It is a very simple parser.

// Please fill in parts that say YOUR CODE FOR HW 2 HERE. 
// Read the other parts to get a context of what is going on. 

/*****************************************************************************/

// Basic includes to get this file to work.  
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#include "Transform.h" 

using namespace std;
using namespace glm;
#include "variables.h" 
#include "readfile.h"


// You may not need to use the following two functions, but it is provided
// here for convenience

// The function below applies the appropriate transform to a 4-vector
void matransform(stack<mat4> &transfstack, GLfloat* values) 
{
  mat4 transform = transfstack.top(); 
  vec4 valvec = vec4(values[0],values[1],values[2],values[3]); 
  vec4 newval = transform * valvec; 
  for (int i = 0; i < 4; i++) values[i] = newval[i]; 
}

void rightmultiply(const mat4 & M, stack<mat4> &transfstack) 
{
  mat4 &T = transfstack.top(); 
  T = T * M; 
}

// Function to read the input data values
// Use is optional, but should be very helpful in parsing.  
bool readvals(stringstream &s, const int numvals, GLfloat* values) 
{
  for (int i = 0; i < numvals; i++) {
    s >> values[i]; 
    if (s.fail()) {
      cout << "Failed reading value " << i << " will skip\n"; 
      return false;
    }
  }
  return true; 
}

void readfile(const char* filename) 
{
  string str, cmd; 
  ifstream in;
  in.open(filename); 
  if (in.is_open()) {

    // I need to implement a matrix stack to store transforms.  
    // This is done using standard STL Templates 
    stack <mat4> transfstack; 
    transfstack.push(mat4(1.0));  // identity

    getline (in, str); 
    while (in) {
      if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {
        // Ruled out comment and blank lines 

        stringstream s(str);
        s >> cmd; 
        int i; 
        GLfloat values[10]; // Position and color for light, colors for others
        // Up to 10 params for cameras.  
        bool validinput; // Validity of input 

        // Process the light, add it to database.
        // Lighting Command
        if (cmd == "light") {
          if (numused == numLights) { // No more Lights 
            cerr << "Reached Maximum Number of Lights " << numused << " Will ignore further lights\n";
          } else {
            validinput = readvals(s, 8, values); // Position/color for lts.
            if (validinput) {

              // YOUR CODE FOR HW 2 HERE. 
              // Note that values[0...7] shows the read in values 
              // Make use of lightposn[] and lightcolor[] arrays in variables.h
              // Those arrays can then be used in display too.  

							//matransform(transfstack, values);
							int x = numused*4;

							for (i=0; i<8; i++) {
								if(i<4) {
									lightposn[x+i] = values[i];
									std::cout << "readfile.cpp: storing lightposn[" << x+i << "] the value " << values[i] << std::endl;
								}								
								else if(i>=4) {
									lightcolor[x+(i%4)] = values[i];
									std::cout << "readfile.cpp: storing lightcolor[" << x+(i%4) << "] the value " << values[i] << std::endl;
								}
							}

              ++numused; 
            }
          }
        }

        // Material Commands 
        // Ambient, diffuse, specular, shininess properties for each object.
        // Filling this in is pretty straightforward, so I've left it in 
        // the skeleton, also as a hint of how to do the more complex ones.
        // Note that no transforms/stacks are applied to the colors. 

        else if (cmd == "ambient") {
          validinput = readvals(s, 4, values); // colors 
          if (validinput) {
            for (i = 0; i < 4; i++) {
              ambient[i] = values[i]; 
            }
          }
        } else if (cmd == "diffuse") {
          validinput = readvals(s, 4, values); 
          if (validinput) {
            for (i = 0; i < 4; i++) {
              diffuse[i] = values[i]; 
            }
          }
        } else if (cmd == "specular") {
          validinput = readvals(s, 4, values); 
          if (validinput) {
            for (i = 0; i < 4; i++) {
              specular[i] = values[i]; 
            }
          }
        } else if (cmd == "emission") {
          validinput = readvals(s, 4, values); 
          if (validinput) {
            for (i = 0; i < 4; i++) {
              emission[i] = values[i]; 
            }
          }
        } else if (cmd == "shininess") {
          validinput = readvals(s, 1, values); 
          if (validinput) {
            shininess = values[0]; 
          }
        } else if (cmd == "size") {
          validinput = readvals(s,2,values); 
          if (validinput) { 
            w = (int) values[0]; h = (int) values[1]; 
          } 
        } else if (cmd == "camera") {
          validinput = readvals(s,10,values); // 10 values eye cen up fov
          if (validinput) {

            // YOUR CODE FOR HW 2 HERE
            // Use all of values[0...9]
            // You may need to use the upvector fn in Transform.cpp
            // to set up correctly. 
            // Set eyeinit upinit center fovy in variables.h 
						
						//for(i=0; i<10; i++) { std::cout << values[i] << std::endl; }
						//std::cout << to_string(eyeinit) << to_string(center) << to_string(upinit) << std::endl;

						vec3 lookFrom = vec3(values[0], values[1], values[2]);
						eyeinit = lookFrom;

						vec3 lookAt = vec3(values[3], values[4], values[5]);
						center = lookAt;			
			
						vec3 up = vec3(values[6], values[7], values[8]);
						upinit = Transform::upvector(up, lookAt-lookFrom);

						//std::cout << to_string(lookFrom) << to_string(lookAt) << to_string(upinit) << std::endl;

						fovy = values[9];
          }
        }

        // I've left the code for loading objects in the skeleton, so 
        // you can get a sense of how this works.  
        // Also look at demo.txt to get a sense of why things are done this way.
        else if (cmd == "sphere" || cmd == "cube" || cmd == "teapot") {
          if (numobjects == maxobjects) { // No more objects 
            cerr << "Reached Maximum Number of Objects " << numobjects << " Will ignore further objects\n";
          } else {
            validinput = readvals(s, 1, values); 
            if (validinput) {
              object * obj = &(objects[numobjects]); 
              obj->size = values[0]; 

              // Set the object's light properties
              for (i = 0; i < 4; i++) {
                (obj->ambient)[i] = ambient[i]; 
                (obj->diffuse)[i] = diffuse[i]; 
                (obj->specular)[i] = specular[i]; 
                (obj->emission)[i] = emission[i];
              }
              obj->shininess = shininess; 

              // Set the object's transform
							cout << "readfile.cpp [transfstack.top()] " << endl << to_string(transfstack.top()) << endl << endl;
              obj->transform = transfstack.top(); 
							cout << "readfile.cpp [obj->transform] " << endl << to_string(obj->transform) << endl << endl;

              // Set the object's type
              if (cmd == "sphere") {
                obj->type = sphere; 
              } else if (cmd == "cube") {
                obj->type = cube; 
              } else if (cmd == "teapot") {
                obj->type = teapot; 
              }
            }
            ++numobjects; 
          }
        }

        else if (cmd == "translate") {
          validinput = readvals(s,3,values); 
          if (validinput) {

            // YOUR CODE FOR HW 2 HERE.  
            // Think about how the transformation stack is affected
            // You might want to use helper functions on top of file. 
            // Also keep in mind what order your matrix is!

						mat4 translateMat = Transform::translate(values[0], values[1], values[2]);
						//multiply the stack here
						cout << "values[0]: " << values[0] << " values[1]: " << values[1] << " values[2]: " << values[2] << endl << endl;
						cout << "readfile.cpp: [translateMat] " << endl << to_string(translateMat) << endl << endl;

						
						rightmultiply(translateMat, transfstack);
						//*(&transfstack.top()) = transfstack.top() * translateMat;				
	
						cout << "readfile.cpp: [translate] " << endl << to_string(transfstack.top()) << endl << endl;
					
          }
        }
        else if (cmd == "scale") {
          validinput = readvals(s,3,values); 
          if (validinput) {

            // YOUR CODE FOR HW 2 HERE.  
            // Think about how the transformation stack is affected
            // You might want to use helper functions on top of file.  
            // Also keep in mind what order your matrix is!
						mat4 scaleMat = Transform::scale(values[0], values[1], values[2]);
						//multiply stack
						rightmultiply(scaleMat, transfstack);
						//*(&transfstack.top()) = transfstack.top() * scaleMat;				


						cout << "readfile.cpp: [scale] " << endl << to_string(transfstack.top()) << endl << endl;

          }
        }
        else if (cmd == "rotate") {
          validinput = readvals(s,4,values); 
          if (validinput) {

            // YOUR CODE FOR HW 2 HERE. 
            // values[0..2] are the axis, values[3] is the angle.  
            // You may want to normalize the axis (or in Transform::rotate)
            // See how the stack is affected, as above.  
            // Note that rotate returns a mat3. 
            // Also keep in mind what order your matrix is!

						mat3 rotateMat3 = Transform::rotate(values[3], vec3(values[0], values[1], values[2]));
//std::cout << to_string(rotateMat3) << std::endl;

						//somehow transfer mat3 to mat4
						mat4 rotateMat4 = mat4(rotateMat3); 
//std::cout << glm::to_string(rotateMat4) << std::endl;
						
						//multiply the matrix stack
						rightmultiply(rotateMat4, transfstack);
						//*(&transfstack.top()) = transfstack.top() * rotateMat4;				

						cout << "readfile.cpp: [rotate] " << endl << to_string(transfstack.top()) << endl << endl;;

          }
        }

        // I include the basic push/pop code for matrix stacks
        else if (cmd == "pushTransform") {
          transfstack.push(transfstack.top()); 
        } else if (cmd == "popTransform") {
          if (transfstack.size() <= 1) {
            cerr << "Stack has no elements.  Cannot Pop\n"; 
          } else {
            transfstack.pop(); 
          }
        }

        else {
          cerr << "Unknown Command: " << cmd << " Skipping \n"; 
        }
      }
      getline (in, str); 
    }

    // Set up initial position for eye, up and amount
    // As well as booleans 

    eye = eyeinit; 
    up = upinit; 
    amount = amountinit;
    sx = sy = 1.0;  // keyboard controlled scales in x and y 
    tx = ty = 0.0;  // keyboard controllled translation in x and y  
    useGlu = false; // don't use the glu perspective/lookat fns

    glEnable(GL_DEPTH_TEST);
  } else {
    cerr << "Unable to Open Input Data File " << filename << "\n"; 
    throw 2; 
  }

std::cout << "number of lights: " << numused << std::endl;
}

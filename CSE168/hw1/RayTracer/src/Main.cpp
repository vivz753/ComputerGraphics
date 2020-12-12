#include <iostream>
#include "FreeImage.h"
// common utils and constants
#include "rtweekend.h"
#include "color.h"
#include "sphere.h"
#include "hittable_list.h"

#include <sstream>
#include <fstream>
#include <string>


using namespace std;

void Rasterize(double aspectRatio, int imageWidth, int bitsPerPixel);
void Rasterize(int, int);

double hit_sphere(const point3& center, double radius, const ray& r)
{
	// ray from center of sphere to eye
	vec3 oc = r.origin() - center;
	// expanded from the formula x^2 + y^2 + z^2 = r^2
	// modified to be (P-C)^2 = r^2 where P = A + tb where A is the ray origin and t is a real number (double) and b is the ray direction
	// so (A + tb - C)^2 = r^2 can be expanded to a quadratic equation t^2*b^2 + 2tb(A-C) + (A-C)^2 = r^2 which is in the form of Ax^2 + Bx + C = 0 if we move r^2 to the left side
	// t is the unknown, so it represents the x in the quadratic eq
	// a = t^2*b^2 or tb*tb since tb is the direction in P = A + tb
	auto a = r.direction().length_squared();
	// b = 2*tb*(A-C)
	auto half_b = dot(oc, r.direction());
	// c = (A-C)*(A-C) - r^2
	auto c = oc.length_squared() - radius * radius;
	// uses the quadratic formula's b2-4ac to find real roots
	auto discriminant = half_b * half_b - a * c;
	// return real roots

	if (discriminant < 0) {
		return -1.0;
	}
	else {
		return (-half_b - sqrt(discriminant)) / a;
	}
}

color ray_color(const ray& r, const hittable& world) {
	hit_record rec;

	if (world.hit(r, 0, infinity, rec)) {
		return 0.5 * (rec.normal + color(1, 1, 1));
	}

	// else return the gradient background sky
	vec3 unit_direction = unit_vector(r.direction());
	auto t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

void PrintProgress(int i, int j, int imageWidth, int imageHeight, int progressArray[]) {
	float progress = static_cast<float>((i * (imageHeight)+((j - imageHeight + 1.0) * -1)));
	float total = static_cast<float> ((imageHeight) * (imageWidth));
	int percent = static_cast<float>(progress / total * 100);
	if (progressArray[percent] != 1) {
		std::cout << "Rendering: " << percent << "%" << std::endl;
		progressArray[percent] = 1;
	}
}

void Rasterize(int imageWidth, int imageHeight) {
	double aspectRatio = imageWidth / imageHeight;
	int bpp = 24;
	Rasterize(aspectRatio, imageWidth, bpp);
}

void Rasterize(double aspectRatio, int imageWidth, int bitsPerPixel) {

	// Image

	const int imageHeight = static_cast<int>(imageWidth / aspectRatio);

	// FreeImage setup

	FreeImage_Initialise();
	FIBITMAP* bitmap = FreeImage_Allocate(imageWidth, imageHeight, bitsPerPixel);
	RGBQUAD *freeimage_color = new RGBQUAD();
	if (!bitmap)
		exit(1);

	// World

	hittable_list world;
	world.add(make_shared<sphere>(point3(1, 0, -1), .5));
	world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
	world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));


	// Camera

	// the viewport AKA near clipping plane
	auto viewportHeight = 2.0;
	auto viewportWidth = aspectRatio * viewportHeight;
	// focal point - the distance from near clipping plane to eye AKA projection point--not to be confused with focus distance
	auto focalLength = 1.0;
	auto origin = point3(0, 0, 0);
	auto horizontal = vec3(viewportWidth, 0, 0);
	auto vertical = vec3(0, viewportHeight, 0);
	// we subtract the focalLength bc we are looking into the -z axis, to respect the RH-coordinate system
	auto lowerLeftCorner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focalLength);

	// Progress tracker setup

	// for progress tracking
	std::cout << "imageWidth: " << imageWidth << " imageHeight: " << imageHeight << "\n" << std::endl;
	int printProgress[100] = {};

	// Render loop
	for (int i = 0; i < imageWidth; i++) {
		for (int j = imageHeight-1; j > 0; j--) {

			// print progress
			PrintProgress(i, j, imageWidth, imageHeight, printProgress);

			// uv mappings of pixels
			auto u = double(i) / (imageWidth-1);
			auto v = double(j) / (imageHeight-1);
			ray r(origin, lowerLeftCorner + u * horizontal + v * vertical - origin);
			color pixel_color = ray_color(r, world);

			// converts our color object to RGBQUAD for FreeImage
			write_color(std::cout, pixel_color, freeimage_color);

			// a pointer needs to be passed to the color struct
			FreeImage_SetPixelColor(bitmap, i, j, freeimage_color);
		}
	}
	std::cout << "\nDone.\n";

	if (FreeImage_Save(FIF_PNG, bitmap, "test.png", 0)) std::cout << "Image successfully saved!" << std::endl;

	std::cout << "FreeImage_" << FreeImage_GetVersion() << "\n";
	std::cout << FreeImage_GetCopyrightMessage() << "\n\n";
	FreeImage_DeInitialise();
}

// Taken from CS167X hw2
// Function to read the input data values
// Use is optional, but should be very helpful in parsing.  
bool readvals(stringstream& s, const int numvals, float* values)
{
    for (int i = 0; i < numvals; i++) {
        s >> values[i];
        if (s.fail()) {
            std::cout << "Failed reading value " << i << " will skip\n";
            return false;
        }
    }
    return true;
}

void ReadFile(const char* filename) {
    string str, cmd;
    ifstream in;
    in.open(filename);


    if (in.is_open()) {

        // I need to implement a matrix stack to store transforms.  
        // This is done using standard STL Templates 
        //stack <mat4> transfstack;
        //transfstack.push(mat4(1.0));  // identity

		std::cout << "Reading file " << filename << std::endl;

        
        while (getline(in, str)) {

            if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {
                // Ruled out comment and blank lines 

                stringstream s(str);
                s >> cmd;

				std::cout << str << std::endl;


                int i;
                float v[10]; // Position and color for light, colors for others
                // Up to 10 params for cameras.  
                bool validinput; // Validity of input 

				// Image size
				if (cmd == "size") {
					// width, height
				}
				// Image file output
				else if (cmd == "output") {
					// "name.png"

				}
				// Camera
				else if (cmd == "camera") {
					// lookFrom x, y, z; lookAt x, y, z; R, G, B, A
					if (readvals(s, 10, v)) {
						vec3 lookFrom = vec3(v[0], v[1], v[2]);
						vec3 lookAt = vec3(v[3], v[4], v[5]); // center of image
						vec3 up = unit_vector(vec3(v[6], v[7], v[8]));

						int fovy = v[9];
					}
				}
				// Lights
				else if (cmd == "light") {

				}
				else if (cmd == "point") {

				}
				// Materials
				else if (cmd == "ambient") {

				}
				else if (cmd == "emission") {

				}
				else if (cmd == "diffuse") {

				}
				else if (cmd == "shininess") {

				}
				else if (cmd == "specular") {

				}
				// Matrix access
				else if (cmd == "pushTransform") {

				}
				else if (cmd == "popTransform") {

				}
				// Transformation matrices
				else if (cmd == "translate") {

				}
				else if (cmd == "scale") {

				}
				else if (cmd == "rotate") {

				}
				// Geometry
				else if (cmd == "sphere") {
				
				}
				else if (cmd == "tri") {

				}
				else if (cmd == "maxverts") {

				}
				else if (cmd == "vertex") {

				}

				else {
					cerr << "Unknown Command: " << cmd << "Skipping" << std::endl;
				}
            }
        }
		in.close();
    }
    else {
        cerr << "Unable to Open Input Data File " << filename << "\n";
    }
}


int main() {

	const double aspectRatio = 16.0 / 9.0;
	const int imageWidth = 500;
	const int bitsPerPixel = 24;
	//Rasterize(aspectRatio, imageWidth, bitsPerPixel);
	string filename = "C:/dev/vivz753/ComputerGraphics/CSE168/hw1/RayTracer/src/homework1-submissionscenes/scene4-diffuse.test";
	ReadFile(filename.c_str());
}
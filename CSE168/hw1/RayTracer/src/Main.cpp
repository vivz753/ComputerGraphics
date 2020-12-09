#include <iostream>
#include "FreeImage.h"
#include "color.h"
#include "vec3.h"
#include "ray.h"

double hit_sphere(const point3& center, double radius, const ray& r)
{
	// ray from center of sphere to eye
	vec3 oc = r.origin() - center;
	// expanded from the formula x^2 + y^2 + z^2 = r^2
	// modified to be (P-C)^2 = r^2 where P = A + tb where A is the ray origin and t is a real number (double) and b is the ray direction
	// so (A + tb - C)^2 = r^2 can be expanded to a quadratic equation t^2*b^2 + 2tb(A-C) + (A-C)^2 = r^2 which is in the form of Ax^2 + Bx + C = 0 if we move r^2 to the left side
	// t is the unknown, so it represents the x in the quadratic eq
	// a = t^2*b^2 or tb*tb since tb is the direction in P = A + tb
	auto a = dot(r.direction(), r.direction());
	// b = 2*tb*(A-C)
	auto b = 2.0 * dot(oc, r.direction());
	// c = (A-C)*(A-C) - r^2
	auto c = dot(oc, oc) - radius * radius;
	// uses the quadratic formula's b2-4ac to find real roots
	auto discriminant = b * b - 4 * a * c;
	// return real roots

	if (discriminant < 0) {
		return -1.0;
	}
	else {
		return (-b - sqrt(discriminant)) / (2.0 * a);
	}
}

color ray_color(const ray& r) {
	// if ray hits sphere at 0, 0, -1 w/ a radius of 0.5, return red color
	//if (hit_sphere(point3(0, 0, -1), 0.5, r)) {
	//	return color(1, 0, 0);
	//}

	// get the hit points on the sphere, color their normals
	auto t = hit_sphere(point3(0, 0, -1), 0.5, r);
	if (t > 0.0) {
		vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));
		return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);
	}

	// else return the gradient background sky
	vec3 unit_direction = unit_vector(r.direction());
	t = 0.5 * (unit_direction.y() + 1.0);
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

void Rasterize(double aspectRatio, int imageWidth, int bitsPerPixel) {

	const int imageHeight = static_cast<int>(imageWidth / aspectRatio);

	// FreeImage setup
	FreeImage_Initialise();
	FIBITMAP* bitmap = FreeImage_Allocate(imageWidth, imageHeight, bitsPerPixel);
	RGBQUAD *freeimage_color = new RGBQUAD();
	if (!bitmap)
		exit(1);

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

	// for progress tracking
	std::cout << "imageWidth: " << imageWidth << " imageHeight: " << imageHeight << "\n" << std::endl;
	int printProgress[100] = {};

	//Draws a gradient from blue to green
	for (int i = 0; i < imageWidth; i++) {
		for (int j = imageHeight-1; j > 0; j--) {

			// print progress
			PrintProgress(i, j, imageWidth, imageHeight, printProgress);

			// uv mappings of pixels
			auto u = double(i) / (imageWidth-1);
			auto v = double(j) / (imageHeight-1);
			ray r(origin, lowerLeftCorner + u * horizontal + v * vertical - origin);
			color pixel_color = ray_color(r);

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

int main() {

	const double aspectRatio = 16.0 / 9.0;
	const int imageWidth = 500;
	const int bitsPerPixel = 24;
	Rasterize(aspectRatio, imageWidth, bitsPerPixel);
}
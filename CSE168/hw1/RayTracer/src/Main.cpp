#include <iostream>
#include "FreeImage.h"
#include "color.h"
#include "vec3.h"
#include "ray.h"

using namespace std;

color ray_color(const ray& r) {
	vec3 unit_direction = unit_vector(r.direction());
	auto t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

void Rasterize(double aspectRatio, int imageWidth, int bitsPerPixel) {

	//const int WIDTH(50), HEIGHT(50), BPP(24);

	const int imageHeight = static_cast<int>(imageWidth / aspectRatio);

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

	//Draws a gradient from blue to green
	for (int i = 0; i < imageWidth; i++) {
		for (int j = imageHeight-1; j > 0; j--) {
			//color.rgbRed = 0;
			//color.rgbGreen = (double)i / WIDTH * 255.0;
			//color.rgbBlue = (double)j / HEIGHT * 255.0;

			//color pixel_color(0.25, double(i) / imageWidth, double(j) / imageHeight);

			auto u = double(i) / (imageWidth-1);
			auto v = double(j) / (imageHeight-1);
			ray r(origin, lowerLeftCorner + u * horizontal + v * vertical - origin);
			color pixel_color = ray_color(r);

			write_color(std::cout, pixel_color, freeimage_color);

			FreeImage_SetPixelColor(bitmap, i, j, freeimage_color);
			cout << "pixel: " << i << ", " << j << endl;
			// Notice the & operator on color; this passes the pointer to the color struct
		}
	}

	if (FreeImage_Save(FIF_PNG, bitmap, "test.png", 0)) cout << "Image successfully saved!" << endl;

	cout << "FreeImage_" << FreeImage_GetVersion() << "\n";
	cout << FreeImage_GetCopyrightMessage() << "\n\n";
	FreeImage_DeInitialise();
}

int main() {

	const double aspectRatio = 16.0 / 9.0;
	const int imageWidth = 25;
	const int bitsPerPixel = 24;
	Rasterize(aspectRatio, imageWidth, bitsPerPixel);
}
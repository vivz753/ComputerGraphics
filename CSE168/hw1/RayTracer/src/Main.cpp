#include <iostream>
#include "FreeImage.h"
#include "Vector3.h"
#include "Point3.h"

using namespace std;

typedef Vector3<Float> Vector3f;
typedef Vector3<int> Vector3i;

typedef Point3<Float> Point3f;
typedef Point3<int>   Point3i;

void Rasterize() {
	const int WIDTH(500), HEIGHT(500), BPP(24);

	FreeImage_Initialise();

	FIBITMAP* bitmap = FreeImage_Allocate(WIDTH, HEIGHT, BPP);
	RGBQUAD color;

	if (!bitmap)
		exit(1);

	//Draws a gradient from blue to green
	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			color.rgbRed = 0;
			color.rgbGreen = (double)i / WIDTH * 255.0;
			color.rgbBlue = (double)j / HEIGHT * 255.0;
			FreeImage_SetPixelColor(bitmap, i, j, &color);
			cout << "pixel: " << i << ", " << j << endl;
			// Notice the & operator on "color; this passes the pointer to the color struct
		}
	}

	if (FreeImage_Save(FIF_PNG, bitmap, "test.png", 0)) cout << "Image successfully saved!" << endl;

	cout << "FreeImage_" << FreeImage_GetVersion() << "\n";
	cout << FreeImage_GetCopyrightMessage() << "\n\n";
	FreeImage_DeInitialise();
}

int main() {

	Vector3i v = Vector3i(0, 0, 1);
	//Vector3<int> v = Vector3<int>(0, 0, 1);

	v = v * 2;
	v.print();
}
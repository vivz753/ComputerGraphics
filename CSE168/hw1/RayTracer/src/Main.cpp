#include <iostream>
#include "FreeImage.h"
#include "color.h"
#include "vec3.h"

using namespace std;

void Rasterize() {
	const int WIDTH(50), HEIGHT(50), BPP(24);

	FreeImage_Initialise();

	FIBITMAP* bitmap = FreeImage_Allocate(WIDTH, HEIGHT, BPP);
	RGBQUAD *freeimage_color = new RGBQUAD();

	if (!bitmap)
		exit(1);

	//Draws a gradient from blue to green
	for (int i = 0; i < WIDTH; i++) {
		for (int j = HEIGHT-1; j > 0; j--) {
			//color.rgbRed = 0;
			//color.rgbGreen = (double)i / WIDTH * 255.0;
			//color.rgbBlue = (double)j / HEIGHT * 255.0;

			color pixel_color(0.25, double(i) / WIDTH, double(j) / HEIGHT);

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

	Rasterize();
}
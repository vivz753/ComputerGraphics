#ifndef COLOR_H
#define COLOR_H

#include "Vec3.h"
#include "FreeImage.h"

#include <iostream>

void write_color(std::ostream& out, color pixel_color, RGBQUAD *color) {
    // Write the translated [0,255] value of each color component.
    color->rgbRed = pixel_color.x() *255.999;
    color->rgbGreen = pixel_color.y() * 255.999;
    color->rgbBlue = pixel_color.z() * 255.999;

    // prints out color value
    //out << static_cast<int>(255.999 * pixel_color.x()) << ' '
    //    << static_cast<int>(255.999 * pixel_color.y()) << ' '
    //    << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}

#endif
#include "RGB.hpp"

RGB::RGB() : r(0), g(0), b(0) {}

RGB::RGB(unsigned char red, unsigned char green, unsigned char blue)
    : r(red), g(green), b(blue) {}

unsigned char RGB::getRed() const {
    return r;
}

unsigned char RGB::getGreen() const {
    return g;
}

unsigned char RGB::getBlue() const {
    return b;
}

void RGB::setRed(unsigned char red) {
    r = red;
}


void RGB::setGreen(unsigned char green) {
    g = green;
}


void RGB::setBlue(unsigned char blue) {
    b = blue;
}


bool RGB::operator==(const RGB& other) const {
    return r == other.r && g == other.g && b == other.b;
}

bool RGB::operator!=(const RGB& other) const {
    return !(*this == other);
}
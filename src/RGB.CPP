#include "RGB.hpp"

// Constructor default
RGB::RGB() : r(0), g(0), b(0) {}

// Constructor dengan nilai RGB
RGB::RGB(unsigned char red, unsigned char green, unsigned char blue)
    : r(red), g(green), b(blue) {}

// Getter untuk komponen merah
unsigned char RGB::getRed() const {
    return r;
}

// Getter untuk komponen hijau
unsigned char RGB::getGreen() const {
    return g;
}

// Getter untuk komponen biru
unsigned char RGB::getBlue() const {
    return b;
}

// Setter untuk komponen merah
void RGB::setRed(unsigned char red) {
    r = red;
}

// Setter untuk komponen hijau
void RGB::setGreen(unsigned char green) {
    g = green;
}

// Setter untuk komponen biru
void RGB::setBlue(unsigned char blue) {
    b = blue;
}

// Operator perbandingan kesamaan
bool RGB::operator==(const RGB& other) const {
    return r == other.r && g == other.g && b == other.b;
}

// Operator perbandingan ketidaksamaan
bool RGB::operator!=(const RGB& other) const {
    return !(*this == other);
}
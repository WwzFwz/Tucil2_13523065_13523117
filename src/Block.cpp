#include "Block.hpp"

// Constructor default
Block::Block() : x(0), y(0), width(0), height(0) {}

// Constructor dengan posisi dan dimensi
Block::Block(int _x, int _y, int _width, int _height)
    : x(_x), y(_y), width(_width), height(_height) {}

// Getter untuk koordinat x
int Block::getX() const {
    return x;
}

// Getter untuk koordinat y
int Block::getY() const {
    return y;
}

// Getter untuk lebar
int Block::getWidth() const {
    return width;
}

// Getter untuk tinggi
int Block::getHeight() const {
    return height;
}

// Setter untuk koordinat x
void Block::setX(int _x) {
    x = _x;
}

// Setter untuk koordinat y
void Block::setY(int _y) {
    y = _y;
}

// Setter untuk lebar
void Block::setWidth(int _width) {
    width = _width;
}

// Setter untuk tinggi
void Block::setHeight(int _height) {
    height = _height;
}

// Operator perbandingan kesamaan
bool Block::operator==(const Block& other) const {
    return x == other.x && y == other.y && 
           width == other.width && height == other.height;
}

// Mendapatkan Block untuk kuadran tertentu
Block Block::getQuadrant(int quadrant) const {
    int halfWidth = width / 2;
    int halfHeight = height / 2;
    
    switch (quadrant) {
        case 0: // Top-left
            return Block(x, y, halfWidth, halfHeight);
        case 1: // Top-right
            return Block(x + halfWidth, y, halfWidth, halfHeight);
        case 2: // Bottom-left
            return Block(x, y + halfHeight, halfWidth, halfHeight);
        case 3: // Bottom-right
            return Block(x + halfWidth, y + halfHeight, halfWidth, halfHeight);
        default:
            return Block(); // Default empty block
    }
}

// Memeriksa apakah sebuah titik berada di dalam block
bool Block::contains(int pointX, int pointY) const {
    return pointX >= x && pointX < x + width && 
           pointY >= y && pointY < y + height;
}
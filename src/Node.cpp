#include "Node.hpp"


Node::Node(const Block& _region, const RGB& _avgColor, bool _isLeaf)
    : region(_region), avgColor(_avgColor), isLeaf(_isLeaf),
      topLeft(nullptr), topRight(nullptr), bottomLeft(nullptr), bottomRight(nullptr) {}


const Block& Node::getRegion() const {
    return region;
}


const RGB& Node::getAvgColor() const {
    return avgColor;
}


bool Node::getIsLeaf() const {
    return isLeaf;
}

// Mendapatkan child node pada kuadran tertentu
Node* Node::getChild(int quadrant) const {
    switch (quadrant) {
        case 0: // Top-left
            return topLeft.get();
        case 1: // Top-right
            return topRight.get();
        case 2: // Bottom-left
            return bottomLeft.get();
        case 3: // Bottom-right
            return bottomRight.get();
        default:
            return nullptr;
    }
}
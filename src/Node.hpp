#ifndef __NODE__HPP__
#define __NODE__HPP__

#include <memory>
#include "Block.hpp"
#include "RGB.hpp"

using namespace std;
class QuadTree;

/**
 * @class Node
 * @brief Kelas untuk merepresentasikan node dalam QuadTree
 */
class Node {
    private:
        Block region;    
        RGB avgColor;    
        bool isLeaf;     
        
        // Child nodes
        unique_ptr<Node> topLeft;
        unique_ptr<Node> topRight;
        unique_ptr<Node> bottomLeft;
        unique_ptr<Node> bottomRight;
        
        friend class QuadTree;
        
    public:
        /**
         * @brief Constructor untuk Node
         * @param Region yang direpresentasikan oleh node
         * @param Warna rata-rata region
         * @param Apakah node ini leaf node
         */
        Node(const Block&, const RGB&, bool = true);
        

        const Block& getRegion() const;
        const RGB& getAvgColor() const;
        bool getIsLeaf() const;
        
        /**
         * @brief Mendapatkan child node pada kuadran tertentu
         * @param Nomor kuadran (0=top-left, 1=top-right, 2=bottom-left, 3=bottom-right)
         * @return Pointer ke child node pada kuadran yang diminta
         */
        Node* getChild(int) const;
};

#endif 
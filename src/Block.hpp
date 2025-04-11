#ifndef __BLOCK__HPP__
#define __BLOCK__HPP__

using namespace std;

/**
 * @class Block
 * @brief Kelas untuk merepresentasikan area persegi dalam gambar
 */
class Block {
private:
    int x, y, width, height;
    
public:
    Block();
    /**
     * @brief Constructor dengan posisi dan dimensi
     * @param Koordinat x dari pojok kiri atas
     * @param Koordinat y dari pojok kiri atas
     * @param Lebar 
     * @param Tinggi 
     */
    Block(int, int, int, int);
    int getX() const;
    int getY() const;
    int getWidth() const;
    int getHeight() const;
    void setX(int);
    void setY(int);
    void setWidth(int);
    void setHeight(int);
    /**
     * @brief Perbandingan kesamaan
     * @param Block yang dibandingkan
     * @return true jika sama, false jika berbeda
     */
    bool operator==(const Block&) const;
    
    /**
     * @brief Mendapatkan persegi untuk kuadran tertentu
     * @param Nomor kuadran (0=top-left, 1=top-right, 2=bottom-left, 3=bottom-right)
     * @return Block baru untuk kuadran yang diminta
     */
    Block getQuadrant(int) const;
    /**
     * @brief Memeriksa apakah sebuah titik berada di dalam persegi
     * @param Koordinat x dari titik
     * @param Koordinat y dari titik
     * @return true jika titik berada di dalam persegi, false jika di luar
     */
    bool contains(int, int) const;
    
    friend class QuadTree;
    friend class Node;
};

#endif 
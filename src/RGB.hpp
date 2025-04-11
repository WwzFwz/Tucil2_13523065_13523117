#ifndef __RGB__HPP__
#define __RGB__HPP__

/**
 * @class RGB
 * @brief Kelas untuk merepresentasikan warna RGB
 */
class RGB {
private:
    unsigned char r, g, b;
    
public:
    /**
     * Constructor default
     */
    RGB();
    
    /**
     * Constructor dengan nilai RGB
     * komponen merah (0-255)
     * komponen hijau (0-255)
     * komponen biru (0-255)
     */
    RGB(unsigned char, unsigned char, unsigned char);
    unsigned char getRed() const;
    unsigned char getGreen() const;
    unsigned char getBlue() const;
    void setRed(unsigned char);
    void setGreen(unsigned char);
    void setBlue(unsigned char);
    bool operator==(const RGB&) const;
    bool operator!=(const RGB&) const;
    
    friend class QuadTree;
    friend class Node;
    friend class ErrorMetrics;
};

#endif 
#ifndef __QUADTREE__HPP__
#define __QUADTREE__HPP__

#include <memory>
#include <vector>
#include <functional>
#include "Node.hpp"
#include "RGB.hpp"
#include "Block.hpp"

using namespace std;

/**
 * @class QuadTree
 * @brief Kelas untuk kompresi gambar menggunakan struktur data QuadTree
 */
class QuadTree {
public:

    enum ErrorMetricType {
        VARIANCE,              
        MEAN_ABSOLUTE_DEVIATION, 
        MAX_PIXEL_DIFFERENCE,   
        ENTROPY,                
        SSIM                    
    };
    
    // Tipe untuk callback visualisasi proses kompresi
    using CompressionCallback = function<void(const vector<vector<RGB>>&)>;
    
private:
    unique_ptr<Node> root;       // Root node dari QuadTree
    vector<vector<RGB>> image;   // Gambar original
    int minBlockSize;            // Ukuran blok minimum untuk subdivisi
    double threshold;            // Threshold error untuk subdivisi
    ErrorMetricType errorMetric; // Metrik error yang digunakan
    int nodeCount;               // Jumlah node dalam tree
    int maxDepth;                // Kedalaman maksimum tree
    
    CompressionCallback compressionCallback;
    
public:
    /**
     * @brief Constructor untuk QuadTree
     * @param Gambar yang akan dikompresi
     * @param Ukuran blok minimum
     * @param Threshold error
     * @param Metrik error yang digunakan
     */
    QuadTree(const vector<vector<RGB>>&, int, double, ErrorMetricType);
    
    /**
     * @brief Membangun QuadTree dengan pendekatan divide and conquer
     */
    void buildTree();
    
    /**
     * @brief Mendapatkan hasil kompresi sebagai gambar
     * @return Gambar hasil kompresi
     */
    vector<vector<RGB>> getCompressedImage() const;
    
    /**
     * @brief Mendapatkan jumlah node dalam tree
     * @return Jumlah node
     */
    int getNodeCount() const;
    
    /**
     * @brief Mendapatkan kedalaman maksimum tree
     * @return Kedalaman maksimum
     */
    int getMaxDepth() const;
    
    /**
     * @brief Menghitung persentase kompresi
     * @return Persentase kompresi (0.0-1.0)
     */
    double getCompressionPercentage() const;
    
    /**
     * @brief Menetapkan callback untuk visualisasi proses kompresi
     * @param Fungsi callback yang akan dipanggil pada setiap langkah kompresi
     */
    void setCompressionCallback(CompressionCallback);
    
    /**
     * @brief Mendapatkan gambaran state tree saat ini
     * @return Gambar yang merepresentasikan state tree saat ini
     */
    vector<vector<RGB>> getCurrentStateImage() const;

    void setCompressionRegionCallback(const std::function<void(const Block&, const RGB&)>& cb);

    
private:
    /**
     * @brief Membagi node dengan pendekatan divide and conquer
     * @param Node yang akan dibagi
     * @param Kedalaman saat ini dalam tree
     */
    void subdivide(Node*, int);
    
    /**
     * @brief Menghitung warna rata-rata dari region
     * @param Region yang akan dihitung warna rata-ratanya
     * @return Warna rata-rata region
     */
    RGB calculateAverageColor(const Block&) const;
    
    /**
     * @brief Menghitung error untuk region berdasarkan metode yang dipilih
     * @param Region yang akan dihitung error-nya
     * @param Warna rata-rata region
     * @return Nilai error
     */
    double calculateError(const Block&, const RGB&) const;

    /**
     * @brief Menghitung kedalaman maksimum dari pohon
     * @param Node saat ini
     * @param Kedalaman saat ini
     * @return Kedalaman maksimum
     */
 
    int calculateMaxDepth(const Node*, int) const;
    
    /**
     * @brief Menghitung jumlah node dalam pohon
     * @param Node saat ini
     * @return Jumlah node
     */
    int countNodes(const Node*) const;
    
    /**
     * @brief Membangun gambar hasil kompresi
     * @param Gambar hasil kompresi
     * @param Node saat ini
     */
    void buildCompressedImage(vector<vector<RGB>>&, const Node*) const;
    std::function<void(const Block&, const RGB&)> compressionRegionCallback;

};

#endif 
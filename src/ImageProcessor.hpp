#ifndef __IMAGE_PROCESSOR__HPP__
#define __IMAGE_PROCESSOR__HPP__

#include <string>
#include <chrono>
#include <vector>
#include <memory>
#include "QuadTree.hpp"
#include "RGB.hpp"


using namespace std;

/**
 * @class ImageProcessor
 * @brief Kelas untuk menangani pemrosesan dan kompresi gambar
 */
class ImageProcessor {
private:
    string inputPath;           
    string outputPath;       
    string gifPath;             
    
    int width, height, channels;     // Dimensi dan jumlah channel gambar
    vector<vector<RGB>> originalImage;    
    vector<vector<RGB>> compressedImage;  
    
    int minBlockSize;                
    double threshold;                      
    QuadTree::ErrorMetricType errorMetricType;  
    double targetCompressionPercentage;    
    
    unique_ptr<QuadTree> quadTree;         // QuadTree untuk kompresi
    
    // Metrik hasil kompresi
    double compressionPercentage;
    int nodeCount;
    int maxDepth;
    chrono::milliseconds executionTime;
    size_t originalSize;
    size_t compressedSize;
    
public:
    /**
     * @brief Constructor untuk ImageProcessor
     * @param Path gambar input
     * @param Path gambar output
     * @param Ukuran blok minimum
     * @param Threshold error
     * @param Jenis metrik error yang digunakan
     * @param Target persentase kompresi (0.0-1.0, 0 untuk menonaktifkan)
     * @param Path untuk GIF output (opsional, bonus)
     */
    ImageProcessor(const string&, const string&,
                   int, double, 
                   QuadTree::ErrorMetricType,
                   double = 0.0,
                   const string& = "");
    
    bool loadImage();
    
    /**
     * @brief Melakukan kompresi gambar
     * @return true jika berhasil, false jika gagal
     */
    bool compressImage();
    
    bool saveCompressedImage();
    void displayMetrics() const;
    
    double getCompressionPercentage() const;
    int getNodeCount() const;
    
    /**
     *  kedalaman maksimum QuadTree
     */
    int getMaxDepth() const;
    chrono::milliseconds getExecutionTime() const;
    size_t getOriginalSize() const;
    
    size_t getCompressedSize() const;
    
private:
    /**
     * @brief Mencari threshold yang sesuai untuk target persentase kompresi (Bonus)
     * @return Threshold yang sesuai
     */
    double findThresholdForTargetCompression();
    
    /**
     * @brief Membuat GIF visualisasi proses kompresi (Bonus)
     * @return true jika berhasil, false jika gagal
     */
    bool generateCompressionGif();
    
    /**
     * @brief Menghitung ukuran file gambar
     * @param Gambar yang dihitung ukurannya
     * @return Ukuran gambar dalam bytes
     */
    size_t calculateImageSize(const vector<vector<RGB>>&) const;
};

#endif 
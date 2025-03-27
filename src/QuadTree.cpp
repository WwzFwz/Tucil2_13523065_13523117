#include "QuadTree.hpp"
#include "ErrorMetrics.hpp"
#include <algorithm>
#include <iostream>

// Constructor
QuadTree::QuadTree(const vector<vector<RGB>>& _image, int _minBlockSize, double _threshold, 
                  ErrorMetricType _errorMetric)
    : image(_image), minBlockSize(_minBlockSize), threshold(_threshold), 
      errorMetric(_errorMetric), nodeCount(0), maxDepth(0), root(nullptr) {
}

// Membangun QuadTree dengan pendekatan divide and conquer
void QuadTree::buildTree() {
    // Validasi gambar
    if (image.empty() || image[0].empty()) {
        std::cerr << "Error: Empty image" << std::endl;
        return;
    }
    
    // Buat root node dengan seluruh gambar sebagai region
    int width = image[0].size();
    int height = image.size();
    Block fullImageBlock(0, 0, width, height);
    
    // Hitung warna rata-rata untuk seluruh gambar
    RGB avgColor = calculateAverageColor(fullImageBlock);
    
    // Buat root node
    root = make_unique<Node>(fullImageBlock, avgColor);
    
    // Mulai proses subdivisi dari root node
    subdivide(root.get(), 0);
    
    // Hitung jumlah node dan kedalaman maksimum
    nodeCount = countNodes(root.get());
    maxDepth = calculateMaxDepth(root.get(), 0);
}

// Mendapatkan hasil kompresi sebagai gambar
vector<vector<RGB>> QuadTree::getCompressedImage() const {
    // Buat gambar kosong dengan ukuran yang sama
    int height = image.size();
    int width = image.empty() ? 0 : image[0].size();
    vector<vector<RGB>> result(height, vector<RGB>(width));
    
    // Jika tree belum dibangun
    if (!root) {
        return result;
    }
    
    // Rekursi untuk membangun gambar hasil kompresi
    buildCompressedImage(result, root.get());
    
    return result;
}

// Getter untuk jumlah node
int QuadTree::getNodeCount() const {
    return nodeCount;
}

// Getter untuk kedalaman maksimum
int QuadTree::getMaxDepth() const {
    return maxDepth;
}

// Menghitung persentase kompresi
double QuadTree::getCompressionPercentage() const {
    // Ukuran gambar asli (setiap piksel = 3 byte RGB)
    size_t originalSize = image.size() * image[0].size() * 3;
    
    // Ukuran terkompresi (setiap node leaf = 3 byte untuk warna + 16 byte untuk posisi & ukuran)
    size_t compressedSize = nodeCount * (3 + 16);
    
    // Hitung persentase kompresi
    double compressionRatio = 1.0 - (static_cast<double>(compressedSize) / originalSize);
    
    // Batasi ke range 0.0-1.0
    return std::max(0.0, std::min(1.0, compressionRatio));
}

// Menetapkan callback untuk visualisasi proses kompresi
void QuadTree::setCompressionCallback(CompressionCallback callback) {
    compressionCallback = callback;
}

// Mendapatkan gambaran state tree saat ini
vector<vector<RGB>> QuadTree::getCurrentStateImage() const {
    // Buat gambar kosong dengan ukuran yang sama
    int height = image.size();
    int width = image.empty() ? 0 : image[0].size();
    vector<vector<RGB>> result(height, vector<RGB>(width));
    
    // Jika tree belum dibangun
    if (!root) {
        return result;
    }
    
    // Rekursi untuk membangun gambar hasil kompresi saat ini
    buildCompressedImage(result, root.get());
    
    return result;
}

// Private methods

// Membagi node dengan pendekatan divide and conquer
void QuadTree::subdivide(Node* node, int depth) {
    if (!node) return;
    static int nodeProcessedCount = 0;
    
    // Hitung error untuk region saat ini
    double error = calculateError(node->region, node->avgColor);

    if (error > 1000000 || error < 0) { // Deteksi nilai ekstrem   //DEBUGGING
        std::cerr << "Warning: Extreme error value " << error 
                  << " for method " << errorMetric << std::endl;
    }
    
    // Tentukan apakah perlu subdivisi
    bool shouldSubdivide = error > threshold;
    int subBlockSize = node->region.getWidth() / 2; // Ukuran sub-blok
    
    // Periksa ukuran minimum blok
    if (subBlockSize < minBlockSize) {
        shouldSubdivide = false;
    }
    
    if (shouldSubdivide) {
        nodeProcessedCount++;
        // Ubah status node menjadi internal (bukan leaf)
        node->isLeaf = false;
        
        // Buat sub-regions untuk 4 kuadran
        Block topLeftRegion = node->region.getQuadrant(0);
        Block topRightRegion = node->region.getQuadrant(1);
        Block bottomLeftRegion = node->region.getQuadrant(2);
        Block bottomRightRegion = node->region.getQuadrant(3);
        
        // Hitung warna rata-rata untuk setiap kuadran
        RGB topLeftColor = calculateAverageColor(topLeftRegion);
        RGB topRightColor = calculateAverageColor(topRightRegion);
        RGB bottomLeftColor = calculateAverageColor(bottomLeftRegion);
        RGB bottomRightColor = calculateAverageColor(bottomRightRegion);
        
        // Buat child nodes
        node->topLeft = make_unique<Node>(topLeftRegion, topLeftColor);
        node->topRight = make_unique<Node>(topRightRegion, topRightColor);
        node->bottomLeft = make_unique<Node>(bottomLeftRegion, bottomLeftColor);
        node->bottomRight = make_unique<Node>(bottomRightRegion, bottomRightColor);
        
        // Panggil callback untuk visualisasi jika tersedia
        if (nodeProcessedCount % 100 == 0 && compressionCallback) {
            compressionCallback(getCurrentStateImage());
        }
        
        // Rekursif subdivisi child nodes
        subdivide(node->topLeft.get(), depth + 1);
        subdivide(node->topRight.get(), depth + 1);
        subdivide(node->bottomLeft.get(), depth + 1);
        subdivide(node->bottomRight.get(), depth + 1);
    }
}

// Menghitung warna rata-rata dari region
RGB QuadTree::calculateAverageColor(const Block& region) const {
    int width = region.getWidth();
    int height = region.getHeight();
    int startX = region.getX();
    int startY = region.getY();
    
    if (width <= 0 || height <= 0 || 
        startX >= image[0].size() || startY >= image.size()) {
        return RGB(0, 0, 0);
    }
    
    long sumR = 0, sumG = 0, sumB = 0;
    int count = 0;
    
    for (int y = startY; y < startY + height && y < image.size(); y++) {
        for (int x = startX; x < startX + width && x < image[y].size(); x++) {
            sumR += image[y][x].getRed();
            sumG += image[y][x].getGreen();
            sumB += image[y][x].getBlue();
            count++;
        }
    }
    
    if (count == 0) {
        return RGB(0, 0, 0);
    }
    
    unsigned char avgR = static_cast<unsigned char>(sumR / count);
    unsigned char avgG = static_cast<unsigned char>(sumG / count);
    unsigned char avgB = static_cast<unsigned char>(sumB / count);
    
    return RGB(avgR, avgG, avgB);
}

// Menghitung error untuk region berdasarkan metode yang dipilih
double QuadTree::calculateError(const Block& region, const RGB& avgColor) const {
    switch (errorMetric) {
        case VARIANCE:
            return calculateVariance(region, avgColor);
        case MEAN_ABSOLUTE_DEVIATION:
            return calculateMAD(region, avgColor);
        case MAX_PIXEL_DIFFERENCE:
            return calculateMaxDifference(region, avgColor);
        case ENTROPY:
            return calculateEntropy(region);
        case SSIM:
            return calculateSSIM(region, avgColor);
        default:
            return calculateVariance(region, avgColor);
    }
}


// Menghitung error menggunakan metode variance
double QuadTree::calculateVariance(const Block& region, const RGB& avgColor) const {
    return ErrorMetrics::calculateVariance(this->image, region, avgColor);
}

// Menghitung error menggunakan metode Mean Absolute Deviation
double QuadTree::calculateMAD(const Block& region, const RGB& avgColor) const {
    return ErrorMetrics::calculateMAD(this->image, region, avgColor);
}

// Menghitung error menggunakan metode Max Pixel Difference
double QuadTree::calculateMaxDifference(const Block& region, const RGB& avgColor) const {
    return ErrorMetrics::calculateMaxDifference(this->image, region, avgColor);
}

// Menghitung error menggunakan metode Entropy
double QuadTree::calculateEntropy(const Block& region) const {
    return ErrorMetrics::calculateEntropy(this->image, region);
}

// Menghitung error menggunakan metode SSIM
double QuadTree::calculateSSIM(const Block& region, const RGB& avgColor) const {
    return ErrorMetrics::calculateSSIM(this->image, region, avgColor);
}
// Menghitung kedalaman maksimum dari pohon
int QuadTree::calculateMaxDepth(const Node* node, int currentDepth) const {
    if (!node) {
        return currentDepth - 1;
    }
    
    // Jika leaf node, kembalikan kedalaman saat ini
    if (node->isLeaf) {
        return currentDepth;
    }
    
    // Rekursi ke semua child nodes dan ambil kedalaman maksimum
    int depthTL = calculateMaxDepth(node->topLeft.get(), currentDepth + 1);
    int depthTR = calculateMaxDepth(node->topRight.get(), currentDepth + 1);
    int depthBL = calculateMaxDepth(node->bottomLeft.get(), currentDepth + 1);
    int depthBR = calculateMaxDepth(node->bottomRight.get(), currentDepth + 1);
    
    return std::max({depthTL, depthTR, depthBL, depthBR});
}

// Menghitung jumlah node dalam pohon
int QuadTree::countNodes(const Node* node) const {
    if (!node) {
        return 0;
    }
    
    // Hitung node saat ini
    int count = 1;
    
    // Jika bukan leaf node, tambahkan jumlah node dari semua child
    if (!node->isLeaf) {
        count += countNodes(node->topLeft.get());
        count += countNodes(node->topRight.get());
        count += countNodes(node->bottomLeft.get());
        count += countNodes(node->bottomRight.get());
    }
    
    return count;
}

// Membangun gambar hasil kompresi
void QuadTree::buildCompressedImage(vector<vector<RGB>>& result, const Node* node) const {
    if (!node) {
        return;
    }
    
    // Jika leaf node, isi region dengan warna rata-rata
    if (node->isLeaf) {
        int startX = node->region.getX();
        int startY = node->region.getY();
        int width = node->region.getWidth();
        int height = node->region.getHeight();
        
        for (int y = startY; y < startY + height && y < result.size(); y++) {
            for (int x = startX; x < startX + width && x < result[y].size(); x++) {
                result[y][x] = node->avgColor;
            }
        }
    } else {
        // Jika bukan leaf node, rekursi ke semua child nodes
        buildCompressedImage(result, node->topLeft.get());
        buildCompressedImage(result, node->topRight.get());
        buildCompressedImage(result, node->bottomLeft.get());
        buildCompressedImage(result, node->bottomRight.get());
    }
}
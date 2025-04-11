#include "ErrorMetrics.hpp"
#include <cmath>
#include <map>
#include <algorithm>
#include <numeric>

// Menghitung variance RGB dalam sebuah region
double ErrorMetrics::calculateVariance(const vector<vector<RGB>>& image, 
                                       const Block& region, 
                                       const RGB& avgColor) {
    // Menghitung variance untuk setiap channel dan mengambil rata-ratanya
    double varR = calculateChannelVariance(image, region, avgColor, 0);
    double varG = calculateChannelVariance(image, region, avgColor, 1);
    double varB = calculateChannelVariance(image, region, avgColor, 2);
    
    // Rata-rata variance dari ketiga channel
    return (varR + varG + varB) / 3.0;
}

// Menghitung variance untuk satu channel warna
double ErrorMetrics::calculateChannelVariance(const vector<vector<RGB>>& image, 
                                             const Block& region, 
                                             const RGB& avgColor, 
                                             int channelOffset) {
    int width = region.getWidth();
    int height = region.getHeight();
    int startX = region.getX();
    int startY = region.getY();
    
    if (width <= 0 || height <= 0) {
        return 0.0;
    }
    
    unsigned char avgVal;
    if (channelOffset == 0) avgVal = avgColor.getRed();
    else if (channelOffset == 1) avgVal = avgColor.getGreen();
    else avgVal = avgColor.getBlue();
    
    double sumSquaredDiff = 0.0;
    int count = 0;
    
    // Untuk setiap piksel dalam region
    for (int y = startY; y < startY + height && y < image.size(); y++) {
        for (int x = startX; x < startX + width && x < image[y].size(); x++) {
            unsigned char pixelVal;
            if (channelOffset == 0) pixelVal = image[y][x].getRed();
            else if (channelOffset == 1) pixelVal = image[y][x].getGreen();
            else pixelVal = image[y][x].getBlue();
            
            // Akumulasi perbedaan kuadrat
            double diff = static_cast<double>(pixelVal) - static_cast<double>(avgVal);
            sumSquaredDiff += diff * diff;
            count++;
        }
    }
    
    // Jika tidak ada piksel yang valid
    if (count == 0) {
        return 0.0;
    }
    
    // Hitung variance (rata-rata perbedaan kuadrat)
    return sumSquaredDiff / count;
}

// Menghitung Mean Absolute Deviation (MAD) dalam sebuah region
double ErrorMetrics::calculateMAD(const vector<vector<RGB>>& image, 
                                  const Block& region, 
                                  const RGB& avgColor) {
    // Menghitung MAD untuk setiap channel dan mengambil rata-ratanya
    double madR = calculateChannelMAD(image, region, avgColor, 0);
    double madG = calculateChannelMAD(image, region, avgColor, 1);
    double madB = calculateChannelMAD(image, region, avgColor, 2);
    
    // Rata-rata MAD dari ketiga channel
    return (madR + madG + madB) / 3.0;
}

// Menghitung MAD untuk satu channel warna
double ErrorMetrics::calculateChannelMAD(const vector<vector<RGB>>& image, 
                                        const Block& region, 
                                        const RGB& avgColor, 
                                        int channelOffset) {
    int width = region.getWidth();
    int height = region.getHeight();
    int startX = region.getX();
    int startY = region.getY();
    
    if (width <= 0 || height <= 0) {
        return 0.0;
    }
    
    unsigned char avgVal;
    if (channelOffset == 0) avgVal = avgColor.getRed();
    else if (channelOffset == 1) avgVal = avgColor.getGreen();
    else avgVal = avgColor.getBlue();
    
    double sumAbsDiff = 0.0;
    int count = 0;
    
    // Untuk setiap piksel dalam region
    for (int y = startY; y < startY + height && y < image.size(); y++) {
        for (int x = startX; x < startX + width && x < image[y].size(); x++) {
            unsigned char pixelVal;
            if (channelOffset == 0) pixelVal = image[y][x].getRed();
            else if (channelOffset == 1) pixelVal = image[y][x].getGreen();
            else pixelVal = image[y][x].getBlue();
            
            // Akumulasi absolute difference
            double diff = abs(static_cast<double>(pixelVal) - static_cast<double>(avgVal));
            sumAbsDiff += diff;
            count++;
        }
    }
    
    // Jika tidak ada piksel yang valid
    if (count == 0) {
        return 0.0;
    }
    
    // Hitung MAD (rata-rata absolute difference)
    return sumAbsDiff / count;
}

// Menghitung perbedaan piksel maksimum dalam sebuah region
double ErrorMetrics::calculateMaxDifference(const vector<vector<RGB>>& image, 
                                           const Block& region, 
                                           const RGB& avgColor) {
    // Menghitung max difference untuk setiap channel dan mengambil rata-ratanya
    double maxDiffR = calculateChannelMaxDifference(image, region, avgColor, 0);
    double maxDiffG = calculateChannelMaxDifference(image, region, avgColor, 1);
    double maxDiffB = calculateChannelMaxDifference(image, region, avgColor, 2);
    
    // Rata-rata max difference dari ketiga channel
    return (maxDiffR + maxDiffG + maxDiffB) / 3.0;
}

// Menghitung perbedaan maksimum untuk satu channel warna
double ErrorMetrics::calculateChannelMaxDifference(const vector<vector<RGB>>& image, 
                                                 const Block& region, 
                                                 const RGB& avgColor, 
                                                 int channelOffset) {
    int width = region.getWidth();
    int height = region.getHeight();
    int startX = region.getX();
    int startY = region.getY();
    
    if (width <= 0 || height <= 0) {
        return 0.0;
    }
    
    unsigned char avgVal;
    if (channelOffset == 0) avgVal = avgColor.getRed();
    else if (channelOffset == 1) avgVal = avgColor.getGreen();
    else avgVal = avgColor.getBlue();
    
    unsigned char minVal = 255;
    unsigned char maxVal = 0;
    
    // Cari nilai minimum dan maksimum dalam region
    for (int y = startY; y < startY + height && y < image.size(); y++) {
        for (int x = startX; x < startX + width && x < image[y].size(); x++) {
            unsigned char pixelVal;
            if (channelOffset == 0) pixelVal = image[y][x].getRed();
            else if (channelOffset == 1) pixelVal = image[y][x].getGreen();
            else pixelVal = image[y][x].getBlue();
            
            minVal = min(minVal, pixelVal);
            maxVal = max(maxVal, pixelVal);
        }
    }
    
    // Hitung perbedaan maksimum
    return static_cast<double>(maxVal - minVal);
}

// Menghitung entropy dalam sebuah region
double ErrorMetrics::calculateEntropy(const vector<vector<RGB>>& image, 
                                     const Block& region) {
    // Menghitung entropy untuk setiap channel dan mengambil rata-ratanya
    double entropyR = calculateChannelEntropy(image, region, 0);
    double entropyG = calculateChannelEntropy(image, region, 1);
    double entropyB = calculateChannelEntropy(image, region, 2);
    
    // Rata-rata entropy dari ketiga channel
    return (entropyR + entropyG + entropyB) / 3.0;
}

// Menghitung entropy untuk satu channel warna
double ErrorMetrics::calculateChannelEntropy(const vector<vector<RGB>>& image, 
                                           const Block& region, 
                                           int channelOffset) {
    int width = region.getWidth();
    int height = region.getHeight();
    int startX = region.getX();
    int startY = region.getY();
    
    if (width <= 0 || height <= 0) {
        return 0.0;
    }
    
    // Hitung histogram (frekuensi) nilai piksel (0-255)
    map<unsigned char, int> histogram;
    int totalPixels = 0;
    
    for (int y = startY; y < startY + height && y < image.size(); y++) {
        for (int x = startX; x < startX + width && x < image[y].size(); x++) {
            unsigned char pixelVal;
            if (channelOffset == 0) pixelVal = image[y][x].getRed();
            else if (channelOffset == 1) pixelVal = image[y][x].getGreen();
            else pixelVal = image[y][x].getBlue();
            
            histogram[pixelVal]++;
            totalPixels++;
        }
    }
    
    // Jika tidak ada piksel yang valid
    if (totalPixels == 0) {
        return 0.0;
    }
    
    // Hitung entropy menggunakan rumus: -sum(p * log2(p))
    double entropy = 0.0;
    for (const auto& pair : histogram) {
        double probability = static_cast<double>(pair.second) / totalPixels;
        if (probability > 0.0) {
            entropy -= probability * log2(probability);
        }
    }
    
    return entropy;
}

// Menghitung Structural Similarity Index (SSIM) dalam sebuah region (bonus)
double ErrorMetrics::calculateSSIM(const vector<vector<RGB>>& image, 
                                  const Block& region, 
                                  const RGB& avgColor) {
    // Konstanta untuk SSIM
    const double C1 = 0.01 * 255 * 0.01 * 255;  // (k1*L)^2, L=255, k1=0.01
    const double C2 = 0.03 * 255 * 0.03 * 255;  // (k2*L)^2, L=255, k2=0.03
    
    // Menghitung SSIM untuk setiap channel dan mengambil weighted average
    double ssimR = calculateChannelSSIM(image, region, avgColor, 0);
    double ssimG = calculateChannelSSIM(image, region, avgColor, 1);
    double ssimB = calculateChannelSSIM(image, region, avgColor, 2);
    
    // Weighted average (human eye is more sensitive to green)
    return 0.299 * ssimR + 0.587 * ssimG + 0.114 * ssimB;
}

// Menghitung SSIM untuk satu channel warna
double ErrorMetrics::calculateChannelSSIM(const vector<vector<RGB>>& image, 
                                        const Block& region, 
                                        const RGB& avgColor, 
                                        int channelOffset) {
    int width = region.getWidth();
    int height = region.getHeight();
    int startX = region.getX();
    int startY = region.getY();
    
    if (width <= 0 || height <= 0) {
        return 1.0;  // SSIM = 1 untuk region kosong (sama persis)
    }
    
    unsigned char avgVal;
    if (channelOffset == 0) avgVal = avgColor.getRed();
    else if (channelOffset == 1) avgVal = avgColor.getGreen();
    else avgVal = avgColor.getBlue();
    
    // Kita sudah tahu nilai rata-rata dari original region (avgVal)
    double mu1 = static_cast<double>(avgVal);
    double mu2 = mu1;  // Untuk region yang sudah dikompresi, nilai semua piksel = avgVal
    
    // Hitung variance untuk region asli
    double sigma1_sq = 0.0;
    int count = 0;
    
    for (int y = startY; y < startY + height && y < image.size(); y++) {
        for (int x = startX; x < startX + width && x < image[y].size(); x++) {
            unsigned char pixelVal;
            if (channelOffset == 0) pixelVal = image[y][x].getRed();
            else if (channelOffset == 1) pixelVal = image[y][x].getGreen();
            else pixelVal = image[y][x].getBlue();
            
            double diff = static_cast<double>(pixelVal) - mu1;
            sigma1_sq += diff * diff;
            count++;
        }
    }
    
    if (count > 0) {
        sigma1_sq /= count;
    }
    
    // Untuk region terkompresi, semua piksel sama, jadi variance = 0
    double sigma2_sq = 0.0;
    
    // Covariance juga 0 karena region terkompresi tidak memiliki variasi
    double sigma12 = 0.0;
    
    // Konstanta untuk stabilitas
    const double C1 = 0.01 * 255 * 0.01 * 255;
    const double C2 = 0.03 * 255 * 0.03 * 255;
    
    // Hitung SSIM
    double ssim = ((2 * mu1 * mu2 + C1) * (2 * sigma12 + C2)) / 
                  ((mu1 * mu1 + mu2 * mu2 + C1) * (sigma1_sq + sigma2_sq + C2));
    
    // SSIM seharusnya berada di range [-1, 1], nilai lebih tinggi = lebih mirip
    // Kita ingin mengubahnya menjadi error metric (0 = mirip, nilai tinggi = beda)
    return 1.0 - ssim;
}
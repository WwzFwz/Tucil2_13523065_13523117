
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define GIF_IMPL  
#include "ImageProcessor.hpp"
#include "Utils.hpp"
#include <chrono>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cmath>
#include <windows.h>
#undef RGB


#include "lib/stb_image.h"
#include "lib/stb_image_write.h"
#include "lib/gif.h"


#define GIF_IMPL

// Constructor
ImageProcessor::ImageProcessor(const string& _inputPath, const string& _outputPath,
                             int _minBlockSize, double _threshold, 
                             QuadTree::ErrorMetricType _errorMetricType,
                             double _targetCompressionPercentage,
                             const string& _gifPath)
    : inputPath(_inputPath), outputPath(_outputPath), gifPath(_gifPath),
      width(0), height(0), channels(0), minBlockSize(_minBlockSize),
      threshold(_threshold), errorMetricType(_errorMetricType),
      targetCompressionPercentage(_targetCompressionPercentage),
      quadTree(nullptr), compressionPercentage(0.0),
      nodeCount(0), maxDepth(0), executionTime(0),
      originalSize(0), compressedSize(0) {
}

// Memuat gambar dari file
bool ImageProcessor::loadImage() {
    // Cek apakah file ada
    if (!Utils::fileExists(inputPath)) {
        std::cerr << "Error: Input file does not exist - " << inputPath << std::endl;
        return false;
    }
    
    // Load gambar menggunakan stb_image
    stbi_set_flip_vertically_on_load(false);
    unsigned char* data = stbi_load(inputPath.c_str(), &width, &height, &channels, 0);
    
    if (!data) {
        std::cerr << "Error: Failed to load image - " << inputPath << std::endl;
        return false;
    }
    
    if (minBlockSize > (width * height)) {
        std::cerr << "Error: Minimum block area (" << minBlockSize << " square pixels) is larger than the image area (" 
                  << (width * height) << " square pixels)." << std::endl;
        stbi_image_free(data);  
        return false;
    }
    // Konversi data gambar ke format yang kita gunakan
    originalImage.resize(height);
    for (int y = 0; y < height; ++y) {
        originalImage[y].resize(width);
        for (int x = 0; x < width; ++x) {
            int idx = (y * width + x) * channels;
            unsigned char r = channels >= 1 ? data[idx] : 0;
            unsigned char g = channels >= 2 ? data[idx + 1] : r;
            unsigned char b = channels >= 3 ? data[idx + 2] : r;
            originalImage[y][x] = RGB(r, g, b);
        }
    }
    
    // Hitung ukuran file original
    originalSize = Utils::getFileSize(inputPath);
    
    // Free memory
    stbi_image_free(data);
    
    std::cout << "Image loaded: " << width << "x" << height 
              << ", channels: " << channels << std::endl;
    
    return true;
}

// Melakukan kompresi gambar
bool ImageProcessor::compressImage() {
    try {
        auto startTime = std::chrono::high_resolution_clock::now();
        
        // Jika target persentase kompresi diaktifkan
        if (targetCompressionPercentage > 0.0) {
            std::cout << "Adaptive threshold mode enabled." << std::endl;
            std::cout << "Original threshold: " << threshold << std::endl;
            
            // Cari threshold yang optimal untuk mencapai target persentase
            double newThreshold = findThresholdForTargetCompression();
            
            std::cout << "Adjusted threshold: " << newThreshold << std::endl;
            threshold = newThreshold;
        }
        
        // Buat dan bangun QuadTree
        quadTree = make_unique<QuadTree>(originalImage, minBlockSize, threshold, errorMetricType);
        
        // Set callback jika gif path diset
        if (!gifPath.empty()) {
            std::cout << "GIF recording enabled." << std::endl;
            // Capture frame untuk visualisasi
            vector<vector<vector<RGB>>> frames;
            
            quadTree->setCompressionCallback([&frames](const vector<vector<RGB>>& frame) {
                frames.push_back(frame);
                std::cout << "Frame captured. Total frames: " << frames.size() << "\r" << std::flush;
            });
        }
        
        // Bangun tree
        std::cout << "Building quadtree..." << std::endl;
        quadTree->buildTree();
        
        // Dapatkan hasil kompresi dan metrik
        std::cout << "Retrieving compressed image..." << std::endl;
        compressedImage = quadTree->getCompressedImage();
        nodeCount = quadTree->getNodeCount();
        maxDepth = quadTree->getMaxDepth();
        compressionPercentage = quadTree->getCompressionPercentage();
        
        auto endTime = std::chrono::high_resolution_clock::now();
        executionTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        
        // Hitung perkiraan ukuran file terkompresi
        compressedSize = calculateImageSize(compressedImage);
        
        // Jika path GIF ditentukan, buat GIF visualisasi
        if (!gifPath.empty()) {
            std::cout << "Generating compression visualization GIF..." << std::endl;
            generateCompressionGif();
        }
        
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error during image compression: " << e.what() << std::endl;
        return false;
    }
}

// Menyimpan gambar hasil kompresi
bool ImageProcessor::saveCompressedImage() {
    // Validasi
    if (compressedImage.empty() || compressedImage[0].empty()) {
        std::cerr << "Error: No compressed image to save" << std::endl;
        return false;
    }
    
    // Prepare data untuk stb_image_write
    unsigned char* data = new unsigned char[width * height * 3]; // Always save as RGB
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int idx = (y * width + x) * 3;
            data[idx] = compressedImage[y][x].getRed();
            data[idx + 1] = compressedImage[y][x].getGreen();
            data[idx + 2] = compressedImage[y][x].getBlue();
        }
    }
    
    // Tentukan format output berdasarkan ekstensi file
    bool success = false;
    string ext = Utils::getFileExtension(outputPath);
    
    if (ext == "jpg" || ext == "jpeg") {
        success = stbi_write_jpg(outputPath.c_str(), width, height, 3, data, 90); // quality 90
    } else if (ext == "png") {
        success = stbi_write_png(outputPath.c_str(), width, height, 3, data, width * 3);
    } else if (ext == "bmp") {
        success = stbi_write_bmp(outputPath.c_str(), width, height, 3, data);
    } else {
        // Default to PNG
        success = stbi_write_png(outputPath.c_str(), width, height, 3, data, width * 3);
    }
    
    delete[] data;
    
    if (!success) {
        std::cerr << "Error: Failed to save compressed image to " << outputPath << std::endl;
        return false;
    }
    
    return true;
}
// Menampilkan semua metrik hasil kompresi
void ImageProcessor::displayMetrics() const {
    std::cout << "\n=== Compression Results ===" << std::endl;
    
    // Informasi tentang threshold dan metode
    std::cout << "Error metric     : " << Utils::errorMetricToString(errorMetricType) << std::endl;
    
    if (targetCompressionPercentage > 0.0) {
        std::cout << "Target compression: " << std::fixed << std::setprecision(1) 
                  << (targetCompressionPercentage * 100) << "%" << std::endl;
        std::cout << "Final threshold  : " << threshold << std::endl;
    } else {
        std::cout << "Threshold        : " << threshold << std::endl;
    }
    
    // Ukuran file dan persentase kompresi
    std::cout << "Original size    : " << originalSize << " bytes" << std::endl;
    
    // Perkiraan ukuran terkompresi berdasarkan file output yang sebenarnya
    size_t actualCompressedSize = compressedSize; // Gunakan nilai saat ini sebagai default
    if (Utils::fileExists(outputPath)) {
        actualCompressedSize = Utils::getFileSize(outputPath);
    }
    
    std::cout << "Compressed size  : " << actualCompressedSize << " bytes" << std::endl;
    
    double actualCompression = 100.0 * (1.0 - static_cast<double>(actualCompressedSize) / originalSize);
    std::cout << "Compression      : " << std::fixed << std::setprecision(2) 
              << actualCompression << "%" << std::endl;
    
    // Statistik QuadTree
    std::cout << "Min block size   : " << minBlockSize << " pixels" << std::endl;
    std::cout << "QuadTree nodes   : " << nodeCount << std::endl;
    std::cout << "Maximum depth    : " << maxDepth << std::endl;
    
    // Waktu eksekusi
    std::cout << "Execution time   : " << executionTime.count() << " ms" << std::endl;
}


// Getter untuk persentase kompresi
double ImageProcessor::getCompressionPercentage() const {
    return compressionPercentage;
}

// Getter untuk jumlah node
int ImageProcessor::getNodeCount() const {
    return nodeCount;
}

// Getter untuk kedalaman maksimum
int ImageProcessor::getMaxDepth() const {
    return maxDepth;
}

// Getter untuk waktu eksekusi
chrono::milliseconds ImageProcessor::getExecutionTime() const {
    return executionTime;
}

// Getter untuk ukuran gambar asli
size_t ImageProcessor::getOriginalSize() const {
    return originalSize;
}

// Getter untuk ukuran gambar terkompresi
size_t ImageProcessor::getCompressedSize() const {
    return compressedSize;
}

// Mencari threshold yang sesuai untuk target persentase kompresi
double ImageProcessor::findThresholdForTargetCompression() {
    // Inisialisasi nilai batas atas dan bawah untuk binary search
    double lowerBound = 0.1;    // Nilai minimum threshold yang masuk akal
    double upperBound = 1000.0; 
    double currentThreshold;    // Threshold yang akan dicoba pada iterasi ini
    
    // Parameter tambahan untuk mengontrol algoritma
    const double tolerance = 0.01;  // Toleransi 1% untuk target persentase
    const int maxIterations = 15;   // Maksimum iterasi untuk mencegah infinite loop
    int iterations = 0;
    
    // Metrik untuk melacak hasil terbaik
    double bestThreshold = threshold; // Mulai dengan threshold awal
    double bestDifference = 1.0;      // Perbedaan terbaik (initialize dengan nilai besar)
    
    std::cout << "Target compression percentage: " << (targetCompressionPercentage * 100) << "%" << std::endl;
    std::cout << "Starting adaptive threshold search..." << std::endl;
    
    // Binary search untuk menemukan threshold yang optimal
    while (iterations < maxIterations && (upperBound - lowerBound) > 0.01) {
        // Hitung threshold di tengah range saat ini
        currentThreshold = (lowerBound + upperBound) / 2.0;
        
        // Buat QuadTree dengan threshold ini untuk pengujian
        QuadTree testTree(originalImage, minBlockSize, currentThreshold, errorMetricType);
        testTree.buildTree();
        
        // Hitung persentase kompresi yang dicapai dengan threshold ini
        double achievedCompression = testTree.getCompressionPercentage();
        
        // Hitung perbedaan dengan target
        double difference = std::fabs(achievedCompression - targetCompressionPercentage);
        
        std::cout << "  Iteration " << (iterations+1) << ": threshold = " << currentThreshold 
                  << ", achieved = " << (achievedCompression * 100) << "%, diff = " 
                  << (difference * 100) << "%" << std::endl;
        
        // Simpan hasil terbaik sejauh ini
        if (difference < bestDifference) {
            bestDifference = difference;
            bestThreshold = currentThreshold;
        }
    
        if (difference <= tolerance) {
            std::cout << "Target achieved within tolerance!" << std::endl;
            return currentThreshold;
        }
        

        if (achievedCompression < targetCompressionPercentage) {

            lowerBound = currentThreshold;
        } else {
            upperBound = currentThreshold;
        }
        
        iterations++;
    }
    
    std::cout << "Search completed. Best threshold = " << bestThreshold 
              << " (difference = " << (bestDifference * 100) << "%)" << std::endl;
    
 
    return bestThreshold;
}


bool ImageProcessor::generateCompressionGif() {
    try {
      
        std::string directory = gifPath.substr(0, gifPath.find_last_of("/\\"));
        if (!directory.empty()) {
            Utils::createDirectoryIfNotExists(directory);
        }


        GifWriter gifWriter;
        if (!GifBegin(&gifWriter, gifPath.c_str(), width, height, 10)) {
            std::cerr << "Failed to initialize GIF writer\n";
            return false;
        }

        std::cout << "Generating GIF (streaming)...\n";

   
        std::vector<std::vector<RGB>> gifBuffer = originalImage;

   
        int frameCounter = 0;
        const int frameInterval = 100;  


        auto callback = [&](const Block& region, const RGB& avgColor) {
    
            for (int y = region.getY(); y < region.getY() + region.getHeight(); ++y) {
                for (int x = region.getX(); x < region.getX() + region.getWidth(); ++x) {
                    gifBuffer[y][x] = avgColor;
                }
            }

        
            if (frameCounter % frameInterval == 0) {
                std::vector<uint8_t> frameData(width * height * 4);
                for (int y = 0; y < height; ++y) {
                    for (int x = 0; x < width; ++x) {
                        int idx = (y * width + x) * 4;
                        const RGB& pixel = gifBuffer[y][x];
                        frameData[idx + 0] = pixel.getRed();
                        frameData[idx + 1] = pixel.getGreen();
                        frameData[idx + 2] = pixel.getBlue();
                        frameData[idx + 3] = 255;
                    }
                }
                std::cout << "Callback triggered at frameCounter = " << frameCounter << std::endl;
                GifWriteFrame(&gifWriter, frameData.data(), width, height, 10);
                std::cout << "Frame " << (frameCounter / frameInterval) << " written\n";
            }

            frameCounter++;

            Sleep(1);

        };

        QuadTree treeCopy(originalImage, minBlockSize, threshold, errorMetricType);
        treeCopy.setCompressionRegionCallback(callback);  
        treeCopy.buildTree();

            
        std::vector<uint8_t> finalFrame(width * height * 4);
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int idx = (y * width + x) * 4;
                const RGB& pixel = compressedImage[y][x];
                finalFrame[idx + 0] = pixel.getRed();
                finalFrame[idx + 1] = pixel.getGreen();
                finalFrame[idx + 2] = pixel.getBlue();
                finalFrame[idx + 3] = 255;
            }
        }
        GifWriteFrame(&gifWriter, finalFrame.data(), width, height, 10);
        std::cout << "Final frame written\n";

        GifEnd(&gifWriter);
        std::cout << "GIF created successfully at " << gifPath << std::endl;
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Compression GIF generation error: " << e.what() << std::endl;
        return false;
    }
}


size_t ImageProcessor::calculateImageSize(const vector<vector<RGB>>& img) const {
    if (img.empty() || img[0].empty()) {
        return 0;
    }
    
    // Perkiraan kasar: 3 byte per piksel (RGB) + overhead
    return img.size() * img[0].size() * 3;
}
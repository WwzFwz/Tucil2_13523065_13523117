#include <iostream>
#include <string>
#include <iomanip>
#include "QuadTree.hpp"
#include "ImageProcessor.hpp"
#include "Utils.hpp"

// Di main.cpp - pastikan ini ada dan tidak dikomentari
#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "lib/stb_image_write.h"

// kemudian kode main Anda...

using namespace std;

int main(int argc, char* argv[]) {
    // Parse command line arguments
    Utils::CommandLineParser parser(argc, argv);
    if (!parser.parse()) {
        return 1;
    }
    
    cout << "=== Quadtree Image Compression ===" << endl;
    
    // Buat image processor dengan parameter dari command line
    ImageProcessor processor(
        parser.getInputFilePath(),
        parser.getOutputFilePath(),
        parser.getMinBlockSize(),
        parser.getThreshold(),
        static_cast<QuadTree::ErrorMetricType>(parser.getErrorMetricType() - 1), // Convert to zero-based index
        parser.getTargetCompression(),
        parser.getGifOutputPath()
    );
    
    cout << "Configuration:" << endl;
    cout << "  Input file      : " << parser.getInputFilePath() << endl;
    cout << "  Output file     : " << parser.getOutputFilePath() << endl;
    cout << "  Error metric    : " << parser.getErrorMetricType() << endl;
    cout << "  Initial threshold: " << parser.getThreshold() << endl;
    cout << "  Min block size  : " << parser.getMinBlockSize() << endl;
    
    // Tampilkan informasi target kompresi jika diaktifkan
    if (parser.getTargetCompression() > 0.0) {
        cout << "  Target compression: " << fixed << setprecision(1) 
             << (parser.getTargetCompression() * 100) << "%" << endl;
        cout << "  [Adaptive threshold mode enabled]" << endl;
    } else {
        cout << "  Target compression: disabled" << endl;
    }
    
    if (!parser.getGifOutputPath().empty()) {
        cout << "  GIF output      : " << parser.getGifOutputPath() << endl;
    }
    
    cout << endl;
    
    // Load gambar
    if (!processor.loadImage()) {
        cerr << "Error: Failed to load image from " << parser.getInputFilePath() << endl;
        return 1;
    }
    
    cout << "Image loaded successfully." << endl;
    
    // Kompresi gambar
    cout << "Compressing image..." << endl;
    if (!processor.compressImage()) {
        cerr << "Error: Failed to compress image." << endl;
        return 1;
    }
    
    // Simpan hasil kompresi
    cout << "Saving compressed image..." << endl;
    if (!processor.saveCompressedImage()) {
        cerr << "Error: Failed to save compressed image to " << parser.getOutputFilePath() << endl;
        return 1;
    }
    
    // Tampilkan metrik hasil kompresi
    processor.displayMetrics();
    
    cout << "Image successfully compressed and saved to " << parser.getOutputFilePath() << endl;
    
    if (!parser.getGifOutputPath().empty()) {
        cout << "Visualization GIF saved to " << parser.getGifOutputPath() << endl;
    }
    
    return 0;
}
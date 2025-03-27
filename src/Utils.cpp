#include "Utils.hpp"
#include <fstream>
#include <sys/stat.h>
#include <algorithm>
#include <iostream>
#include <stdexcept>

#ifdef _WIN32
#include <direct.h>
#endif

// Namespace Utils implementation

// Mengecek apakah file ada
bool Utils::fileExists(const string& filename) {
    std::ifstream file(filename);
    return file.good();
}

// Mendapatkan ekstensi file
string Utils::getFileExtension(const string& filename) {
    size_t pos = filename.find_last_of(".");
    if (pos != string::npos) {
        string ext = filename.substr(pos + 1);
        // Convert to lowercase
        std::transform(ext.begin(), ext.end(), ext.begin(), 
                      [](unsigned char c) { return std::tolower(c); });
        return ext;
    }
    return "";
}

// Menghitung ukuran file
size_t Utils::getFileSize(const string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.good()) {
        return 0;
    }
    return file.tellg();
}

// Mengkonversi nama metode error menjadi string untuk display
string Utils::errorMetricToString(QuadTree::ErrorMetricType errorMetric) {
    switch (errorMetric) {
        case QuadTree::VARIANCE:
            return "Variance";
        case QuadTree::MEAN_ABSOLUTE_DEVIATION:
            return "Mean Absolute Deviation (MAD)";
        case QuadTree::MAX_PIXEL_DIFFERENCE:
            return "Max Pixel Difference";
        case QuadTree::ENTROPY:
            return "Entropy";
        case QuadTree::SSIM:
            return "Structural Similarity Index (SSIM)";
        default:
            return "Unknown";
    }
}

// Membuat direktori jika belum ada
bool Utils::createDirectoryIfNotExists(const string& path) {
    struct stat info;
    
    // Cek apakah direktori sudah ada
    if (stat(path.c_str(), &info) == 0) {
        // Pastikan ini adalah direktori
        return (info.st_mode & S_IFDIR);
    }
    
    // Buat direktori baru
#ifdef _WIN32
    return (_mkdir(path.c_str()) == 0);
#else
    return (mkdir(path.c_str(), 0755) == 0);
#endif
}

// Menampilkan progress bar
void Utils::displayProgressBar(int progress, int total) {
    const int barWidth = 50;
    float percentage = static_cast<float>(progress) / total;
    int pos = static_cast<int>(barWidth * percentage);
    
    std::cout << "[";
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    
    std::cout << "] " << int(percentage * 100.0) << " %\r";
    std::cout.flush();
}

// CommandLineParser implementation

// Constructor
Utils::CommandLineParser::CommandLineParser(int _argc, char** _argv)
    : argc(_argc), argv(_argv), errorMetricType(0), threshold(0.0), 
      minBlockSize(1), targetCompression(0.0) {}

// Parse command line arguments
bool Utils::CommandLineParser::parse() {
    if (argc < 6) {
        displayHelp();
        return false;
    }
    
    try {
        // Parameter wajib
        inputFilePath = argv[1];
        errorMetricType = std::stoi(argv[2]);
        threshold = std::stod(argv[3]);
        minBlockSize = std::stoi(argv[4]);
        
        // Validasi parameter wajib
        if (errorMetricType < 1 || errorMetricType > 5) {
            std::cerr << "Error: Invalid error metric type (must be 1-5)" << std::endl;
            return false;
        }
        
        if (threshold < 0) {
            std::cerr << "Error: Threshold must be non-negative" << std::endl;
            return false;
        }
        
        if (minBlockSize < 1) {
            std::cerr << "Error: Minimum block size must be at least 1" << std::endl;
            return false;
        }
        
        // Parameter target persentase kompresi
        if (argc >= 6) {
            targetCompression = std::stod(argv[5]);
            
            // Validasi target kompresi
            if (targetCompression < 0.0 || targetCompression > 1.0) {
                if (targetCompression != 0.0) { // 0.0 diperbolehkan untuk menonaktifkan fitur
                    std::cerr << "Warning: Target compression must be between 0.0-1.0 (0.0 to disable)" << std::endl;
                    std::cerr << "         Value will be clamped to valid range" << std::endl;
                    targetCompression = std::max(0.0, std::min(1.0, targetCompression));
                }
            }
        }
        
        // Output path (wajib)
        if (argc >= 7) {
            outputFilePath = argv[6];
        } else {
            std::cerr << "Error: Output file path is required" << std::endl;
            return false;
        }
        
        // Path GIF (opsional)
        if (argc >= 8) {
            gifOutputPath = argv[7];
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error parsing command line arguments: " << e.what() << std::endl;
        return false;
    }
    
    return true;
}

// Menampilkan bantuan penggunaan
void Utils::CommandLineParser::displayHelp() const {
    std::cout << "Quadtree Image Compression" << std::endl;
    std::cout << "Usage: " << (argc > 0 ? argv[0] : "program") 
              << " <input_image> <error_metric> <threshold> <min_block_size> <target_compression> <output_image> [gif_output]" << std::endl;
    std::cout << std::endl;
    std::cout << "Parameters:" << std::endl;
    std::cout << "  input_image       : Path to the input image file" << std::endl;
    std::cout << "  error_metric      : Error metric type (1=Variance, 2=MAD, 3=MaxDiff, 4=Entropy, 5=SSIM)" << std::endl;
    std::cout << "  threshold         : Error threshold for subdivision" << std::endl;
    std::cout << "  min_block_size    : Minimum block size (pixels)" << std::endl;
    std::cout << "  target_compression: Target compression percentage (0.0-1.0, 0=disable)" << std::endl;
    std::cout << "                      When enabled, threshold will be adjusted automatically" << std::endl;
    std::cout << "  output_image      : Path to save the compressed image" << std::endl;
    std::cout << "  gif_output        : (Optional) Path to save visualization GIF" << std::endl;
    std::cout << std::endl;
    std::cout << "Example:" << std::endl;
    std::cout << "  " << (argc > 0 ? argv[0] : "program") 
              << " input.jpg 1 20 4 0.7 output.jpg output.gif" << std::endl;
    std::cout << "  This will compress input.jpg using Variance metric with" << std::endl;
    std::cout << "  automatically adjusted threshold to achieve 70% compression" << std::endl;
}

// Getter untuk input file path
string Utils::CommandLineParser::getInputFilePath() const {
    return inputFilePath;
}

// Getter untuk output file path
string Utils::CommandLineParser::getOutputFilePath() const {
    return outputFilePath;
}

// Getter untuk error metric type
int Utils::CommandLineParser::getErrorMetricType() const {
    return errorMetricType;
}

// Getter untuk threshold
double Utils::CommandLineParser::getThreshold() const {
    return threshold;
}

// Getter untuk minimum block size
int Utils::CommandLineParser::getMinBlockSize() const {
    return minBlockSize;
}

// Getter untuk target compression
double Utils::CommandLineParser::getTargetCompression() const {
    return targetCompression;
}

// Getter untuk GIF output path
string Utils::CommandLineParser::getGifOutputPath() const {
    return gifOutputPath;
}
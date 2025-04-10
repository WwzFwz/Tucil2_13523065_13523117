// Utils.cpp
#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

bool Utils::fileExists(const std::string& filePath) {
    std::ifstream file(filePath);
    return file.good();
}

size_t Utils::getFileSize(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file.good()) {
        std::cerr << "Error getting file size for " << filePath << std::endl;
        return 0;
    }
    return static_cast<size_t>(file.tellg());
}

std::string Utils::getFileExtension(const std::string& filePath) {
    size_t dotPos = filePath.find_last_of('.');
    if (dotPos != std::string::npos) {
        return filePath.substr(dotPos + 1);
    }
    return "";
}

bool Utils::isAbsolutePath(const std::string& path) {
    return true;
}

std::string Utils::toAbsolutePath(const std::string& path) {
    fs::path p(path);
    if (p.is_absolute()) {
        return path; 
    }

    return fs::absolute(p).string(); 
}

std::string Utils::errorMetricToString(QuadTree::ErrorMetricType method) {
    switch (method) {
        case QuadTree::VARIANCE: return "Variance";
        case QuadTree::MEAN_ABSOLUTE_DEVIATION: return "Mean Absolute Deviation";
        case QuadTree::MAX_PIXEL_DIFFERENCE: return "Max Pixel Difference";
        case QuadTree::ENTROPY: return "Entropy";
        case QuadTree::SSIM: return "Structural Similarity Index (SSIM)";
        default: return "Unknown";
    }
}

double Utils::getDefaultThreshold(QuadTree::ErrorMetricType method) {
    switch (method) {
        case QuadTree::VARIANCE: return 20.0;
        case QuadTree::MEAN_ABSOLUTE_DEVIATION: return 15.0;
        case QuadTree::MAX_PIXEL_DIFFERENCE: return 50.0;
        case QuadTree::ENTROPY: return 0.5;
        case QuadTree::SSIM: return 0.15;
        default: return 20.0;
    }
}

int Utils::getDefaultMinBlockArea() {
    return 16; // 4x4 pixels default minimum area
}


bool Utils::createDirectoryIfNotExists(const std::string& dirPath) {
    if (dirPath.empty()) {
        return true; // No directory to create
    }
    
    // Create each directory in the path
    std::string path = "";
    size_t pos = 0;
    
    // Handle absolute paths
    if (dirPath[0] == '/' || (dirPath.size() > 1 && dirPath[1] == ':')) {
        path = dirPath[0];
        if (dirPath.size() > 1 && dirPath[1] == ':') {
            path = dirPath.substr(0, 2); // Include drive letter on Windows
            pos = 2;
        } else {
            pos = 1;
        }
    }
    
    while (pos < dirPath.size()) {
        size_t nextPos = dirPath.find_first_of("/\\", pos);
        if (nextPos == std::string::npos) {
            nextPos = dirPath.size();
        }
        
        path += dirPath.substr(pos, nextPos - pos);
        
        // Try to create this directory segment if it doesn't exist
        if (!path.empty()) {
            #ifdef _WIN32
            int result = _mkdir(path.c_str());
            if (result != 0 && errno != EEXIST) {
                // Only report error if it's not because directory already exists
                if (errno != EEXIST) {
                    std::cerr << "Error creating directory " << path << std::endl;
                    return false;
                }
            }
            #else
            int result = mkdir(path.c_str(), 0755);
            if (result != 0 && errno != EEXIST) {
                std::cerr << "Error creating directory " << path << std::endl;
                return false;
            }
            #endif
        }
        
        if (nextPos < dirPath.size()) {
            path += dirPath[nextPos]; // Add the separator
        }
        pos = nextPos + 1;
    }
    
    return true;
}
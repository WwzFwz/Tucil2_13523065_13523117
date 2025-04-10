// Utils.cpp
#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include <string>
#include <sstream>

namespace fs = std::experimental::filesystem;

void Utils::getThresholdLimits(QuadTree::ErrorMetricType method, double& minThreshold, double& maxThreshold) {
    switch(method) {
        case QuadTree::VARIANCE:
            minThreshold = 0.0;
            maxThreshold = 65025.0;
            break;
        case QuadTree::MEAN_ABSOLUTE_DEVIATION:
        case QuadTree::MAX_PIXEL_DIFFERENCE:
            minThreshold = 0.0;
            maxThreshold = 255.0;
            break;
        case QuadTree::ENTROPY:
            minThreshold = 0.0;
            maxThreshold = 8.0;
            break;
        case QuadTree::SSIM:
            minThreshold = 0.0;
            maxThreshold = 1.0;
            break;
        default:
            minThreshold = 0.0;
            maxThreshold = 255.0;
    }
}
std::string Utils::getDefaultGifPath(const std::string& inputPath, 
    QuadTree::ErrorMetricType method, 
    double threshold, 
    int minBlockSize,
    double percentageCompression) {
    // Extract directory, filename, and extension
    size_t lastSlash = inputPath.find_last_of("/\\");
    std::string directory = (lastSlash != std::string::npos) ? 
    inputPath.substr(0, lastSlash + 1) : "";

    std::string filename = (lastSlash != std::string::npos) ? 
    inputPath.substr(lastSlash + 1) : inputPath;

    size_t lastDot = filename.find_last_of(".");
    std::string baseName = (lastDot != std::string::npos) ? 
    filename.substr(0, lastDot) : filename;

    // Create gif directory (could be a subdirectory of output)
    std::string gifDir;

    // If directory has "input" in the path, replace with "output/gif"
    size_t inputPos = directory.find("input");
    if (inputPos != std::string::npos) {
    gifDir = directory.substr(0, inputPos) + "output/gif/";
    } else {
    // Otherwise, just use a subfolder "output/gif" under the current directory
    gifDir = directory + "output/gif/";
    }

    // Ensure gif directory exists
    createDirectoryIfNotExists(gifDir);

    // Build new filename with .gif extension
    std::stringstream ss;
    if (percentageCompression > 0.0) {
    ss << baseName << "_percentage_" << percentageCompression;
    } else {
    ss << baseName << "_" << static_cast<int>(method) << "_" 
    << threshold << "_" << minBlockSize;
    }
    ss << ".gif";

    return gifDir + ss.str();
}

std::string Utils::getDefaultOutputPath(const std::string& inputPath, 
    QuadTree::ErrorMetricType method, 
    double threshold, 
    int minBlockSize,
    double percentageCompression) {
    // Extract directory, filename, and extension
    size_t lastSlash = inputPath.find_last_of("/\\");
    std::string directory = (lastSlash != std::string::npos) ? 
    inputPath.substr(0, lastSlash + 1) : "";

    std::string filename = (lastSlash != std::string::npos) ? 
    inputPath.substr(lastSlash + 1) : inputPath;

    size_t lastDot = filename.find_last_of(".");
    std::string baseName = (lastDot != std::string::npos) ? 
    filename.substr(0, lastDot) : filename;

    std::string extension = (lastDot != std::string::npos) ? 
    filename.substr(lastDot) : "";

    // Create path to output directory
    // Replace "input" with "output" in the directory path if possible
    std::string outputDirectory = directory;
    size_t inputPos = outputDirectory.find("input");
    if (inputPos != std::string::npos) {
    outputDirectory.replace(inputPos, 5, "output");
    } else {
    // If "input" not found, try to create an "output" directory at the same level
    size_t lastDirSlash = outputDirectory.substr(0, outputDirectory.length() - 1).find_last_of("/\\");
    if (lastDirSlash != std::string::npos) {
    outputDirectory = outputDirectory.substr(0, lastDirSlash + 1) + "output/";
    } else {
    // If all else fails, just use the same directory
    outputDirectory = directory;
    }
    }

    // Build new filename
    std::stringstream ss;
    if (percentageCompression > 0.0) {
    ss << baseName << "_percentage_" << percentageCompression;
    } else {
    ss << baseName << "_" << static_cast<int>(method) << "_" 
    << threshold << "_" << minBlockSize;
    }
    ss << extension;

    return outputDirectory + ss.str();
}
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
    return 16; 
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
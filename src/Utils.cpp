// Utils.cpp
#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


void Utils::getThresholdLimits(QuadTree::ErrorMetricType method, double& minThreshold, double& maxThreshold) {
    switch(method) {
        case QuadTree::VARIANCE:
            minThreshold = 0.0;
            maxThreshold = 1000;
            break;
        case QuadTree::MEAN_ABSOLUTE_DEVIATION:
            minThreshold = 2;
            maxThreshold = 50;
            break;
        case QuadTree::MAX_PIXEL_DIFFERENCE:
            minThreshold = 3;
            maxThreshold = 225;
            break;
        case QuadTree::ENTROPY:
            minThreshold = 0.0;
            maxThreshold = 6;
            break;
        case QuadTree::SSIM:
            minThreshold = 0.0;
            maxThreshold = 0.9;
            break;
        default:
            minThreshold = 0.0;
            maxThreshold = 1000;
    }
}
std::string Utils::getDefaultGifPath(const std::string& inputPath, 
    QuadTree::ErrorMetricType method, 
    double threshold, 
    int minBlockSize,
    double percentageCompression) {
    size_t lastSlash = inputPath.find_last_of("/\\");
    std::string directory = (lastSlash != std::string::npos) ? 
    inputPath.substr(0, lastSlash + 1) : "";

    std::string filename = (lastSlash != std::string::npos) ? 
    inputPath.substr(lastSlash + 1) : inputPath;

    size_t lastDot = filename.find_last_of(".");
    std::string baseName = (lastDot != std::string::npos) ? 
    filename.substr(0, lastDot) : filename;

    std::string gifDir;

    size_t inputPos = directory.find("input");
    if (inputPos != std::string::npos) {
    gifDir = directory.substr(0, inputPos) + "output/gif/";
    } else {
    gifDir = directory + "output/gif/";
    }

    createDirectoryIfNotExists(gifDir);

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


    std::string outputDirectory = directory;
    size_t inputPos = outputDirectory.find("input");
    if (inputPos != std::string::npos) {
    outputDirectory.replace(inputPos, 5, "output");
    } else {

    size_t lastDirSlash = outputDirectory.substr(0, outputDirectory.length() - 1).find_last_of("/\\");
    if (lastDirSlash != std::string::npos) {
    outputDirectory = outputDirectory.substr(0, lastDirSlash + 1) + "output/";
    } else {

    outputDirectory = directory;
    }
    }

    
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
        return true; 
    }

    std::string path = "";
    size_t pos = 0;
    
    if (dirPath[0] == '/' || (dirPath.size() > 1 && dirPath[1] == ':')) {
        path = dirPath[0];
        if (dirPath.size() > 1 && dirPath[1] == ':') {
            path = dirPath.substr(0, 2); 
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
        

        if (!path.empty()) {
            #ifdef _WIN32
            int result = _mkdir(path.c_str());
            if (result != 0 && errno != EEXIST) {
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
            path += dirPath[nextPos]; 
        }
        pos = nextPos + 1;
    }
    
    return true;
}
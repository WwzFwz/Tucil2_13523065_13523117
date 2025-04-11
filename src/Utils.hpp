#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <cstring>  
#include <cerrno>   
#include "QuadTree.hpp"


#ifdef _WIN32
#include <direct.h>
#define mkdir(path, mode) _mkdir(path)
#else
#include <sys/stat.h>
#include <unistd.h>
#endif

namespace Utils {

    bool fileExists(const std::string& filePath);
    size_t getFileSize(const std::string& filePath);
    std::string getFileExtension(const std::string& filePath);
    bool createDirectoryIfNotExists(const std::string& dirPath);
    
    

    std::string errorMetricToString(QuadTree::ErrorMetricType method);
    double getDefaultThreshold(QuadTree::ErrorMetricType method);
    int getDefaultMinBlockArea();
    void getThresholdLimits(QuadTree::ErrorMetricType method, double& minThreshold, double& maxThreshold);
    std::string getDefaultOutputPath(const std::string& inputPath, 
            QuadTree::ErrorMetricType method, 
            double threshold, 
            int minBlockSize,
            double percentageCompression = 0.0);
            
    std::string getDefaultGifPath(const std::string& inputPath, 
        QuadTree::ErrorMetricType method, 
        double threshold, 
        int minBlockSize,
        double percentageCompression = 0.0);
}

#endif 
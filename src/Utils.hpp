#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <cstring>  // For errno
#include <cerrno>   // For errno
#include "QuadTree.hpp"

// Platform-specific definitions
#ifdef _WIN32
#include <direct.h>
#define mkdir(path, mode) _mkdir(path)
#else
#include <sys/stat.h>
#include <unistd.h>
#endif

namespace Utils {
    // File operations
    bool fileExists(const std::string& filePath);
    size_t getFileSize(const std::string& filePath);
    std::string getFileExtension(const std::string& filePath);
    bool createDirectoryIfNotExists(const std::string& dirPath);
    
    // Path operations
    bool isAbsolutePath(const std::string& path);
    std::string toAbsolutePath(const std::string& path);
    
    // Helper functions
    std::string errorMetricToString(QuadTree::ErrorMetricType method);
    double getDefaultThreshold(QuadTree::ErrorMetricType method);
    int getDefaultMinBlockArea();
}

#endif // UTILS_HPP
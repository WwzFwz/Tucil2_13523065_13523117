#include <iostream>
#include <string>
#include <iomanip>
#include "ImageProcessor.hpp"
#include "Utils.hpp"

// ANSI color codes for terminal coloring
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"

void printColoredText(const std::string& text, const std::string& color) {
    std::cout << color << text << RESET;
}

void displayUsage(const char* programName) {
    std::cout << "Usage: " << programName << " <options>\n";
    std::cout << "Options:\n";
    std::cout << "  -i, --input <file>            Input image file path\n";
    std::cout << "  -o, --output <file>           Output compressed image file path\n";
    std::cout << "  -m, --method <number>         Error metric method (1=Variance, 2=MAD, 3=MaxDiff, 4=Entropy, 5=SSIM)\n";
    std::cout << "  -t, --threshold <number>      Error threshold\n";
    std::cout << "  -b, --blocksize <number>      Minimum block area in square pixels\n";
    std::cout << "  -c, --compression <percent>   Target compression percentage (0.0-1.0, 0 to disable)\n";
    std::cout << "  -g, --gif <file>              Output GIF visualization file path (optional)\n";
    std::cout << "  -h, --help                    Display this help message\n";
    std::cout << "\nOr run without arguments to use interactive mode.\n";
}

QuadTree::ErrorMetricType getErrorMetricFromString(const std::string& input) {
    if (input == "1") return QuadTree::VARIANCE;
    if (input == "2") return QuadTree::MEAN_ABSOLUTE_DEVIATION;
    if (input == "3") return QuadTree::MAX_PIXEL_DIFFERENCE;
    if (input == "4") return QuadTree::ENTROPY;
    if (input == "5") return QuadTree::SSIM;
    
    return QuadTree::VARIANCE; // Default
}

int main(int argc, char* argv[]) {
    std::string inputPath, outputPath, gifPath;
    QuadTree::ErrorMetricType errorMethod = QuadTree::VARIANCE;
    double threshold = -1.0; // -1 means use default based on method
    int minBlockSize = Utils::getDefaultMinBlockArea();
    double targetCompression = 0.0;
    bool interactiveMode = (argc <= 1);
    
    // Parse command line arguments if provided
    if (!interactiveMode) {
        for (int i = 1; i < argc; i++) {
            std::string arg = argv[i];
            
            if (arg == "-h" || arg == "--help") {
                displayUsage(argv[0]);
                return 0;
            } else if (arg == "-i" || arg == "--input") {
                if (i + 1 < argc) inputPath = argv[++i];
            } else if (arg == "-o" || arg == "--output") {
                if (i + 1 < argc) outputPath = argv[++i];
            } else if (arg == "-m" || arg == "--method") {
                if (i + 1 < argc) errorMethod = getErrorMetricFromString(argv[++i]);
            } else if (arg == "-t" || arg == "--threshold") {
                if (i + 1 < argc) threshold = std::stod(argv[++i]);
            } else if (arg == "-b" || arg == "--blocksize") {
                if (i + 1 < argc) minBlockSize = std::stoi(argv[++i]);
            } else if (arg == "-c" || arg == "--compression") {
                if (i + 1 < argc) targetCompression = std::stod(argv[++i]);
            } else if (arg == "-g" || arg == "--gif") {
                if (i + 1 < argc) gifPath = argv[++i];
            }
        }
        
        // Check if required arguments are provided
        if (inputPath.empty()) {
            std::cerr << "Error: Input path is required.\n";
            displayUsage(argv[0]);
            return 1;
        }
        
        // Check if input file exists
        if (!Utils::fileExists(inputPath)) {
            std::cerr << "Error: Input file '" << inputPath << "' does not exist.\n";
            return 1;
        }
        
        // Use default output path if not specified
        if (outputPath.empty()) {
            outputPath = Utils::getDefaultOutputPath(inputPath, errorMethod, 
                                                   threshold, minBlockSize, targetCompression);
            std::cout << "Using default output path: " << outputPath << std::endl;
        }
        
        // Use default gif path if not specified but target compression enabled
        if (gifPath.empty() && targetCompression > 0.0) {
            gifPath = Utils::getDefaultGifPath(inputPath, errorMethod, 
                                             threshold, minBlockSize, targetCompression);
            std::cout << "Using default GIF path: " << gifPath << std::endl;
        }
        
        // Use default threshold if not specified
        if (threshold < 0) {
            threshold = Utils::getDefaultThreshold(errorMethod);
            std::cout << "Using default threshold: " << threshold << std::endl;
        }
    } else {
        // Interactive mode with colored prompts
        printColoredText("\n=== QuadTree Image Compression ===\n\n", BLUE);
        
        // Input file
        printColoredText("Enter input image path: ", GREEN);
        std::getline(std::cin, inputPath);
        
        // Check if file exists
        if (!Utils::fileExists(inputPath)) {
            printColoredText("Error: Input file does not exist. Please check the path and try again.\n", RED);
            return 1;
        }
        
        // Output file
        printColoredText("Enter output image path (leave empty for default): ", GREEN);
        std::getline(std::cin, outputPath);

        printColoredText("\nSelect error metric method:\n", YELLOW);
        std::cout << "  1. Variance\n";
        std::cout << "  2. Mean Absolute Deviation (MAD)\n";
        std::cout << "  3. Max Pixel Difference\n";
        std::cout << "  4. Entropy\n";
        std::cout << "  5. SSIM (Structural Similarity Index)\n";
        printColoredText("Enter your choice (1-5) [default: 1]: ", GREEN);
        std::string methodStr;
        bool validMethod = false;
        do {
            std::getline(std::cin, methodStr);
            if (methodStr.empty()) {
                errorMethod = QuadTree::VARIANCE; // Default
                validMethod = true;
            } else {
                try {
                    int methodNum = std::stoi(methodStr);
                    if (methodNum >= 1 && methodNum <= 5) {
                        errorMethod = getErrorMetricFromString(methodStr);
                        validMethod = true;
                    } else {
                        printColoredText("Invalid method number. Please enter a number between 1 and 5: ", RED);
                    }
                } catch (const std::exception&) {
                    printColoredText("Invalid input. Please enter a number between 1 and 5: ", RED);
                }
            }
        } while (!validMethod);
        
        // Get default threshold and limits based on selected method
        double defaultThreshold = Utils::getDefaultThreshold(errorMethod);
        double minThreshold, maxThreshold;
        Utils::getThresholdLimits(errorMethod, minThreshold, maxThreshold);
        
        // Threshold with validation
        printColoredText("\nEnter error threshold (" + std::to_string(minThreshold) + 
                        " to " + std::to_string(maxThreshold) + 
                        ") [default: " + std::to_string(defaultThreshold) + "]: ", GREEN);
        std::string thresholdStr;
        bool validThreshold = false;
        do {
            std::getline(std::cin, thresholdStr);
            if (thresholdStr.empty()) {
                threshold = defaultThreshold;
                validThreshold = true;
            } else {
                try {
                    threshold = std::stod(thresholdStr);
                    if (threshold >= minThreshold && threshold <= maxThreshold) {
                        validThreshold = true;
                    } else {
                        printColoredText("Threshold must be between " + std::to_string(minThreshold) + 
                                        " and " + std::to_string(maxThreshold) + ": ", RED);
                    }
                } catch (const std::exception&) {
                    printColoredText("Invalid input. Please enter a valid number: ", RED);
                }
            }
        } while (!validThreshold);
        
        // Minimum block size
        printColoredText("Enter minimum block area in square pixels [default: " + 
                         std::to_string(Utils::getDefaultMinBlockArea()) + "]: ", GREEN);
        std::string blockSizeStr;
        std::getline(std::cin, blockSizeStr);
        if (!blockSizeStr.empty()) minBlockSize = std::stoi(blockSizeStr);
        
        // Target compression
        printColoredText("\nEnter target compression percentage (0.0-1.0, 0 to disable): ", GREEN);
        std::string compressionStr;
        std::getline(std::cin, compressionStr);
        if (!compressionStr.empty()) targetCompression = std::stod(compressionStr);
        
        // Set default output path if not specified
        if (outputPath.empty()) {
            outputPath = Utils::getDefaultOutputPath(inputPath, errorMethod, 
                                                   threshold, minBlockSize, targetCompression);
            printColoredText("Using default output path: " + outputPath + "\n", CYAN);
        }
        
        // GIF path
        printColoredText("Enter GIF visualization path (leave empty for default or 'n' to skip): ", GREEN);
        std::getline(std::cin, gifPath);
        
        if (gifPath.empty() && targetCompression > 0.0) {
            // Provide default GIF path for target compression
            gifPath = Utils::getDefaultGifPath(inputPath, errorMethod, 
                                             threshold, minBlockSize, targetCompression);
            printColoredText("Using default GIF path: " + gifPath + "\n", CYAN);
        } else if (gifPath == "n" || gifPath == "N") {
            // User explicitly opted out of GIF
            gifPath = "";
        }
        
        printColoredText("\nStarting compression with the following parameters:\n", CYAN);
        std::cout << "  Input: " << inputPath << "\n";
        std::cout << "  Output: " << outputPath << "\n";
        std::cout << "  Error Method: " << Utils::errorMetricToString(errorMethod) << "\n";
        std::cout << "  Threshold: " << threshold << " (range: " << minThreshold << " to " << maxThreshold << ")\n";
        std::cout << "  Min Block Area: " << minBlockSize << " square pixels\n";
        std::cout << "  Target Compression: " << std::fixed << std::setprecision(1) 
                  << (targetCompression * 100) << "%\n";
        if (!gifPath.empty()) {
            std::cout << "  GIF Output: " << gifPath << "\n";
        }
        std::cout << "\n";
    }
    
    // Process the image
    ImageProcessor processor(inputPath, outputPath, minBlockSize, threshold, 
                           errorMethod, targetCompression, gifPath);
    
    // Print progress message
    printColoredText("Loading image...\n", YELLOW);
    bool success = processor.loadImage();
    if (!success) {
        printColoredText("Failed to load the input image.\n", RED);
        return 1;
    }
    
    printColoredText("Compressing image...\n", YELLOW);
    success = processor.compressImage();
    if (!success) {
        printColoredText("Failed to compress the image.\n", RED);
        return 1;
    }
    
    printColoredText("Saving compressed image...\n", YELLOW);
    success = processor.saveCompressedImage();
    if (!success) {
        printColoredText("Failed to save the compressed image.\n", RED);
        return 1;
    }
    
    // Display compression metrics
    processor.displayMetrics();
    
    if (interactiveMode) {
        printColoredText("\nCompression completed successfully!\n", GREEN);
        std::cout << "Press Enter to exit...";
        std::cin.get();
    }
    
    return 0;
}
#ifndef __UTILS__HPP__
#define __UTILS__HPP__

#include <string>
#include "QuadTree.hpp"

using namespace std;

/**
 * @namespace Utils
 * @brief Namespace untuk fungsi-fungsi pembantu
 */
namespace Utils {
    /**
     * @brief Mengecek apakah file ada
     * @param Nama file yang akan dicek
     * @return true jika file ada, false jika tidak
     */
    bool fileExists(const string&);
    
    /**
     * @brief Mendapatkan ekstensi file
     * @param Nama file
     * @return Ekstensi file
     */
    string getFileExtension(const string&);
    
    /**
     * @brief Menghitung ukuran file
     * @param Nama file
     * @return Ukuran file dalam bytes
     */
    size_t getFileSize(const string&);
    
    /**
     * @brief Mengkonversi nama metode error menjadi string untuk display
     * @param Jenis metrik error
     * @return Nama metrik error sebagai string
     */
    string errorMetricToString(QuadTree::ErrorMetricType);
    
    /**
     * @brief Membuat direktori jika belum ada
     * @param Path direktori yang akan dibuat
     * @return true jika berhasil atau direktori sudah ada, false jika gagal
     */
    bool createDirectoryIfNotExists(const string&);
    
    /**
     * @brief Menampilkan progress bar
     * @param Progress saat ini
     * @param Total progress
     */
    void displayProgressBar(int, int);
    
    /**
     * @class CommandLineParser
     * @brief Kelas untuk parsing command line arguments
     */
    class CommandLineParser {
    private:
        int argc;
        char** argv;
        string inputFilePath;
        string outputFilePath;
        int errorMetricType;
        double threshold;
        int minBlockSize;
        double targetCompression;
        string gifOutputPath;
        
    public:
        /**
         * @brief Constructor CommandLineParser
         * @param Jumlah arguments
         * @param Array arguments
         */
        CommandLineParser(int, char**);
        
        /**
         * @brief Parse command line arguments
         * @return true jika berhasil, false jika gagal
         */
        bool parse();
        
        /**
         * @brief Menampilkan bantuan penggunaan
         */
        void displayHelp() const;

        string getInputFilePath() const;
        string getOutputFilePath() const;
        int getErrorMetricType() const;
        double getThreshold() const;
        int getMinBlockSize() const;
        double getTargetCompression() const;
        string getGifOutputPath() const;
    };
};

#endif // __UTILS__HPP__
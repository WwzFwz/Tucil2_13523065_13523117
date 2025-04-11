#ifndef __ERROR_METRICS__HPP__
#define __ERROR_METRICS__HPP__

#include <vector>
#include "RGB.hpp"
#include "Block.hpp"

using namespace std;

/**
 * @class ErrorMetrics
 * @brief Kelas untuk menghitung berbagai metrik error dalam kompresi gambar
 */
class ErrorMetrics {
    public:
        /**
         * @brief Menghitung variance RGB dalam sebuah region
         * @param Gambar yang dianalisis
         * @param Region yang dihitung variance-nya
         * @param Warna rata-rata region
         * @return Nilai variance
         */
        static double calculateVariance(const vector<vector<RGB>>&, 
                                        const Block&, 
                                        const RGB&);
        
        /**
         * @brief Menghitung Mean Absolute Deviation (MAD) dalam sebuah region
         * @param Gambar yang dianalisis
         * @param Region yang dihitung MAD-nya
         * @param Warna rata-rata region
         * @return Nilai MAD
         */
        static double calculateMAD(const vector<vector<RGB>>&, 
                                const Block&, 
                                const RGB&);
        
        /**
         * @brief Menghitung perbedaan piksel maksimum dalam sebuah region
         * @param Gambar yang dianalisis
         * @param Region yang dihitung perbedaan piksel maksimumnya
         * @param Warna rata-rata region
         * @return Nilai perbedaan maksimum
         */
        static double calculateMaxDifference(const vector<vector<RGB>>&, 
                                            const Block&, 
                                            const RGB&);
        
        /**
         * @brief Menghitung entropy dalam sebuah region
         * @param Gambar yang dianalisis
         * @param Region yang dihitung entropy-nya
         * @return Nilai entropy
         */
        static double calculateEntropy(const vector<vector<RGB>>&, 
                                    const Block&);
        
        /**
         * @brief Menghitung Structural Similarity Index (SSIM) dalam sebuah region (bonus)
         * @param Gambar yang dianalisis
         * @param Region yang dihitung SSIM-nya
         * @param Warna rata-rata region
         * @return Nilai SSIM
         */
        static double calculateSSIM(const vector<vector<RGB>>&, 
                                    const Block&, 
                                    const RGB&);
        
    private:
        /**
         * @brief Menghitung variance untuk satu channel warna
         * @param Gambar yang dianalisis
         * @param Region yang dihitung
         * @param Warna rata-rata region
         * @param Offset channel (0=R, 1=G, 2=B)
         * @return Variance untuk channel tersebut
         */
        static double calculateChannelVariance(const vector<vector<RGB>>&, 
                                            const Block&, 
                                            const RGB&, 
                                            int);
        
        /**
         * @brief Menghitung MAD untuk satu channel warna
         * @param Gambar yang dianalisis
         * @param Region yang dihitung
         * @param Warna rata-rata region
         * @param Offset channel (0=R, 1=G, 2=B)
         * @return MAD untuk channel tersebut
         */
        static double calculateChannelMAD(const vector<vector<RGB>>&, 
                                        const Block&, 
                                        const RGB&, 
                                        int);
        
        /**
         * @brief Menghitung perbedaan maksimum untuk satu channel warna
         * @param Gambar yang dianalisis
         * @param Region yang dihitung
         * @param Warna rata-rata region
         * @param Offset channel (0=R, 1=G, 2=B)
         * @return Perbedaan maksimum untuk channel tersebut
         */
        static double calculateChannelMaxDifference(const vector<vector<RGB>>&, 
                                                const Block&, 
                                                const RGB&, 
                                                int);
        
        /**
         * @brief Menghitung entropy untuk satu channel warna
         * @param Gambar yang dianalisis
         * @param Region yang dihitung
         * @param Offset channel (0=R, 1=G, 2=B)
         * @return Entropy untuk channel tersebut
         */
        static double calculateChannelEntropy(const vector<vector<RGB>>&, 
                                            const Block&, 
                                            int);
        
        /**
         * @brief Menghitung SSIM untuk satu channel warna (bonus)
         * @param Gambar yang dianalisis
         * @param Region yang dihitung
         * @param Warna rata-rata region
         * @param Offset channel (0=R, 1=G, 2=B)
         * @return SSIM untuk channel tersebut
         */
        static double calculateChannelSSIM(const vector<vector<RGB>>&, 
                                        const Block&, 
                                        const RGB&, 
                                        int);
};

#endif 
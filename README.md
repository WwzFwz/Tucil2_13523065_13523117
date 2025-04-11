<h1 align="center">Tugas Kecil 2 IF2211 Strategi Algoritma</h1>
<h3 align="center">Kompresi Gambar dengan Metode Quadtree</h3>


## Table of Contents

- [Description](#description)
- [Features](#features)
- [Tech Stack](#tech-stack)
- [Structure](#structure)
- [Getting Started](#getting-started)
- [Contributors](#contributors)
- [References](#references)

## Description
Program sederhana yang mengimplementasikan algoritma kompresi gambar dengan metode QuadTree berbasis pendekatan divide and conquer.

## Features
-**Kompresi gambar berbasis QuadTree dengan metode divide and conquer**
-**Pilihan 5 metode pengukuran error yang berbeda**
-**Mode kompresi adaptif dengan target persentase kompresi **
-**Visualisasi proses kompresi dalam format GIF**
-**Input melalui mode CLI atau mode command args"

## Requirements
- **C++ Compiler (g++) with C++14 support**
- **Make (opstional)**
- **Operating System: Windows/Linux/MacOS**

## Structure
```
quadtree_compression/
├── bin/                    # Executable files
├── test/                   # Test files
│   ├── input/              # Input images
│   └── output/             # Compressed output images and GIFs
├── src/                    # Source code
│   ├── include/            # Header files
│   ├── main.cpp            # Main program
│   ├── Block.cpp           # Block class implementation
│   ├── Node.cpp            # QuadTree node implementation
│   ├── RGB.cpp             # RGB color class
│   ├── QuadTree.cpp        # QuadTree implementation
│   ├── ErrorMetrics.cpp    # Error calculation methods
│   ├── ImageProcessor.cpp  # Image processing functionality
│   └── Utils.cpp           # Utility functions
├── lib/                    # External libraries
│   ├── stb_image.h         # Image loading library
│   ├── stb_image_write.h   # Image writing library
│   └── gif.h               # GIF creation library
├── makefile                # Compilation instructions
└── README.md               # Documentation
```
- src : contains the main program's source code files.
- doc : contains the assignment report.
- test : contains datasets and query fir testing.

## Getting Started
1. **Clone this repository: or download latest release** 
   ```bash
   git clone https://github.com/WwzFwz/Tucil2_13523065_13523117.git
   ```
2. **Navigate to the src directory of the program by running the following command in the terminal:**
   ```bash
   cd Tucil2_13523065_13523117
   ```

### How to Compile
After opening the terminal on the project 
## Alternative 1 - Running the existing executable file**
   ```bash
      ./bin/quadtree_compression
   ```
## Alternative 2 - Recompiling with make 
   ```bash
   make
   ```
   then you can run the new executable file as in Alternative 1

## Alternative 3 - Running and recompiling with make
   ```bash
   make run
   ```
   note : alternative 3 using **interactive mode** (CLI input)
   
## Alternative 4 - Recompiling with g++ in terminal 
   ```bash
   g++ -std=c++14 -Wall -Wextra -I./src/include -I./lib  src/main.cpp src/Block.cpp src/Node.cpp src/RGB.cpp   src/QuadTree.cpp src/ErrorMetrics.cpp src/ImageProcessor.cpp src/Utils.cpp  -o quadtree_compression
   ```
   then you can run the new executable file as in Alternative 1
   
### How to Run
**The program can be run in two modes:**
-***Interactive Mode***: Without command line parameters
-***Command Line Mode***: With command line parameters
## A. INTERACTIVE MODE (CLI)
   Run the program without parameters :
   ```bash
      ./bin/quadtree_compression
   ```
## B. COMMAND LINE MODE 
   ```bash
   ./bin/quadtree_compression [options]
   ```
###**Available optionns** :
Available options:
**-i, --input <file>**: Input image file path (REQUIRED)
**-o, --output <file>**: Output compressed image file path (if omitted, a default path will be generated)
**-m, --method <number>**: Error measurement method (1=Variance, 2=MAD, 3=MaxDiff, 4=Entropy, 5=SSIM) (default: 1)
**-t, --threshold <number>**: Error threshold (if omitted, a default value based on the method will be used)
**-b, --blocksize <number>**: Minimum block size in square pixels (default: 16)
**-c, --compression <percent>**: Target compression percentage (0.0-1.0, 0 to disable) (default: 0)
**-g, --gif <file>**: GIF visualization file path (if omitted, no GIF will be generated unless target compression is enabled)
**-h, --help**: Display help information

###**Default Behavior for Omitted Parameters**

If **output is not specified**, a **default path** will be generated based on the input filename and compression parameters
If **method is not specified, Variance (1)** will be used as the default method
If **threshold is not specified**, a **default value** will be used based on the selected method
If **blocksize is not specified**, **16 square pixels** will be used as the default
If **compression is not specified, adaptive compression mode** will be disabled (0.0)
If **gif is not specified** but **compression is set**, a **default GIF path will be generated**
## Possible Error

## Contributors

| **NIM**  | **Nama Anggota**               | **Github** |
| -------- | ------------------------------ | ---------- |
| 13523065 | Dzaky Aurelia Fawwaz           | [WwzFwz](https://github.com/WwzFwz) |
| 13523117 | Ferdin Arsenarendra Purtadi    | [Ferdin-Arsenic](https://github.com/Ferdin-Arsenic) |

## References
- [Slide Kuliah 1 IF2211 2024/2025](https://informatika.stei.itb.ac.id/~rinaldi.munir/Stmik/2024-2025/07-Algoritma-Divide-and-Conquer-(2025)-Bagian1.pdf)
- [Slide Kuliah 2 IF2211 2024/2025](https://informatika.stei.itb.ac.id/~rinaldi.munir/Stmik/2024-2025/08-Algoritma-Divide-and-Conquer-(2025)-Bagian2.pdf)
- [Slide Kuliah 3 IF2211 2024/2025](https://informatika.stei.itb.ac.id/~rinaldi.munir/Stmik/2024-2025/09-Algoritma-Divide-and-Conquer-(2025)-Bagian3.pdf)
- [Slide Kuliah 4 IF2211 2024/2025](https://informatika.stei.itb.ac.id/~rinaldi.munir/Stmik/2024-2025/10-Algoritma-Divide-and-Conquer-(2025)-Bagian4.pdf)
- [Spesifikasi Tugas Besar](https://informatika.stei.itb.ac.id/~rinaldi.munir/Stmik/2024-2025/Tucil2-Stima-2025.pdf)

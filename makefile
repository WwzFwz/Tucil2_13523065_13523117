CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -I./src/include
SRC_DIR = src
BIN_DIR = bin
OBJ_DIR = obj

# Daftar file source
SOURCES = $(SRC_DIR)/*.cpp \
          $(SRC_DIR)/*.cpp \
          $(SRC_DIR)/*.cpp \
          $(SRC_DIR)/*.cpp \
          $(SRC_DIR)/*.cpp

# Membuat daftar file objek
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Target executable
TARGET = $(BIN_DIR)/quadtree_compression

# Default target
all: directories $(TARGET)

# Membuat direktori yang diperlukan
directories:
	mkdir -p $(BIN_DIR) $(OBJ_DIR)

# Membuat executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile source files ke object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Membersihkan hasil build
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Clean dan build ulang
rebuild: clean all

# Menjalankan program
run: all
	$(TARGET)

.PHONY: all directories clean rebuild run
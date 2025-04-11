CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -I./src/include -I./lib
# LDFLAGS = -lstdc++fs
SRC_DIR = src
BIN_DIR = bin

all:
	mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/quadtree_compression \
	$(SRC_DIR)/main.cpp \
	$(SRC_DIR)/Block.cpp \
	$(SRC_DIR)/Node.cpp \
	$(SRC_DIR)/RGB.cpp \
	$(SRC_DIR)/QuadTree.cpp \
	$(SRC_DIR)/ErrorMetrics.cpp \
	$(SRC_DIR)/ImageProcessor.cpp \
	$(SRC_DIR)/Utils.cpp \


run: all
	./$(BIN_DIR)/quadtree_compression

clean:
	rm -f $(BIN_DIR)/quadtree_compression

.PHONY: all clean run
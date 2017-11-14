#pragma once
#include <algorithm>

struct __attribute__ ((packed)) Pixel {
	uint8_t r, g, b;
};

class Display {
public:
	constexpr static int ROWS = 8;
	constexpr static int COLS = 8;

	Display() {
		pixels = new Pixel[ROWS * COLS];
	}

	~Display() {
		if(pixels) {
			delete[] pixels;
		}
	}

	void clear() {
		memset(pixels, 0, ROWS * COLS * sizeof(Pixel));
	}

	constexpr int rows() {
		return ROWS;
	}

	constexpr int cols() {
		return COLS;
	}

	Pixel& begin() {
		return *pixels;
	}

	Pixel& end() {
		return pixels[ROWS * COLS];
	}

	int size() {
		return ROWS * COLS * 3;
	}

	Pixel& at(int row, int col) {
		return pixels[col + row * COLS];
	}

	void rev() {
		for(int row = 0; row < ROWS / 2; row++) {
			for(int col = 0; col < COLS; col++) {
				std::swap(at(row, col), at(ROWS - row - 1, col));
			}
		}
	}

	void revCols() {
		for(int row = 0; row < ROWS; row++) {
			for(int col = 0; col < COLS / 2; col++) {
				std::swap(at(row, col), at(row, COLS - col - 1));
			}
		}
	}

	void transpose() {
		for(int row = 0; row < ROWS; row++) {
			for(int col = 0; col < row; col++) {
				std::swap(at(row, col), at(col, row));
			}
		}
	}

	void rotate90() {
		rev();
		transpose();
	}

	void rotate(int count) {
		count = count % 4;
		if(count == 1) {
			rotate90();
		} else if(count == 2) {
			rotate90();
			rotate90();
		} else if(count == 3) {
			transpose();
			rev();
		}
	}
private:
	Pixel *pixels;
};
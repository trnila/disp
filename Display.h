#pragma once
#include <algorithm>

struct Pixel {
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

	Pixel& at(int row, int col) {
		return pixels[col + row * COLS];
	}

	void rev() {
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j < 8; j++) {
				std::swap(at(i, j), at(ROWS - i, j));
			}
		}
	}
/*
	void revCols() {
		for(int i = 0; i < 8; i++) {
			for(int j = 0; j < 4; j++) {
				swap(&pixels[1+i+(j*8*3)+RED  ], &pixels[1+i+((7-j)*8*3)+RED  ]);
				swap(&pixels[1+i+(j*8*3)+GREEN  ], &pixels[1+i+((7-j)*8*3)+GREEN  ]);
				swap(&pixels[1+i+(j*8*3)+BLUE  ], &pixels[1+i+((7-j)*8*3)+BLUE  ]);
			}
		}
	}

	void transpose() {
		for(int i = 0; i < 8; i++) {
			for(int j = 0; j < i; j++) {
				swap(&pixels[1+i+(j*8*3)+RED  ], &pixels[1+j+(i*8*3)+RED  ]);
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
*/
private:
	Pixel *pixels;
};
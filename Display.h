#pragma once

struct Pixel {
	uint8_t r, g, b;
};

class Display {
public:
	const int ROWS = 8;
	const int COLS = 8;
	const int COLORS = 3;

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

	constexpr int size() {
		return ROWS * COLS * COLORS + 1;
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

	void render() {
		for (int i = 0 ; i<8; i++) {
			for(int j = 0 ; j<8; j++){
				int set = at(i, j).r != 0;
				printf(set ? "x" : " ");
			}
			printf("\n");
		}
	}

	void i2crender() {

	}

	Pixel& at(int row, int col) {
		return pixels[col + row * COLS];
	}

private:
	Pixel *pixels;
};
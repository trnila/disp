#pragma once

class Font {
public:
	Font(unsigned char (*font)[8]): font(font) {
		calculateLetterSizes();
		this->sizes[' '] = 8;
		this->starts[' '] = 1;
	}

	int sizes[255];
	int starts[255];
	unsigned char (*font)[8];

private:
	void calculateLetterSizes() {
		for(int i = 0; i < 255; i++) {
			int start = 0;
			int stop = 7;

			unsigned char *bitmap = font[i];
			for(int j = 0; j < 8; j++) {
				for(int row = 0; row < 8; row++) {
					if(bitmap[row] & 1 << (j)) {
						if(start == 0) {
							start = j;
						}
						stop = j;
					}
				}
			}
			this->sizes[i] = stop + 2;
			this->starts[i] = start;
		}
	};
};
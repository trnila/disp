
#pragma once


typedef struct {
	char r;
	char g;
	char b;
} Color;

Color randColor() {
	char r = 5;
	char g = 5;
	char b = 5;

	if((random() % 3) == 0) r += 50;
	if((random() % 3) == 1) g += 50;
	if((random() % 3) == 2) b += 50;

	Color c;
	c.r = r;
	c.g = g;
	c.b = b;
	return c;
}

typedef struct {
	const char* text;
	int len;
	Color* colors;
	unsigned char (*font)[8];
} Input;

void calculateLetterSizes();

void newInput(Input *input, const char* text) {
	input->text = text;
	input->len = strlen(text);

	input->colors = (Color*) malloc(sizeof(Color) * input->len);
	for(int i = 0; i < input->len; i++) {
		input->colors[i] = randColor();
	}
}


class BitsAlgo {
public:
	BitsAlgo(Input *input): input(input) {
		currentLetter = 0;
		this->input = input;
		this->shiftInLetter = 0;

		calculateLetterSizes();
		this->sizes[' '] = 8;
		this->starts[' '] = 1;
	}

	void next(Display &display) {
		const char *text = this->input->text;
		unsigned char *bitmap = input->font[text[currentLetter]];

		int rendL = currentLetter; // current rendered letter
		int pos = this->shiftInLetter; // shiftInLetter in rendered letter
		for(int row = 0; row < Display::ROWS; row++){
			for (int col = 0; col < Display::COLS; col++) {
				if(pos >= this->sizes[text[rendL]]) {
					rendL = (rendL + 1) % input->len;
					bitmap = input->font[text[rendL]];
					pos = this->starts[text[currentLetter]] - 1;
				}

				int set = bitmap[col] & 1 << (pos);
				if(set) {
					display.at(col, row).r = this->input->colors[rendL].r;
					display.at(col, row).g = this->input->colors[rendL].g;
					display.at(col, row).b = this->input->colors[rendL].b;
				}
			}
			pos++;
		}

		this->shiftInLetter++;
		if(this->shiftInLetter >= this->sizes[text[currentLetter]] + 1) {
			this->currentLetter = (currentLetter + 1) % input->len;
			this->shiftInLetter = this->starts[text[this->currentLetter]];
		}
	}

private:
	int sizes[255];
	int starts[255];
	int currentLetter;
	int shiftInLetter;
	Input* input;

	void calculateLetterSizes() {
		for(int i = 0; i < 255; i++) {
			int start = 0;
			int stop = 7;

			unsigned char *bitmap = input->font[i];
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
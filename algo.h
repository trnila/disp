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
	Font* font;
} Input;


void newInput(Input *input, const char* text, Font *font) {
	input->text = text;
	input->len = strlen(text);
	input->font = font;

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
	}

	void next(Display &display) {
		const char *text = this->input->text;
		unsigned char *bitmap = input->font->font[text[currentLetter]];

		int rendL = currentLetter; // current rendered letter
		int pos = this->shiftInLetter; // shiftInLetter in rendered letter
		for(int row = 0; row < Display::ROWS; row++){
			for (int col = 0; col < Display::COLS; col++) {
				if(pos >= input->font->sizes[text[rendL]]) {
					rendL = (rendL + 1) % input->len;
					bitmap = input->font->font[text[rendL]];
					pos = input->font->starts[text[currentLetter]] - 1;
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
		if(this->shiftInLetter >= input->font->sizes[text[currentLetter]] + 1) {
			this->currentLetter = (currentLetter + 1) % input->len;
			this->shiftInLetter = input->font->starts[text[this->currentLetter]];
		}
	}

private:
	int currentLetter;
	int shiftInLetter;
	Input* input;
};
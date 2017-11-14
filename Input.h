#pragma once

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
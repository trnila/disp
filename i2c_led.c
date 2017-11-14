#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include "font8x8-master/font8x8.h"
#include "Display.h"
#include "OstreamRenderer.h"
#include <iostream>
#include "I2CRenderer.h"

#define RED 0
#define GREEN 8
#define BLUE  16  

typedef struct {
	char r;
	char g;
	char b;
} Color;

int swap(uint8_t *a, uint8_t *b) {
	int s = *a;
	*a = *b;
	*b = s;
}
/*
void rev() {
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 8; j++) {
			swap(&pixels[1+i+(j*8*3)+RED  ], &pixels[1+(7 - i)+(j*8*3)+RED  ]);
			swap(&pixels[1+i+(j*8*3)+GREEN  ], &pixels[1+(7 - i)+(j*8*3)+GREEN  ]);
			swap(&pixels[1+i+(j*8*3)+BLUE  ], &pixels[1+(7 - i)+(j*8*3)+BLUE  ]);
		}
	}
}

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
} Input;

typedef struct {
	int sizes[255];
	int starts[255];
	int l;
	int shift;
	Input* input;

} Mem;

void newInput(Input *input, const char* text) {
	input->text = text;
	input->len = strlen(text);

	input->colors = (Color*) malloc(sizeof(Color) * input->len);
	for(int i = 0; i < input->len; i++) {
		input->colors[i] = randColor();
	}
}

void init(Mem* impl, Input* input) {
	for(int i = 0; i < 255; i++) {
		int start = 0;
		int stop = 7;

		unsigned char *bitmap = font8x8_basic[i];
		for(int j = 0; j < 8; j++) {
			for(int row = 0; row < 8; row++) {
				int set = bitmap[row] & 1 << (j);
				if(set) {
					if(start == 0) {
						start = j;
					}
					stop = j;
				}
			}
		}
		impl->sizes[i] = stop + 2;
		impl->starts[i] = start;
	}
	impl->sizes[' '] = 8;
	impl->starts[' '] = 1;

	impl->l = 0;
	impl->input = input;
	impl->shift = 0;
}


void next(Mem *impl, Display &display) {
	const char *text = impl->input->text;
	int l = impl->l;
	int len = strlen(text);


	int rendL = l; 
	unsigned char *bitmap = font8x8_basic[text[l]];
	int pos = impl->shift;
	for(int j = 0; j<8; j++){ //radky
		for (int i = 0 ; i<8; i++) {
			if(pos >= impl->sizes[text[rendL]]) {
				rendL = (rendL + 1) % len;
				bitmap = font8x8_basic[text[rendL]];
				pos = impl->starts[text[l]] - 1;
			}
			int set = bitmap[i] & 1 << (pos);
			if(set) {
				display.at(i, j).r = impl->input->colors[rendL].r;
				//pixels[1+i+((j)*8*3)+RED  ] = impl->input->colors[rendL].r;
				//pixels[1+i+((j)*8*3)+GREEN  ] = impl->input->colors[rendL].g;
				//pixels[1+i+((j)*8*3)+BLUE  ] = impl->input->colors[rendL].b;
			}
		}
		pos++;
	}

	impl->shift++;

	if(impl->shift >= impl->sizes[text[l]] + 1) {
		impl->l = (l + 1) % len;
		impl->shift = impl->starts[text[impl->l]]; 
	}
}


int main(int argc, char **argv) {
	if(argc != 2) {
		fprintf(stderr, "Usage: %s text\n", argv[0]);
		exit(1);
	}
	char *text = argv[1];
	int len = strlen(text);



	srand(time(NULL));


	Input input;
	newInput(&input, text);

	Mem mem;
	init(&mem, &input);

	Display display;
//	I2CRenderer rendererA("/dev/i2c-2");
	OstreamRenderer renderer(std::cout);

	for(;;) {
		display.clear();
		next(&mem, display);
		renderer.render(display);

		usleep(50000);
	}
}


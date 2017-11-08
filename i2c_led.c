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

#define RED 0
#define GREEN 8
#define BLUE  16  

typedef struct {
	char r;
	char g;
	char b;
} Color;

uint8_t pixels[] = {
	0x00,
	0x1F, 0x1F, 0x1F, 0x1F, 0x14, 0x03, 0x00, 0x00,
	0x00, 0x00, 0x03, 0x12, 0x1F, 0x1F, 0x1F, 0x1F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07,

	0x1F, 0x1F, 0x1F, 0x12, 0x03, 0x00, 0x00, 0x00,
	0x00, 0x04, 0x14, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x1D,

	0x1F, 0x1F, 0x11, 0x02, 0x00, 0x00, 0x00, 0x00,
	0x05, 0x15, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x0B,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x1F, 0x1F,

	0x1F, 0x0F, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x17, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x0A, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x0A, 0x1F, 0x1F, 0x1F,

	0x0E, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03,
	0x1F, 0x1F, 0x1F, 0x1F, 0x1D, 0x08, 0x00, 0x00,
	0x00, 0x00, 0x01, 0x0B, 0x1F, 0x1F, 0x1F, 0x1F,

	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x14,
	0x1F, 0x1F, 0x1F, 0x1B, 0x07, 0x00, 0x00, 0x00,
	0x00, 0x01, 0x0C, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F,

	0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x15, 0x1F,
	0x1F, 0x1F, 0x19, 0x06, 0x00, 0x00, 0x00, 0x00,
	0x02, 0x0E, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x12,

	0x00, 0x00, 0x00, 0x00, 0x05, 0x17, 0x1F, 0x1F,
	0x1F, 0x17, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x0F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x0F, 0x02,
};

uint8_t array[192];

int swap(uint8_t *a, uint8_t *b) {
	int s = *a;
	*a = *b;
	*b = s;
}

void display() {
	for (int i = 0 ; i<8; i++) {
		for(int j = 0 ; j<8; j++){
			int set = pixels[1+i+(j*8*3)+RED  ] != 0;
			printf(set ? "x" : " ");
		}
		printf("\n");
	}
}

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

int main(int argc, char **argv) {
	if(argc != 2) {
		fprintf(stderr, "Usage: %s text\n", argv[0]);
		exit(1);
	}
	char *text = argv[1];
	int len = strlen(text);

	int file;
	const char *filename = "/dev/i2c-2";
	if ((file = open(filename, O_RDWR)) < 0) {
		perror("Failed to open the i2c bus");
		exit(1);
	}

	int addr = 0x46;          // The I2C address
	if (ioctl(file, I2C_SLAVE, addr) < 0) {
		perror("Failed to acquire bus access and/or talk to slave.\n");
		exit(1);
	}

	srand(time(NULL));

	Color *colors = (Color*) malloc(sizeof(Color) * len);
	for(int i = 0; i < len; i++) {
		colors[i] = randColor();
	}

	int l  = 0;
	for(;;) {
		for(int pos = 0; pos < 8; pos++) {
			char *bitmap = font8x8_basic[text[l]];

			// clear matrix
			memset(pixels, 0, sizeof(pixels));

			for (int i = 0 ; i<8; i++) {
				for(int j = pos ; j<8; j++){
					int set = bitmap[i] & 1 << (j);
					if(set){
						pixels[1+i+((j - pos)*8*3)+RED  ] = colors[l].r;
						pixels[1+i+((j - pos)*8*3)+GREEN  ] = colors[l].g;
						pixels[1+i+((j - pos)*8*3)+BLUE  ] = colors[l].b;
					}
				}
			}

			int nextL = (l + 1) % len;
			bitmap = font8x8_basic[text[nextL]];
			for (int i = 0 ; i<8; i++) {
				for(int j = 0; j< pos; j++){
					int set = bitmap[i] & 1 << (j);
					if(set){
						if(pos > 0) {
							pixels[1+i+(((8-pos)+j)*8*3)+RED  ] = colors[nextL].r;
							pixels[1+i+(((8-pos)+j)*8*3)+GREEN  ] = colors[nextL].g;
							pixels[1+i+(((8-pos)+j)*8*3)+BLUE  ] = colors[nextL].b;
						}
					}
				}
			}

//			revCols();
//			rev()
//			rotate();
			rev();			
			display();

			if (write(file,pixels,sizeof(pixels)) != sizeof(pixels) ) {
				printf("Failed to write to the i2c bus.\n");
				exit(1);
			}

			usleep(100000);
		}
		l = (l + 1) % len;
	}

	close(file);
	free(colors);
}


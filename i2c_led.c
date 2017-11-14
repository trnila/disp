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
#include "Display.h"
#include "OstreamRenderer.h"
#include <iostream>
#include "font8x8-master/font8x8_basic.h"
//#include "I2CRenderer.h"
#include "algo.h"


int main(int argc, char **argv) {
	if(argc != 2) {
		fprintf(stderr, "Usage: %s text\n", argv[0]);
		exit(1);
	}
	const char *text = argv[1];
	srand(time(NULL));

	Input input;
	newInput(&input, text);
	input.font = font8x8_basic;

	Display display;
//	I2CRenderer rendererA("/dev/i2c-2");
	OstreamRenderer renderer(std::cout);
	BitsAlgo algo(&input);

	for(;;) {
		display.clear();
		algo.next(display);
		renderer.render(display);

		usleep(50000);
	}
}

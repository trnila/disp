#include <time.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include "font8x8-master/font8x8_basic.h"
#include "Display.h"
#include "OstreamRenderer.h"
#include "I2CRenderer.h"
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
	I2CRenderer rendererA("/dev/i2c-2");
	OstreamRenderer renderer(std::cout);
	BitsAlgo algo(&input);

	for(;;) {
		display.clear();
		algo.next(display);
		//display.rev();
		//display.revCols();
		//display.rotate(1);
		renderer.render(display);

		usleep(50000);
	}
}

#include <time.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include "font8x8-master/font8x8_basic.h"
#include "Display.h"
#include "OstreamRenderer.h"
#include "I2CRenderer.h"
#include "Font.h"
#include "algo.h"

/*
template<typename A, typename B>
struct Renderer {
	Renderer(A a, B b): a(a), b(b) {}

	void operator()(Display &display) {
		a.render(display);
		b.render(display);
	}

	A a;
	B b;
};*/

template<typename A, typename... B>
struct Renderer {
	Renderer(A a, B... b): a(a), b(Renderer<B...>(b...)) {}

	void operator()(Display &display) {
		a.render(display);
		b.render(display);
	}

	void render(Display &display) {
		b.render(display);
	}

	A a;
	Renderer<B...> b;
};

template<>
struct Renderer<I2CRenderer> {
	Renderer(I2CRenderer a): a(a) {

	}

	void operator()(Display &display) {
		a.render(display);
	}

	void render(Display &display) {
		a.render(display);
	}

	I2CRenderer a;
};

int main(int argc, char **argv) {
	if(argc != 2) {
		fprintf(stderr, "Usage: %s text\n", argv[0]);
		exit(1);
	}
	const char *text = argv[1];
	srand(time(NULL));

	Font font(font8x8_basic);

	Input input;
	newInput(&input, text, &font);

	Display display;
	I2CRenderer rendererA("/dev/i2c-2");
	OstreamRenderer renderer(std::cout);
	BitsAlgo algo(&input);

	Renderer r(OstreamRenderer(std::cout), OstreamRenderer(std::cout), I2CRenderer("/dev/i2c-2"));

	for(;;) {
		display.clear();
		algo.next(display);

		//display.rev();
		//display.revCols();
		//display.rotate(1);
		//renderer.render(display);
		r(display);

		usleep(50000);
	}
}

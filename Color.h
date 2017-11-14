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
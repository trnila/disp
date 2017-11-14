#pragma once
#include <ostream>

class OstreamRenderer {
public:
	OstreamRenderer(std::ostream &out): out(out) {}

	void render(Display &display) {
		std::cout << "ostream\n";
		for(int row = 0; row < display.rows(); row++) {
			for (int col = 0; col < display.cols(); ++col) {
				int set = display.at(row, col).r != 0;
				out << (set ? "x" : " ");
			}
			out << "\n";
		}
	}

private:
	std::ostream &out;

};
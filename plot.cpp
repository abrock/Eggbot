#include <iostream>
#include <stdint.h>
#include <fstream>
#include <cmath>
#include <png++/png.hpp>

using namespace std;

int main(int argc, char ** argv) {
	if (argc < 3) {
		cout	<< "This program converts textfiles into images. The image shows a print preview for the eggbot." << endl
					<< "Usage: " << argv[0] << " <data file> <output file (png)>" << endl;
		return 0;
	}
	fstream in;
	in.open(argv[1], fstream::in);

	int64_t max_x = 0, min_x = 0, max_y = 0, min_y = 0;
	// move the pen
	int64_t position_x = 0;
	// Rotate the egg
	int64_t position_y = 0;


	while (in) {
		char command;
		in >> command;
		switch(command) {
			case 'l': position_x--; break;
			case 'r': position_x++; break;
			case 'u': position_y--; break;
			case 'd': position_y++; break;
			case '1': position_x--; position_y--; break;
			case '2': position_x++; position_y--; break;
			case '3': position_x--; position_y++; break;
			case '4': position_x++; position_y++; break;
		}
		max_x = max(max_x, position_x);
		min_x = min(min_x, position_x);
		max_y = max(max_y, position_y);
		min_y = min(min_y, position_y);
	}

	in.close();

	size_t height = max_y - min_y + 4;
	size_t width  = max_x - min_x + 4;
	
	cout	<< "min_x:\t" << min_x << endl
				<< "max_x:\t" << max_x << endl
				<< "min_y:\t" << min_y << endl
				<< "max_y:\t" << max_y << endl
				<< "width:\t" << width << endl
				<< "height:\t" << height << endl;

	in.open(argv[1], fstream::in);
	
 png::image< png::rgb_pixel > image(width, height);
 for (size_t y = 0; y < image.get_height(); ++y)
 {
     for (size_t x = 0; x < image.get_width(); ++x)
     {
         image[y][x] = png::rgb_pixel(255,255,255);
         // non-checking equivalent of image.set_pixel(x, y, ...);
     }
 }

	position_x = -min_x + 2;
	position_y = -min_y + 2;
	while (in) {
		char command;
		in >> command;
		switch(command) {
			case 'l': position_x--; break;
			case 'r': position_x++; break;
			case 'u': position_y--; break;
			case 'd': position_y++; break;
			case '1': position_x--; position_y--; break;
			case '2': position_x++; position_y--; break;
			case '3': position_x--; position_y++; break;
			case '4': position_x++; position_y++; break;
		}
		image.set_pixel(position_x, position_y, png::rgb_pixel(0,0,0));
	}

 image.write(argv[2]);

	return 0;
}


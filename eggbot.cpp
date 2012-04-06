#include <iostream>
#include <fstream>
#include <SerialStream.h>
#include <sstream>
#include <stdio.h>
#include <sys/time.h>
#include <cmath>
#include <vector>
#include <map>
#include <algorithm>
#include <cstdlib>

using namespace std;
using namespace LibSerial;

#include "LibSerialHelper.h"

template<class T>
class EggBot {
public:

bool open_device(const char * device, int baudrate) {
	LibSerialHelper helper;
  helper.Open(usb0, device, baudrate);
	return true;
}

	void start() {
		while(true) {
			putc('d');
		}
	}

	void putc(char c) {
		usb0.write(&c,1);
	}

private:
SerialStream usb0;
};

int main(int argc, char* argv[]) {

	EggBot<float> bot;

	if (argc < 3) {
		cerr << "Usage: " << argv[0] << " device baudrate" << endl;
		return 0;
	}

	int baudrate = 9600;
	{
		stringstream tmp(argv[2]);
		tmp >> baudrate;
	}

	bot.open_device(argv[1], baudrate);
	bot.start();


	return 0;
}

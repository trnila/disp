#pragma once
#include <stdexcept>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <unistd.h>

class I2CRenderer {
public:
	I2CRenderer(const char* i2cDev) {
		if((fd = open(i2cDev, O_RDWR)) < 0) {
		//	throw std::runtime_error("Failed to open the i2c bus");
		}

		const int addr = 0x46;
		if(ioctl(fd, I2C_SLAVE, addr) < 0) {
		//	throw std::runtime_error("Failed to acquire bus access and/or talk to slave.\n");
		}
	}

	~I2CRenderer() {
		close(fd);
	}

	void render(Display &display) {
		int byte = 0x00;
		if(write(fd, &byte, sizeof(byte)) != sizeof(byte)) {
			//throw std::runtime_error("Failed to write to the i2c bus.\n");
		}

		const int size = display.size() * sizeof(uint8_t);
		if (write(fd, &display.begin(), size) != size) {
			//throw std::runtime_error("Failed to write to the i2c bus.\n");
		}
	}

private:
	int fd;
};

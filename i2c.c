#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void i2c_read(int bus, int addr, int reg)
{
	int file;		/* fd */
	char filename[40];

	sprintf(filename,"/dev/i2c-%d", bus);
	if ((file = open(filename, O_RDWR)) < 0) {
		printf("Failed to open the bus.\n");
		/* ERROR HANDLING; you can check errno to see what went wrong */
		exit(1);
	}

	if (ioctl(file, I2C_SLAVE, addr) < 0) {
		printf("Failed to acquire bus access and/or talk to slave.\n");
		/* ERROR HANDLING; you can check errno to see what went wrong */
		exit(1);
	}

	char buf[10] = {0};
	char channel;

	int i;
	for(i = 0; i<4; i++) {
		// Using I2C Read
		if (read(file, buf, 2) != 2) {
			/* ERROR HANDLING: i2c transaction failed */
			printf("Failed to read from the i2c bus.\n");
			printf("Error: %d\n", errno);
		} else {
			int j;
			for (j = 0; j<10; j++)
			{
				printf("%d: %d", j, buf[j]);
			}
		}
	}
}

int i2c_write(void)
{

	//unsigned char reg = 0x10; // Device register to access
	//buf[0] = reg;
	char buf[10] = {0};
	int file;		/* fd */
	char filename[40];
	buf[0] = 0b11110000;

	if (write(file, buf, 1) != 1) {
		/* ERROR HANDLING: i2c transaction failed */
		printf("Failed to write to the i2c bus.\n");
		printf("Error: %d\n", errno);
	}
}

int help(void)
{
	printf("Not enough arguments\n");
}

int main(int argc, char *argv[])
{
	if (argc < 1) {
		help();
		exit(1);
	}

	i2c_read(4, 0b11111110, 0);
}


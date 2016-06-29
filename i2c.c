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

int i2c_read(int bus, int addr, int reg)
{
	int file;		/* fd */
	char filename[40];
	int ret;

	sprintf(filename,"/dev/i2c-%d", bus);
	if ((file = open(filename, O_RDWR)) < 0) {
		return errno;
	}

	if (ioctl(file, I2C_SLAVE, addr) < 0) {
		return errno;
	}

	char buf[10] = {0};
	char channel;

	int i;
	for(i = 0; i < 4; i++) {
		if (read(file, buf, 2) != 2) {
			return errno;
		} else {
			int j;
			for (j = 0; j < 10; j++)
			{
				printf("%d: %d", j, buf[j]);
			}
			return 0;
		}
	}
}

int smbus_read(int bus, int addr, int reg)
{
	int file;		/* fd */
	char filename[40];
	int ret;
	__s32 res;

	sprintf(filename,"/dev/i2c-%d", bus);
	if ((file = open(filename, O_RDWR)) < 0) {
		return errno;
	}

	if (ioctl(file, I2C_SLAVE, addr) < 0) {
		return errno;
	}

	res = i2c_smbus_read_word_data(file, reg);
	if (res < 0) {
		return 1;
	} else {
		printf("%d\n", res);
	}
}

int i2c_scan(bus)
{
	int i;
	for (i = 0; i <= 0xFF; i++)
	{
		printf("%d\n", i);
		if (smbus_read(4, i, 0) == 0)
		{
			printf("OK: %d", i);
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

	//i2c_read(4, 0b11111110, 0);
	//
	int bus;
	char *str, *endptr;

	bus = strtol(argv[1], &endptr, 10);
	// bus = atoi(argv[1]);

	printf("Scanning bus: %d\n", bus);
	i2c_scan(bus);
}


all:
	gcc `pkg-config --cflags glib-2.0` -lm i2c.c -o i2c

clean:
	rm i2c

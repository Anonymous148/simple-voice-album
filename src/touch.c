#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <linux/input.h>
#include"touch.h"

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

int get_finger_move_direct(int *xx, int *yy)
{   struct input_event ev;
	int direct;
	int fd;
	int x0 = -1, y0 = -1;
	int x1, y1;
    int delta_x, delta_y;
	fd = open("/dev/input/event0", O_RDWR);
	//if (fd == -1)
	if (-1 == fd)
	{
		printf("open errror!\n");
		return -1;
	}
	
	while (1)
	{
		read(fd, &ev, sizeof(ev));
		if ((ev.type == EV_ABS) && (ev.code == ABS_X))
		{
			if (x0 == -1)
				x0 = ev.value;
			else
				x1 = ev.value;
			xx = ev.value;
			printf("x: %d\n", x0);
		}
		else if ((ev.type == EV_ABS) && (ev.code == ABS_Y))
		{
			if (y0 == -1)
				y0 = ev.value;
			else
				y1 = ev.value;
			yy = ev.value;
			printf("y: %d\n", y0);
		}

		if (ev.type == EV_KEY && ev.code == BTN_TOUCH)
		{
			if (ev.value == 0)
			{
				printf("Release\n");
				break;
			}
			else if (ev.value == 1)
				printf("Press\n");
		}
	}
	delta_x = abs(x1 - x0);
	delta_y = abs(y1 - y0);
	
    if (delta_y > delta_x)
	{
		if (y1 > y0)
		{
			printf("down....\n");
			direct = DOWN;
		}
		else
		{
			printf("up...\n");
			direct = UP;
		}
	}
	else
	{
		if (x1 > x0)
		{
			printf("right....\n");
			direct = RIGHT;
		}
		else
		{
			printf("left ...\n");
			direct = LEFT;
		}
	}
	close(fd);
	return direct;
}
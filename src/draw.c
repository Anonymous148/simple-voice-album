#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include"draw.h"
int lcd_fd=0;
int lcd_init()
{
	lcd_fd = open("/dev/fb0", O_RDWR);
	if(lcd_fd == -1)
	{
		perror("open lcd fail");
		return -1;
	}
	
	p = mmap(NULL, 800*480*4, PROT_READ | PROT_WRITE, MAP_SHARED, lcd_fd, 0);
	if(p == NULL)
	{
		perror("mmap fail");
		return -1;
	}
	
	return 0;
}
void lcd_uninit()
{
	munmap(p, 800*480*4);
	close(lcd_fd);
}
void draw_white()
{	
	int i,j;
	unsigned int color = 0x00ffffff;
	for(i=0; i<480; i++)
	{
		for(j=0; j<800; j++)
		{
			draw_point( j, i,color);
		}
	}
}
void draw_point(int x,int y,int color)
{
	if(x>=0 && x<800 && y>=0 && y<480 )
	{
		*(p+800*y+x) = color ;
	}
}
void show_bmp(char *pathname, int x, int y)
{
	int fd = open(pathname, O_RDONLY);
	if (fd == -1)
	{
		perror("open error\n");
		return;
	}
	int fd1 = open("/dev/fb0", O_RDWR);
	if (fd1 == -1)
	{
		perror("open error\n");
		return;
	}
	printf("open success\n");

	p = mmap(NULL, 800 * 480 * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd1, 0);
	if (p == NULL)
	{
		perror("mmap error\n");
		return;
	}
	int width, height;
	short depth;
	unsigned char buf[4];
	//读取宽度
	lseek(fd, 0x12, SEEK_SET);
	read(fd, buf, 4);
	width = buf[3] << 24 | buf[2] << 16 | buf[1] << 8 | buf[0];
	//读取高度
	read(fd, buf, 4);
	height = buf[3] << 24 | buf[2] << 16 | buf[1] << 8 | buf[0];
	//读取色深
	lseek(fd, 0x1c, SEEK_SET);
	read(fd, buf, 2);
	depth = buf[1] << 8 | buf[0];
	//打印信息
	printf("width = %d  height = %d  depth = %d \n", width, height, depth);

	//像素数组
	int line_valid_bytes = abs(width) * depth / 8; //一行本有的有效字节
	int laizi = 0;								   //填充字节
	if ((line_valid_bytes % 4) != 0)
	{
		laizi = 4 - line_valid_bytes % 4;
	}
	int line_bytes = line_valid_bytes + laizi; //一行所有的字节数

	int total_bytes = line_bytes * abs(height); //整个像素数组的大小

	unsigned char *p1 = malloc(total_bytes);

	lseek(fd, 54, SEEK_SET);
	read(fd, p1, total_bytes);

	
	unsigned char a, r, g, b;
	int i = 0;			
	int x0 = 0, y0 = 0; 
	int color;
	for (y0 = 0; y0 < abs(height); y0++) 
	{
		for (x0 = 0; x0 < abs(width); x0++) 
		{
	
			b = p1[i++];
			g = p1[i++];
			r = p1[i++];
			if (depth == 32)
			{
				a = p1[i++];
			}
			if (depth == 24)
			{
				a = 0;
			}
			color = a << 24 | r << 16 | g << 8 | b;
			draw_point(width > 0 ? x + x0 : abs(width) + x - 1 - x0,
					   height > 0 ? y + height - 1 - y0 : y + y0, color);
		}
		i = i + laizi; 
		usleep(100);
	}
	free(p1);
	close(fd1);
	munmap(p, 800 * 480 * 4);
	close(fd);
}
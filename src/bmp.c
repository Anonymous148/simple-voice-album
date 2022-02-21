#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h> // malloc 函数的头文件
#include <string.h> // strcpy 函数的头文件
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <math.h>
#include <stdlib.h>
#include "list.h"
#include "draw.h"
#include "bmp.h"

void show1(struct List *p)
{
	int n = 5;
	struct node *p1 = p->first;
	while (n)
	{
		p1 = p1->next;
		sleep(1);
		show_bmp(p1->data, 0, 0);
		n--;
	}
}
void shown(struct List *p,struct node *p2)
{
		p2 = p2->next;
		show_bmp(p2->data, 0, 0);
}
void showp(struct List *p,struct node *p2)
{
		p2 = p2->prev;
		show_bmp(p2->data, 0, 0);
}
void bootvideo()
{
	show_bmp("/IOT/kaiji/6.bmp", 0, 0);
	usleep(10);
	show_bmp("/IOT/kaiji/7.bmp", 0, 0);
	usleep(10);
	show_bmp("/IOT/kaiji/8.bmp", 0, 0);
	usleep(10);
	show_bmp("/IOT/kaiji/9.bmp", 0, 0);
	usleep(10);
	show_bmp("/IOT/kaiji/10.bmp", 0, 0);
}
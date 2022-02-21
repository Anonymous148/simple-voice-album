#ifndef __PAINT_H
#define __PAINT_H
int *p;
int lcd_init();
void lcd_uninit();
void draw_white();
void draw_point(int x,int y,int color);
void show_bmp (char * pathname ,int x ,int y);
#endif

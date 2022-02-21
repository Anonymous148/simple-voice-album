#include "common.h"
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include "draw.h"
#include "bmp.h"
#include "list.h"
#include "touch.h"
#include "voi.h"
#include <stdlib.h> // malloc 函数的头文件
#include <string.h> // strcpy 函数的头文件
#include <dirent.h>
#include <math.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include "testDB.h"
#include <sqlite3.h>

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

#define REC_CMD "arecord -d3 -c1 -r16000 -traw -fS16_LE cmd.pcm"
#define PCM_FILE "./cmd.pcm"

//int said = 0;

int sockfd;
int xx, yy;
int id;
pthread_t pt1, pt2, pt3;
char *commandbuf = "null";
int Info;


int main(int argc, char *argv[])
{

	char data[100];
	int sockfd = init_asr(argv[1]);
	struct dirent *dirp = NULL;
	DIR *p = opendir("/IOT");
	struct List *p1 = create_list();
	struct node *p2 = p1->first;

	sqlite3 *db = NULL;
	int rc;
	char *zErrMsg = 0;
	int i, j;
	int NUM=0;

	printf("[SQLite]===================open=================\n");

	rc = sqlite3_open("test.db", &db); //打开指定的数据库文件,如果不存在将创建一个同名的数据库文件
	if (rc)
	{
		//fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		printf("[SQLite]Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
	}
	printf("[SQLite]===================create=================\n");
	//创建一个表,如果该表存在，则不创建，并给出提示信息，存储在 zErrMsg 中
	char *sql = "CREATE TABLE ID(NUM INTEGER PRIMARY KEY,Info VARCHAR(12),ID VARCHAR(5));";
	sqlite3_exec(db, sql, 0, 0, &zErrMsg);

	while (1)
	{
		dirp = readdir(p);
		if (dirp == NULL)
		{
			perror("readdir fail");
			break;
		}
		int t = strlen(dirp->d_name);
		if (strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0)
		{
			continue;
		}
		if (dirp->d_name[t - 1] == 'p' && dirp->d_name[t - 2] == 'm' && dirp->d_name[t - 3] == 'b' && dirp->d_name[t - 4] == '.')
		{ // puts(dirp->d_name);
			strcpy(data, dirp->d_name);

			p1 = add_list(p1, data);
		}
	}

	lcd_init();
	bootvideo();

	struct input_event ev;
	int x = -1, y = -1;
	int touch_fd = open("/dev/input/event0", O_RDONLY);
	if (touch_fd < 0)
	{
		perror("open touch fail");
		return -1;
	}
	while (1)
	{
		read(touch_fd, &ev, sizeof(ev));
		if (ev.type == EV_ABS && ev.code == ABS_X)
			x = ev.value * 800 / 1024;
		if (ev.type == EV_ABS && ev.code == ABS_Y)
			y = ev.value * 800 / 1024;
		if (ev.type == EV_KEY && ev.code == BTN_TOUCH)
		{
			if (ev.value == 1)
			{
				if (x > 0 && x < 266) 
				{
					show1(p1);
					show_bmp("/IOT/kaiji/10.bmp", 0, 0);
					continue;
				}
				else if (x > 267 && x < 532)
				{
					int n = 5;
					struct node *p2 = p1->first;
					int direct;
					while (n)
					{
						direct = get_finger_move_direct(&xx, &yy);
						switch (direct)
						{
						case UP:
							p2 = p2->next;
							show_bmp(p2->data, 0, 0);
							n--;
							break;
						case DOWN:
							p2 = p2->prev;
							show_bmp(p2->data, 0, 0);
							n--;
							break;
						case LEFT:
							p2 = p2->next;
							show_bmp(p2->data, 0, 0);
							n--;
							break;
						case RIGHT:
							p2 = p2->prev;
							show_bmp(p2->data, 0, 0);
							n--;
							break;
						}
					}
					show_bmp("/IOT/kaiji/10.bmp", 0, 0);
				}
				else if (x > 533 && x < 800)
				{
					struct node *p2 = p1->first;
					int id;
				
					while (1)
					{
						NUM++;
						id = run_asr(sockfd);
				
						if (id == 1)
						{
							show1(p1);
							char sql[1024] = {0};
							sprintf(sql, "INSERT INTO ID VALUES(%d,'自动播放','1');",NUM);
							sqlite3_exec(db, sql, 0, 0, &zErrMsg);

							continue;
						}
						else if (id == 2)
						{
							p2 = p2->next;
							show_bmp(p2->data, 0, 0);
							//id = run_asr(sockfd);
							char sql[1024] = {0};
							sprintf(sql, "INSERT INTO ID VALUES( %d,'上滑','2');",NUM);
							sqlite3_exec(db, sql, 0, 0, &zErrMsg);
							continue;
						}
						else if (id == 3)
						{

							p2 = p2->prev;
							show_bmp(p2->data, 0, 0);
							//id = run_asr(sockfd);
							char sql[1024] = {0};
							sprintf(sql, "INSERT INTO ID VALUES(%d,'下滑','3');",NUM);
							sqlite3_exec(db, sql, 0, 0, &zErrMsg);

							continue;
						}
						else if (id == 4)
						{
							p2 = p2->next;
							show_bmp(p2->data, 0, 0);
							//id = run_asr(sockfd);
							char sql[1024] = {0};
							sprintf(sql, "INSERT INTO ID VALUES(%d ,'左滑','4');",NUM);
							sqlite3_exec(db, sql, 0, 0, &zErrMsg);

							continue;
						}
						else if (id == 5)
						{

							p2 = p2->prev;
							show_bmp(p2->data, 0, 0);
							//id = run_asr(sockfd);
							char sql[1024] = {0};
							sprintf(sql, "INSERT INTO ID VALUES(%d ,'右滑','5');",NUM);
							sqlite3_exec(db, sql, 0, 0, &zErrMsg);

							continue;
						}
						else if (id == 999)
						{
							show_bmp("/IOT/white/b.bmp", 0, 0);
							printf("[SQLite]===================query=================\n");
							sql = "SELECT * FROM ID;";
							query(db, sql);
							break;
						}
					}
				}
				x = -1;
				y = -1;
			}
		}
	}
	close(touch_fd);
	uinit_asr(sockfd);
	lcd_uninit();
	closedir(p);
}
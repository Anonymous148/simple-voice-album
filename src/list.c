#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h> // malloc 函数的头文件
#include <string.h> // strcpy 函数的头文件
#include <dirent.h>
#include "list.h"

struct List *create_list()
{
	struct List *p = malloc(sizeof(struct List));
	p->first = NULL;
	p->last = NULL;
	return p;
}

struct List *add_list(struct List *p, char *data)
{
	//step 1 每一次保存一个数据 就要创建一个数据结点
	struct node *p1 = malloc(sizeof(struct node));
	strcpy(p1->data, data);
	p1->next = NULL;
	p1->prev = NULL;

	//step 2 把结点假如到链表中
	//是不是第一个节点
	if (p->first == NULL)
	{
		p->first = p1;
		p->last = p1;
		p1->prev = p1;
		p1->next = p1;
	}
	else //尾插法
	{
		p1->prev = p->last;
		p->last->next = p1;
		p->last = p1;
		p->first->prev = p1;
		p1->next = p->first;
	}

	return p;
}
void printf_list(struct List *p)
{
	struct node *p1 = p->first;
	while (p1)
	{
		p1 = p1->next;
		printf("%s", p1->data);
		if (p1->next == p->last)
			break;
	}
}

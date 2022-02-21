#ifndef __LIST_H
#define __LIST_H
typedef struct node 
{
    struct node *next;
    struct node *prev;
    char  data[100];// 保存 图片的绝对路径 
}  node;
	
typedef  struct List
{
    struct node *first;
    struct node *last;
    
} List;
struct List* create_list();
struct List* add_list(struct List *p,char *data);
void printf_list(struct List * p);
#endif

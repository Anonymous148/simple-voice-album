#ifndef __TESTDB_H
#define __TESTDB_H
#include<sqlite3.h>
int query(sqlite3 *db,char *sql);
static int callback(void *data, int argc, char **argv, char **azColName);
int get_finger_move_direct(int *xx, int *yy);
#endif
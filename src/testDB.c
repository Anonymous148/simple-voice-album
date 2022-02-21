#include <stdio.h>
#include <string.h>
#include"testDB.h"

typedef	unsigned char	UINT8;
typedef	unsigned short	UINT16;


int query(sqlite3 *db,char *sql){
	int nrow = 0, ncolumn = 0;
	int i,j;
	char *zErrMsg = 0;
	char **azResult; //二维数组存放结果
	sqlite3_get_table( db , sql , &azResult , &nrow , &ncolumn , &zErrMsg );

	printf( "[SQLite]row:%d column=%d \n" , nrow , ncolumn );
	for( i=0 ; i< nrow + 1; i++ ){
		for( j=0 ; j< ncolumn ; j++ ){
			printf( "%s ",azResult[i * ncolumn + j]);
		}
		printf( "\n");
	}

	sqlite3_free_table( azResult );
}

static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   printf("[SQLite]%s: ", (const char*)data);
   for(i=0; i<argc; i++){
      printf("%s = %s  ", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

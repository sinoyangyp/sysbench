#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "decimal.h"
#include "dmi/sqltypes.h"
#include "dmi/sqlhdr.h"
#include "dmi/sqlca.h"
#include "esql/locator.h"

//#define SINO_DEBUG
EXEC SQL include decimal;
struct InformixConnData {
	int saveSqlCode;
	int saveIsamError;

	char sysdesc[128];
	char sel_id[128]; 
	char sel_curs[128];
	char sql[2048];
};


void* mallocInformixConnData(void) {
	void* data = malloc(sizeof(struct InformixConnData));
	memset(data, 0, sizeof(struct InformixConnData));
	return data;
}


void freeInformixConnData(void* data) {
	free(data);
}


void showConnectionName(void) {
	printf("connection name = %s\n", ifx_getcur_conn_name());
}


void saveSqlStatus(void* p_data) {
	((struct InformixConnData*)p_data)->saveSqlCode=SQLCODE;
	((struct InformixConnData*)p_data)->saveIsamError=sqlca.sqlerrd[1]; 
}


int getSaveSqlCode(void* p_data) {
	return ((struct InformixConnData*)p_data)->saveSqlCode;
}


int getSaveIsamError(void* p_data) {
	return ((struct InformixConnData*)p_data)->saveIsamError;
}


int informixNewConnection(int connectionId, const char* connectionNameP, const char* dbName, const char* userNameP, const char* passwordP, void* p_data) {
  $const char* connectionName;
  $const char* db;
  $const char* userName;
  $const char* password;
  int sqlCode;
  int affectedRow;
  db=dbName;
  connectionName = connectionNameP;
  userName = userNameP;
  password = passwordP;
  $connect to $db as :connectionName user :userName using :password;
  if( SQLCODE != 0 ) {
		saveSqlStatus(p_data);
		return SQLCODE;
  }
  sqlCode = informixExecSql(&affectedRow, "set lock mode to wait 5;", connectionId, 0,  p_data);
  if (sqlCode != 0) 
    return sqlCode;
  
  // this must be added to allow data field value contains mutiple lines
  sqlCode = informixExecSql(&affectedRow, "execute procedure ifx_allow_newline('t');", connectionId, 1, p_data);
  if (sqlCode != 0) 
    return sqlCode;
    
  return 0;
}


int informixDisconnect(void* p_data) {
	EXEC SQL disconnect current;
	if( SQLCODE != 0 )
		saveSqlStatus(p_data);

	return SQLCODE;
}


int informixActiveConnection(const char* connectionNameP, void* p_data) {
	$const char* connectionName;
	connectionName = connectionNameP;
	EXEC SQL set connection :connectionName;
	if( SQLCODE != 0 )
		saveSqlStatus(p_data);

	return SQLCODE;
}


int informixExecSql(int* affectedRowCount, const char *sql, int connectionId, int increaseId, void* p_data) {
	int result;
	char statement_id[1024];
	result = statementPrepare(statement_id, sql, connectionId, increaseId, p_data);
	if ( result != 0 )
		return result;

	result = statementExecute(affectedRowCount, statement_id, p_data);
	statementFree(statement_id);	
	return result;
}

/*
int informixExecSqlDescriptor(int* affectedRowCount, const char* sqlStr, void* p_data, struct sqlda* p_sqlda) {
	int result;
	char statement_id[1024];
	result = statementPrepare(statement_id, sqlStr, p_data);
	if ( result != 0 )
		return result;

	result = statementExecuteSqlda(affectedRowCount, statement_id, p_sqlda, p_data);
	statementFree(statement_id);	
	return result;
}*/


static void resetCursorString(void* p_data) {
	((struct InformixConnData*)p_data)->sysdesc[0] = '\0';
	((struct InformixConnData*)p_data)->sel_id[0] = '\0';
	((struct InformixConnData*)p_data)->sel_curs[0] = '\0';
}


void closeCursor(void* p_data) {	
	EXEC SQL BEGIN DECLARE SECTION;
	char* sel_curs;
	char* sysdesc;
	char* sel_id;
	EXEC SQL END DECLARE SECTION;
	sysdesc = ((struct InformixConnData*)p_data)->sysdesc;
	sel_id = ((struct InformixConnData*)p_data)->sel_id;
	sel_curs = ((struct InformixConnData*)p_data)->sel_curs;

	EXEC SQL close : sel_curs;
	EXEC SQL deallocate descriptor : sysdesc;
	EXEC SQL free : sel_id;
	EXEC SQL free : sel_curs;
}


int statementPrepare(char* p_statement_id, const char* sqlStr, int connectionId, int increaseId, void* p_data) {
	EXEC SQL BEGIN DECLARE SECTION;
	const char* actSqlStr;
	char* statement_id;	
	EXEC SQL END DECLARE SECTION;
	actSqlStr = sqlStr;
	statement_id = p_statement_id;

	sprintf(statement_id, "statement_id_%d_%d", connectionId, increaseId);
	EXEC SQL prepare : statement_id from : actSqlStr;
	if (0 != SQLCODE) { 
		saveSqlStatus(p_data);
		statement_id[0] = '\0';
	}
	return SQLCODE;
}


void statementFree(char* p_statement_id) {	
	EXEC SQL BEGIN DECLARE SECTION;
	char* statement_id;
	EXEC SQL END DECLARE SECTION;
	if ( p_statement_id[0] == '\0')
		return;
	statement_id = p_statement_id;
	EXEC SQL free : statement_id;
	statement_id[0] = '\0';
}


int statementOpenCursor( char* p_cursor_id, const char* p_statement_id , int connectionId, int increaseId, void* p_data) {
	EXEC SQL BEGIN DECLARE SECTION;
	const char* statement_id;	
	char* cursor_id;
	EXEC SQL END DECLARE SECTION;
	statement_id = p_statement_id;
	cursor_id = p_cursor_id;

	sprintf(cursor_id, "cursor_id_%d_%d", connectionId, increaseId);
	EXEC SQL declare : cursor_id cursor for : statement_id;
	if (0 != SQLCODE) {
		saveSqlStatus(p_data);
		cursor_id[0] = '\0';
		return SQLCODE;
	}

	EXEC SQL open : cursor_id;
	if (0 != SQLCODE) {
		saveSqlStatus(p_data);
		EXEC SQL free : cursor_id;
		cursor_id[0] = '\0';
		return ((struct InformixConnData*)p_data)->saveSqlCode;
	}

	return 0;
}


int statementOpenCursorSqlda(char* p_cursor_id, const char* p_statement_id, struct sqlda* p_sqlda , int connectionId, int increaseId, void* p_data) {
	EXEC SQL BEGIN DECLARE SECTION;
	const char* statement_id;	
	char* cursor_id;
	EXEC SQL END DECLARE SECTION;
	statement_id = p_statement_id;
	cursor_id = p_cursor_id;

	sprintf(cursor_id, "cursor_id_%d_%d", connectionId, increaseId);
	EXEC SQL declare : cursor_id cursor for : statement_id;
	if (0 != SQLCODE) {
		saveSqlStatus(p_data);
		cursor_id[0] = '\0';
		return SQLCODE;
	}

	EXEC SQL open : cursor_id  using DESCRIPTOR p_sqlda;
	if (0 != SQLCODE) {
		saveSqlStatus(p_data);
		EXEC SQL free : cursor_id;
		cursor_id[0] = '\0';
		return ((struct InformixConnData*)p_data)->saveSqlCode;
	}

	return 0;
}


void statementCloseCursor(char* p_cursor_id) {	
	EXEC SQL BEGIN DECLARE SECTION;
	char* cursor_id;
	EXEC SQL END DECLARE SECTION;
	if ( p_cursor_id[0] == '\0' )
		return;
	cursor_id = p_cursor_id;
	EXEC SQL close : cursor_id;
	EXEC SQL free : cursor_id;
	cursor_id[0] = '\0';
}


int statementExecute(int* affectedRowCount, const char* p_statement_id , void* p_data) {
	EXEC SQL BEGIN DECLARE SECTION;
	const char* statement_id;	
	EXEC SQL END DECLARE SECTION;
	statement_id = p_statement_id;
	$execute : statement_id;
	if (0 != SQLCODE) 
		saveSqlStatus(p_data);
	else
		*affectedRowCount = sqlca.sqlerrd[2]; 

	return SQLCODE;
}


int statementExecuteSqlda(int* affectedRowCount, const char* p_statement_id, struct sqlda* p_sqlda , void* p_data) {
	EXEC SQL BEGIN DECLARE SECTION;
	const char* statement_id;	
	EXEC SQL END DECLARE SECTION;
	statement_id = p_statement_id;
	$execute : statement_id using DESCRIPTOR p_sqlda;
	if (0 != SQLCODE) 
		saveSqlStatus(p_data);
	else
		*affectedRowCount = sqlca.sqlerrd[2]; 

	return SQLCODE;
}


int statementGetDescription(const char* p_statement_id, struct sqlda** p_p_outputSqlda , void* p_data) {
	EXEC SQL BEGIN DECLARE SECTION;
	char* sel_id;
	EXEC SQL END DECLARE SECTION;
	struct sqlda* p_outputSqlda;
	sel_id = p_statement_id;
	EXEC SQL describe : sel_id into p_outputSqlda;
	if (0 != SQLCODE) 
		saveSqlStatus(p_data);
	*p_p_outputSqlda = p_outputSqlda; 
	return SQLCODE;
}


#define SQLBIGINT       52

void printSqlda(struct sqlda* sqlda, int count){
	void* data;
	int i, j;
	struct sqlvar_struct* col_ptr;
	char* type;
	int ind;
	char i1, i2;
	// print number of columns (sqld) and number of fetch-array elements
	printf("\nsqld: %d, fetch-array elements: %d.\n", sqlda->sqld, count);
	/* Outer loop: loop through each element of a fetch array */
	for ( j = 0; j < count; j++ ){
		/* Inner loop: loop through each of the sqlvar_struct structures */
		for ( i = 0, col_ptr = sqlda->sqlvar; i < sqlda->sqld; i++, col_ptr++ ){
			data = col_ptr->sqldata + (j * col_ptr->sqllen);
			switch ( col_ptr->sqltype ){
			case SQLCHAR:
			case SQLVCHAR:
				printf("%s", data);
				break;
			case SQLINT:
				printf("%d", *(int*)data);
				break;
			case SQLINT8:
			case SQLBIGINT:
				printf("%lld", *(int8_t*)data);
				break;
			case SQLDECIMAL: {
				char result[80];
				memset(result, 0, 80);
				dectoasc(data, result, sizeof(result), -1);
				printf("%s", result);
			}
				break;
			default:
				printf("type not implemented: can¡¯t print %d", col_ptr->sqltype);
				break;
			} /* end switch */

			printf("\t");
		} /* end for (i=0...) */
		printf("\n");
	} /* end for (j=0...) */
}

int getSelectRowCountSqlda(const char* p_cursorId, struct sqlda* p_sqlda) {
	EXEC SQL BEGIN DECLARE SECTION;
	const char* cursorId; 
	EXEC SQL END DECLARE SECTION;
	int total = 0;
	cursorId = p_cursorId;

	for ( ; ; ){
		EXEC SQL fetch : cursorId using descriptor p_sqlda;
		if ( SQLCODE != 0 )
			break;
		total += sqlca.sqlerrd[2];
		#ifdef SINO_DEBUG
		printSqlda(p_sqlda, sqlca.sqlerrd[2]);
		#endif
	}
	return total;
}



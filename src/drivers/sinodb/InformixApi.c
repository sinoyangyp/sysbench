#include <sqlhdr.h>
#include <sqliapi.h>
#line 1 "InformixApi.ec"
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
/* 
 * EXEC SQL include decimal;
 */
#line 13 "InformixApi.ec"

#line 13 "InformixApi.ec"
#line 1 "/data/fc8_xmpoc/incl/esql/decimal.h"
/****************************************************************************
 *
 * Licensed Material - Property Of IBM
 *
 * "Restricted Materials of IBM"
 *
 * IBM Informix Client SDK
 * Copyright IBM Corporation 1997, 2010. All rights reserved.
 *
 *  Title:       decimal.h
 *  Description: DECIMAL (and MONEY) data type.
 *
 ***************************************************************************
 */

#ifndef _DECIMAL_H
#define _DECIMAL_H

#include "ifxtypes.h"

#ifdef NT_MI_SAPI
#ifdef NT_SERVER /* NT Server */
#define MI_EXT_DECL __declspec(dllexport)
#else            /* NT Blade */
#define MI_EXT_DECL __declspec(dllimport)
#endif /* NT_SERVER */
#else            /* UNIX Blade & Server - Default */
#define MI_EXT_DECL extern
#endif /* !NT_MI_SAPI */

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Unpacked Format (format for program usage)
 *
 *    Signed exponent "dec_exp" ranging from  -64 to +63
 *    Separate sign of mantissa "dec_pos"
 *    Base 100 digits (range 0 - 99) with decimal point
 *	immediately to the left of first digit.
 */

#define DECSIZE 16
#define DECUNKNOWN -2

struct decimal
    {
    int2 dec_exp;		/* exponent base 100		*/
    int2 dec_pos;		/* sign: 1=pos, 0=neg, -1=null	*/
    int2 dec_ndgts;		/* number of significant digits	*/
    char  dec_dgts[DECSIZE];	/* actual digits base 100	*/
    };
typedef struct decimal dec_t;

/*
 *  A decimal null will be represented internally by setting dec_pos
 *  equal to DECPOSNULL
 */

#define DECPOSNULL	(-1)

#define DECPOSPOS       +1
#define DECPOSNEG        0
#define DECEXPZERO     -64
#define DECEXPNULL	 0
#define DECEXPMAX      +63
#define DECEXPMIN      -64

/*
 * DECLEN calculates minumum number of bytes
 * necessary to hold a decimal(m,n)
 * where m = total # significant digits and
 *	 n = significant digits to right of decimal
 */

#define DECLEN(m,n)	(((m)+((n)&1)+3)/2)
#define DECLENGTH(len)	DECLEN(PRECTOT(len),PRECDEC(len))

/*
 * DECPREC calculates a default precision given
 * number of bytes used to store number
 */

#define DECPREC(size)	(((size-1)<<9)+2)

/* macros to look at and make encoded decimal precision
 *
 *  PRECTOT(x)		return total precision (digits total)
 *  PRECDEC(x) 		return decimal precision (digits to right)
 *  PRECMAKE(x,y)	make precision from total and decimal
 */

#define PRECTOT(x)	(((x)>>8) & 0xff)
#define PRECDEC(x)	((x) & 0xff)
#define PRECMAKE(x,y)	(((x)<<8) + (y))

/*
 * Packed Format  (format in records in files)
 *
 *    First byte =
 *	  top 1 bit = sign 0=neg, 1=pos
 *	  low 7 bits = Exponent in excess 64 format
 *    Rest of bytes = base 100 digits in 100 complement format
 *    Notes --	This format sorts numerically with just a
 *		simple byte by byte unsigned comparison.
 *		Zero is represented as 80,00,00,... (hex).
 *		Negative numbers have the exponent complemented
 *		and the base 100 digits in 100's complement
 */

/*
** DECIMALTYPE Functions
*/
MI_EXT_DECL mint decadd(struct decimal *n1, struct decimal *n2, struct decimal *n3);
MI_EXT_DECL mint decsub(struct decimal *n1, struct decimal *n2, struct decimal *n3);
MI_EXT_DECL mint decmul(struct decimal *n1, struct decimal *n2, struct decimal *n3);
MI_EXT_DECL mint decdiv(struct decimal *n1, struct decimal *n2, struct decimal *n3);
MI_EXT_DECL mint deccmp(struct decimal *n1, struct decimal *n2);
MI_EXT_DECL void deccopy(struct decimal *n1, struct decimal *n2);
MI_EXT_DECL mint deccvasc(char *cp, mint len, struct decimal *np);
MI_EXT_DECL mint deccvdbl(double dbl, struct decimal *np);
MI_EXT_DECL mint deccvint(mint in, struct decimal *np);
MI_EXT_DECL mint deccvlong(int4 lng, struct decimal *np);
MI_EXT_DECL char *dececvt(struct decimal *np, mint ndigit, mint *decpt, mint *sign);
MI_EXT_DECL char *decfcvt(struct decimal *np, mint ndigit, mint *decpt, mint *sign);
MI_EXT_DECL void decround(struct decimal *np, mint dec_round);
MI_EXT_DECL mint dectoasc(struct decimal *np, char *cp, mint len, mint right);
MI_EXT_DECL mint dectodbl(struct decimal *np, double *dblp);
MI_EXT_DECL mint dectoint(struct decimal *np, mint *ip);
MI_EXT_DECL mint dectolong(struct decimal *np, int4 *lngp);
MI_EXT_DECL void dectrunc(struct decimal *np, mint trunc);
MI_EXT_DECL mint deccvflt(double source, struct decimal *destination);
MI_EXT_DECL mint dectoflt(struct decimal *source, float *destination);

#ifdef __cplusplus
}
#endif

#endif /* _DECIMAL_H */
#line 140 "/data/fc8_xmpoc/incl/esql/decimal.h"
#line 14 "InformixApi.ec"
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
/*
 *   $const char* connectionName;
 */
#line 59 "InformixApi.ec"
const   char *connectionName;
/*
 *   $const char* db;
 */
#line 60 "InformixApi.ec"
const   char *db;
/*
 *   $const char* userName;
 */
#line 61 "InformixApi.ec"
const   char *userName;
/*
 *   $const char* password;
 */
#line 62 "InformixApi.ec"
const   char *password;
  int sqlCode;
  int affectedRow;
  db=dbName;
  connectionName = connectionNameP;
  userName = userNameP;
  password = passwordP;
/*
 *   $connect to $db as :connectionName user :userName using :password;
 */
#line 69 "InformixApi.ec"
  {
#line 69 "InformixApi.ec"
  ifx_conn_t      *_sqiconn;
  _sqiconn = (ifx_conn_t *)ifx_alloc_conn_user(userName, password);
  sqli_connect_open(ESQLINTVERSION, 0, db, connectionName, _sqiconn, 0);
  ifx_free_conn_user(&_sqiconn);
#line 69 "InformixApi.ec"
  }
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
/*
 * 	EXEC SQL disconnect current;
 */
#line 88 "InformixApi.ec"
  {
#line 88 "InformixApi.ec"
  sqli_connect_close(3, (char *)0, 0, 0);
#line 88 "InformixApi.ec"
  }
	if( SQLCODE != 0 )
		saveSqlStatus(p_data);

	return SQLCODE;
}


int informixActiveConnection(const char* connectionNameP, void* p_data) {
/*
 * 	$const char* connectionName;
 */
#line 97 "InformixApi.ec"
const   char *connectionName;
	connectionName = connectionNameP;
/*
 * 	EXEC SQL set connection :connectionName;
 */
#line 99 "InformixApi.ec"
  {
#line 99 "InformixApi.ec"
  sqli_connect_set(0, connectionName, 0);
#line 99 "InformixApi.ec"
  }
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
/*
 * 	EXEC SQL BEGIN DECLARE SECTION;
 */
#line 141 "InformixApi.ec"
#line 142 "InformixApi.ec"
  char *sel_curs;
  char *sysdesc;
  char *sel_id;
/*
 * 	EXEC SQL END DECLARE SECTION;
 */
#line 145 "InformixApi.ec"

	sysdesc = ((struct InformixConnData*)p_data)->sysdesc;
	sel_id = ((struct InformixConnData*)p_data)->sel_id;
	sel_curs = ((struct InformixConnData*)p_data)->sel_curs;

/*
 * 	EXEC SQL close : sel_curs;
 */
#line 150 "InformixApi.ec"
  {
#line 150 "InformixApi.ec"
  sqli_curs_close(ESQLINTVERSION, sqli_curs_locate(ESQLINTVERSION, sel_curs, 256));
#line 150 "InformixApi.ec"
  }
/*
 * 	EXEC SQL deallocate descriptor : sysdesc;
 */
#line 151 "InformixApi.ec"
  {
#line 151 "InformixApi.ec"
  sqli_desc_dealloc(sysdesc);
#line 151 "InformixApi.ec"
  }
/*
 * 	EXEC SQL free : sel_id;
 */
#line 152 "InformixApi.ec"
  {
#line 152 "InformixApi.ec"
  sqli_mt_free(sel_id);
#line 152 "InformixApi.ec"
  }
/*
 * 	EXEC SQL free : sel_curs;
 */
#line 153 "InformixApi.ec"
  {
#line 153 "InformixApi.ec"
  sqli_mt_free(sel_curs);
#line 153 "InformixApi.ec"
  }
}


int statementPrepare(char* p_statement_id, const char* sqlStr, int connectionId, int increaseId, void* p_data) {
/*
 * 	EXEC SQL BEGIN DECLARE SECTION;
 */
#line 158 "InformixApi.ec"
#line 159 "InformixApi.ec"
const   char *actSqlStr;
  char *statement_id;
/*
 * 	EXEC SQL END DECLARE SECTION;
 */
#line 161 "InformixApi.ec"

	actSqlStr = sqlStr;
	statement_id = p_statement_id;

	sprintf(statement_id, "statement_id_%d_%d", connectionId, increaseId);
/*
 * 	EXEC SQL prepare : statement_id from : actSqlStr;
 */
#line 166 "InformixApi.ec"
  {
#line 166 "InformixApi.ec"
  sqli_prep(ESQLINTVERSION, statement_id, actSqlStr,(ifx_literal_t *)0, (ifx_namelist_t *)0, -1, 0, 0 ); 
#line 166 "InformixApi.ec"
  }
	if (0 != SQLCODE) { 
		saveSqlStatus(p_data);
		statement_id[0] = '\0';
	}
	return SQLCODE;
}


void statementFree(char* p_statement_id) {	
/*
 * 	EXEC SQL BEGIN DECLARE SECTION;
 */
#line 176 "InformixApi.ec"
#line 177 "InformixApi.ec"
  char *statement_id;
/*
 * 	EXEC SQL END DECLARE SECTION;
 */
#line 178 "InformixApi.ec"

	if ( p_statement_id[0] == '\0')
		return;
	statement_id = p_statement_id;
/*
 * 	EXEC SQL free : statement_id;
 */
#line 182 "InformixApi.ec"
  {
#line 182 "InformixApi.ec"
  sqli_mt_free(statement_id);
#line 182 "InformixApi.ec"
  }
	statement_id[0] = '\0';
}


int statementOpenCursor( char* p_cursor_id, const char* p_statement_id , int connectionId, int increaseId, void* p_data) {
/*
 * 	EXEC SQL BEGIN DECLARE SECTION;
 */
#line 188 "InformixApi.ec"
#line 189 "InformixApi.ec"
const   char *statement_id;
  char *cursor_id;
/*
 * 	EXEC SQL END DECLARE SECTION;
 */
#line 191 "InformixApi.ec"

	statement_id = p_statement_id;
	cursor_id = p_cursor_id;

	sprintf(cursor_id, "cursor_id_%d_%d", connectionId, increaseId);
/*
 * 	EXEC SQL declare : cursor_id cursor for : statement_id;
 */
#line 196 "InformixApi.ec"
  {
#line 196 "InformixApi.ec"
  sqli_curs_decl_dynm(ESQLINTVERSION, sqli_curs_locate(ESQLINTVERSION, cursor_id, 0), cursor_id, sqli_curs_locate(ESQLINTVERSION, statement_id, 1), 0, 0);
#line 196 "InformixApi.ec"
  }
	if (0 != SQLCODE) {
		saveSqlStatus(p_data);
		cursor_id[0] = '\0';
		return SQLCODE;
	}

/*
 * 	EXEC SQL open : cursor_id;
 */
#line 203 "InformixApi.ec"
  {
#line 203 "InformixApi.ec"
  sqli_curs_open(ESQLINTVERSION, sqli_curs_locate(ESQLINTVERSION, cursor_id, 256), (ifx_sqlda_t *)0, (char *)0, (struct value *)0, 0, 0);
#line 203 "InformixApi.ec"
  }
	if (0 != SQLCODE) {
		saveSqlStatus(p_data);
/*
 * 		EXEC SQL free : cursor_id;
 */
#line 206 "InformixApi.ec"
  {
#line 206 "InformixApi.ec"
  sqli_mt_free(cursor_id);
#line 206 "InformixApi.ec"
  }
		cursor_id[0] = '\0';
		return ((struct InformixConnData*)p_data)->saveSqlCode;
	}

	return 0;
}


int statementOpenCursorSqlda(char* p_cursor_id, const char* p_statement_id, struct sqlda* p_sqlda , int connectionId, int increaseId, void* p_data) {
/*
 * 	EXEC SQL BEGIN DECLARE SECTION;
 */
#line 216 "InformixApi.ec"
#line 217 "InformixApi.ec"
const   char *statement_id;
  char *cursor_id;
/*
 * 	EXEC SQL END DECLARE SECTION;
 */
#line 219 "InformixApi.ec"

	statement_id = p_statement_id;
	cursor_id = p_cursor_id;

	sprintf(cursor_id, "cursor_id_%d_%d", connectionId, increaseId);
/*
 * 	EXEC SQL declare : cursor_id cursor for : statement_id;
 */
#line 224 "InformixApi.ec"
  {
#line 224 "InformixApi.ec"
  sqli_curs_decl_dynm(ESQLINTVERSION, sqli_curs_locate(ESQLINTVERSION, cursor_id, 0), cursor_id, sqli_curs_locate(ESQLINTVERSION, statement_id, 1), 0, 0);
#line 224 "InformixApi.ec"
  }
	if (0 != SQLCODE) {
		saveSqlStatus(p_data);
		cursor_id[0] = '\0';
		return SQLCODE;
	}

/*
 * 	EXEC SQL open : cursor_id  using DESCRIPTOR p_sqlda;
 */
#line 231 "InformixApi.ec"
  {
#line 231 "InformixApi.ec"
  sqli_curs_open(ESQLINTVERSION, sqli_curs_locate(ESQLINTVERSION, cursor_id, 256), p_sqlda, (char *)0, (struct value *)0, 1, 0);
#line 231 "InformixApi.ec"
  }
	if (0 != SQLCODE) {
		saveSqlStatus(p_data);
/*
 * 		EXEC SQL free : cursor_id;
 */
#line 234 "InformixApi.ec"
  {
#line 234 "InformixApi.ec"
  sqli_mt_free(cursor_id);
#line 234 "InformixApi.ec"
  }
		cursor_id[0] = '\0';
		return ((struct InformixConnData*)p_data)->saveSqlCode;
	}

	return 0;
}


void statementCloseCursor(char* p_cursor_id) {	
/*
 * 	EXEC SQL BEGIN DECLARE SECTION;
 */
#line 244 "InformixApi.ec"
#line 245 "InformixApi.ec"
  char *cursor_id;
/*
 * 	EXEC SQL END DECLARE SECTION;
 */
#line 246 "InformixApi.ec"

	if ( p_cursor_id[0] == '\0' )
		return;
	cursor_id = p_cursor_id;
/*
 * 	EXEC SQL close : cursor_id;
 */
#line 250 "InformixApi.ec"
  {
#line 250 "InformixApi.ec"
  sqli_curs_close(ESQLINTVERSION, sqli_curs_locate(ESQLINTVERSION, cursor_id, 256));
#line 250 "InformixApi.ec"
  }
/*
 * 	EXEC SQL free : cursor_id;
 */
#line 251 "InformixApi.ec"
  {
#line 251 "InformixApi.ec"
  sqli_mt_free(cursor_id);
#line 251 "InformixApi.ec"
  }
	cursor_id[0] = '\0';
}


int statementExecute(int* affectedRowCount, const char* p_statement_id , void* p_data) {
/*
 * 	EXEC SQL BEGIN DECLARE SECTION;
 */
#line 257 "InformixApi.ec"
#line 258 "InformixApi.ec"
const   char *statement_id;
/*
 * 	EXEC SQL END DECLARE SECTION;
 */
#line 259 "InformixApi.ec"

	statement_id = p_statement_id;
/*
 * 	$execute : statement_id;
 */
#line 261 "InformixApi.ec"
  {
#line 261 "InformixApi.ec"
  sqli_exec(ESQLINTVERSION, sqli_curs_locate(ESQLINTVERSION, statement_id, 257), (ifx_sqlda_t *)0, (char *)0, (struct value *)0, (ifx_sqlda_t *)0, (char *)0, (struct value *)0, 0);
#line 261 "InformixApi.ec"
  }
	if (0 != SQLCODE) 
		saveSqlStatus(p_data);
	else
		*affectedRowCount = sqlca.sqlerrd[2]; 

	return SQLCODE;
}


int statementExecuteSqlda(int* affectedRowCount, const char* p_statement_id, struct sqlda* p_sqlda , void* p_data) {
/*
 * 	EXEC SQL BEGIN DECLARE SECTION;
 */
#line 272 "InformixApi.ec"
#line 273 "InformixApi.ec"
const   char *statement_id;
/*
 * 	EXEC SQL END DECLARE SECTION;
 */
#line 274 "InformixApi.ec"

	statement_id = p_statement_id;
/*
 * 	$execute : statement_id using DESCRIPTOR p_sqlda;
 */
#line 276 "InformixApi.ec"
  {
#line 276 "InformixApi.ec"
  sqli_exec(ESQLINTVERSION, sqli_curs_locate(ESQLINTVERSION, statement_id, 257), p_sqlda, (char *)0, (struct value *)0, (ifx_sqlda_t *)0, (char *)0, (struct value *)0, 0);
#line 276 "InformixApi.ec"
  }
	if (0 != SQLCODE) 
		saveSqlStatus(p_data);
	else
		*affectedRowCount = sqlca.sqlerrd[2]; 

	return SQLCODE;
}


int statementGetDescription(const char* p_statement_id, struct sqlda** p_p_outputSqlda , void* p_data) {
/*
 * 	EXEC SQL BEGIN DECLARE SECTION;
 */
#line 287 "InformixApi.ec"
#line 288 "InformixApi.ec"
  char *sel_id;
/*
 * 	EXEC SQL END DECLARE SECTION;
 */
#line 289 "InformixApi.ec"

	struct sqlda* p_outputSqlda;
	sel_id = p_statement_id;
/*
 * 	EXEC SQL describe : sel_id into p_outputSqlda;
 */
#line 292 "InformixApi.ec"
  {
#line 292 "InformixApi.ec"
  sqli_describe_stmt(ESQLINTVERSION, sqli_curs_locate(ESQLINTVERSION, sel_id, 257), &p_outputSqlda, 0);
#line 292 "InformixApi.ec"
  }
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
/*
 * 	EXEC SQL BEGIN DECLARE SECTION;
 */
#line 347 "InformixApi.ec"
#line 348 "InformixApi.ec"
const   char *cursorId;
/*
 * 	EXEC SQL END DECLARE SECTION;
 */
#line 349 "InformixApi.ec"

	int total = 0;
	cursorId = p_cursorId;

	for ( ; ; ){
/*
 * 		EXEC SQL fetch : cursorId using descriptor p_sqlda;
 */
#line 354 "InformixApi.ec"
  {
#line 354 "InformixApi.ec"
  static _FetchSpec _FS0 = { 0, 1, 0 };
  sqli_curs_fetch(ESQLINTVERSION, sqli_curs_locate(ESQLINTVERSION, cursorId, 256), (ifx_sqlda_t *)0, p_sqlda, (char *)0, &_FS0);
#line 354 "InformixApi.ec"
  }
		if ( SQLCODE != 0 )
			break;
		total += sqlca.sqlerrd[2];
		#ifdef SINO_DEBUG
		printSqlda(p_sqlda, sqlca.sqlerrd[2]);
		#endif
	}
	return total;
}



#line 365 "InformixApi.ec"

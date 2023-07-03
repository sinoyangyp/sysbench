#ifdef STDC_HEADERS
# include <stdio.h>
#endif
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef HAVE_STRING_H
# include <string.h>
#endif
#ifdef HAVE_STRINGS_H
# include <strings.h>
#endif
#include <stdio.h>
#include <ctype.h>
#include "sb_options.h"
#include "db_driver.h"

#include "esql/sqlhdr.h"
#include "esql/sqliapi.h"
#include "esql/sqltypes.h"
#include <stdlib.h>
#include <unistd.h>
//#include "InformixApi.h"
#include "InformixApi.c"
//#define WITH_FETCH
//#define SINO_ERROR
#ifdef SINO_ERROR
#define SINO_ERROR_PRINTF printf
#else
#define SINO_ERROR_PRINTF(...)
#endif

//#define SINO_DEBUG
#ifdef SINO_DEBUG
#define SINO_DEBUG_PRINTF printf
#else
#define SINO_DEBUG_PRINTF(...)
#endif

/* SinoDB driver arguments */
// SinoDB 驱动参数，只有在以下参数列表中的命令才可以解析
static sb_arg_t sinodb_drv_args[] =
{
  SB_OPT("sinodb-user", "SinoDB user", "informix", STRING),
  SB_OPT("sinodb-password", "SinoDB password", "Ids4test", STRING),
  SB_OPT("sinodb-db", "SinoDB database name", "sbtest@demo", STRING),
  SB_OPT("sino-ignore-errors", "list of errors to ignore, or \"all\"", "-268,-271,-143,-154", LIST),
  SB_OPT_END
};

// 命令行参数值结构体
typedef struct
{
  const char         *user;
  const char         *password;
  const char         *db;
  sb_list_t* ignored_errors;
} sinodb_drv_args_t;

int getStringFieldNum(struct sqlda* p_sqlda){
	int i;
	struct sqlvar_struct* col_ptr;
	int total = 0;

	for ( i = 0, col_ptr = p_sqlda->sqlvar; i < p_sqlda->sqld; i++, col_ptr++ ){
		if ( col_ptr->sqltype == SQLCHAR || col_ptr->sqltype == SQLVCHAR || col_ptr->sqltype == SQLDECIMAL)
			++total;
	}

	return total;
}


int getIntFieldNum(struct sqlda* p_sqlda){
	int i;
	struct sqlvar_struct* col_ptr;
	int total = 0;

	for ( i = 0, col_ptr = p_sqlda->sqlvar; i < p_sqlda->sqld; i++, col_ptr++ ){
		if ( col_ptr->sqltype != SQLCHAR && col_ptr->sqltype != SQLVCHAR && col_ptr->sqltype != SQLDECIMAL)
			++total;
	}

	return total;
}

#define MAX_FETCH_COUNT 500


int calcFetchArraySize(struct sqlda* p_sqlda){
	int i;
	int arraySize;
	struct sqlvar_struct* col_ptr;
	int msgLen = 0;
	SINO_DEBUG_PRINTF("fetch column count=%d\n", p_sqlda->sqld);

	for ( i = 0, col_ptr = p_sqlda->sqlvar; i < p_sqlda->sqld; i++, col_ptr++ ){
		SINO_DEBUG_PRINTF("before rtypmsize fetch column[%d] sqltype=%d, sqllen=%d\n", i, col_ptr->sqltype, col_ptr->sqllen);
		col_ptr->sqllen = rtypmsize(col_ptr->sqltype, col_ptr->sqllen);
		msgLen += col_ptr->sqllen; /* get database sizes */
		SINO_DEBUG_PRINTF("after rtypmsize fetch column[%d]  sqltype=%d, sqllen=%d\n", i, col_ptr->sqltype, col_ptr->sqllen);
	}

	arraySize = FetBufSize / msgLen;
	SINO_DEBUG_PRINTF("arraySize = %d\n", arraySize);
	if ( arraySize > MAX_FETCH_COUNT )
		arraySize = MAX_FETCH_COUNT;
	return arraySize;
}


struct MySqlda{
	char** stringArray;
	int totalStringArray;

	char** intArray;
	int totalIntArray;
};


#define MAX_STRING_ARRAY_LEN 64000
#define MAX_INT_ARRAY_LEN 5000


void allocMySqlda(struct MySqlda* p_mySqlda, int totalStringArray, int totalIntArray) {
	int stringArrayIndex;
	int intArrayIndex;
	p_mySqlda->totalStringArray = totalStringArray;
	p_mySqlda->stringArray = (char**)calloc(1, totalStringArray * sizeof(char*));
	for ( stringArrayIndex = 0; stringArrayIndex < totalStringArray; ++stringArrayIndex )
		p_mySqlda->stringArray[stringArrayIndex] = (char*)malloc(MAX_STRING_ARRAY_LEN);

	p_mySqlda->totalIntArray = totalIntArray;
	p_mySqlda->intArray = (char**)calloc(1, totalIntArray * sizeof(char*));
	for ( intArrayIndex = 0; intArrayIndex < totalIntArray; ++intArrayIndex )
		p_mySqlda->intArray[intArrayIndex] = (char*)malloc(MAX_INT_ARRAY_LEN);
}


void freeMySqlda(struct MySqlda* p_mySqlda) {
	if ( p_mySqlda->stringArray != 0 ){
		int stringArrayIndex;
		for ( stringArrayIndex = 0; stringArrayIndex < p_mySqlda->totalStringArray; ++stringArrayIndex )
			free(p_mySqlda->stringArray[stringArrayIndex]);

		free(p_mySqlda->stringArray);
		p_mySqlda->stringArray = 0; 
		p_mySqlda->totalStringArray = 0;
	}

	if ( p_mySqlda->intArray != 0 ){
		int intArrayIndex;
		for ( intArrayIndex = 0; intArrayIndex < p_mySqlda->totalIntArray; ++intArrayIndex )
			free(p_mySqlda->intArray[intArrayIndex]);

		free(p_mySqlda->intArray); 
		p_mySqlda->totalIntArray = 0;
		p_mySqlda->intArray = 0;
	}
}


void setSqldaDataPtr(struct sqlda* p_sqlda, struct MySqlda* p_mySqlda){
	int intIndex;
	int stringIndex;
	int i;
	struct sqlvar_struct* col_ptr;
	stringIndex = 0;
	intIndex = 0;
	for ( i = 0, col_ptr = p_sqlda->sqlvar; i < p_sqlda->sqld; i++, col_ptr++ ){
		if ( col_ptr->sqltype == SQLCHAR || col_ptr->sqltype == SQLVCHAR || col_ptr->sqltype == SQLDECIMAL){
			col_ptr->sqldata = p_mySqlda->stringArray[stringIndex];
			++stringIndex;
		}
		else{
			col_ptr->sqldata = p_mySqlda->intArray[intIndex];
			++intIndex;
		}
	}
}


//sinodb数据库连接的属性结构体
typedef struct
{
    char connectionName[204];
    void* informixConnData;
    int haveConnected;
	char errorCode[128];
	char isamCode[1024 + 1];
	int connectionId;
	int increaseId;
#ifdef WITH_FETCH
	struct MySqlda outputMySqlda;
#endif
} db_sinodb_conn_t;

/* Structure used for DB-to-SinoDB bind types map 
   用于DB到sinodb绑定类型映射的结构
*/

typedef struct
{
  db_bind_type_t   db_type;
  int              sinodb_type;
} db_sinodb_bind_map_t;

/* DB-to-SinoDB bind types map
   DB到sinodb绑定类型映射数组
   sinodb类型在sqltypes.h文件中  
*/

db_sinodb_bind_map_t db_sinodb_bind_map[] =
{
  {DB_TYPE_TINYINT,   0},
  {DB_TYPE_SMALLINT,  CSHORTTYPE},
  {DB_TYPE_INT,       CINTTYPE},
  {DB_TYPE_BIGINT,    CBIGINTTYPE},
  {DB_TYPE_FLOAT,     CFLOATTYPE},
  {DB_TYPE_DOUBLE,    CDOUBLETYPE},
  {DB_TYPE_DATETIME,  CDTIMETYPE},
  {DB_TYPE_TIMESTAMP, 0},
  {DB_TYPE_CHAR,      CCHARTYPE},
  {DB_TYPE_VARCHAR,   CVCHARTYPE},
  {DB_TYPE_NONE,      0}
};

/* SinoDB driver capabilities */
// 驱动的功能
static drv_caps_t sinodb_drv_caps =
{
  0,      /* multi_rows_insert 多行插入*/
  1,      /* prepared_statements 准备语句*/
  0,      /* auto_increment 自动递增*/
  0,      /* needs_commit 需要提交*/
  1,      /* serial  序列*/
  0       /* unsigned int 无符号整型*/
};


static sinodb_drv_args_t args;          /* driver args */

/* SinoDB driver operations */

static int sinodb_drv_init(void);
static int sinodb_drv_describe(drv_caps_t *);
static int sinodb_drv_connect(db_conn_t *);
static int sinodb_drv_reconnect(db_conn_t *);
static int sinodb_drv_disconnect(db_conn_t *);
static int sinodb_drv_prepare(db_stmt_t *, const char *, size_t);
static int sinodb_drv_bind_param(db_stmt_t *, db_bind_t *, size_t);
static int sinodb_drv_bind_result(db_stmt_t *, db_bind_t *, size_t);
static db_error_t sinodb_drv_execute(db_stmt_t *, db_result_t *);
static int sinodb_drv_fetch(db_result_t *);
static int sinodb_drv_fetch_row(db_result_t *, db_row_t *);
static db_error_t sinodb_drv_query(db_conn_t *, const char *, size_t,db_result_t *);
static int sinodb_drv_free_results(db_result_t *);
static int sinodb_drv_close(db_stmt_t *);
static int sinodb_drv_done(void);



long long getInformixIncreaseId(){
	static long long s_increaseId = 0;
	static pthread_mutex_t id_mutex;
	long long returnId;
	pthread_mutex_lock(&id_mutex);
	++s_increaseId;
	returnId = s_increaseId;
	pthread_mutex_unlock(&id_mutex);
	return returnId;
}


static db_driver_t sinodb_driver =
{
  .sname = "sinodb",
  .lname = "SinoDB driver",
  .args = sinodb_drv_args,
  .ops = {
    .init = sinodb_drv_init,
    .describe = sinodb_drv_describe,
    .connect = sinodb_drv_connect,
    .disconnect = sinodb_drv_disconnect,
    .reconnect = sinodb_drv_reconnect,
    .prepare = sinodb_drv_prepare,
    .bind_param = sinodb_drv_bind_param,
    .bind_result = sinodb_drv_bind_result,
    .execute = sinodb_drv_execute,
    .fetch = sinodb_drv_fetch,
    .fetch_row = sinodb_drv_fetch_row,
    .free_results = sinodb_drv_free_results,
    .close = sinodb_drv_close,
    .query = sinodb_drv_query,
    .done = sinodb_drv_done
  }
};


/* Register SinoDB driver */
//注册驱动程序
int register_driver_sinodb(sb_list_t *drivers)
{
  printf("1:register_driver_sinodb\n");
  SB_LIST_ADD_TAIL(&sinodb_driver.listitem, drivers);
  return 0;
}


/* SinoDB driver initialization */
int sinodb_drv_init(void)
{
  args.user = sb_get_value_string("sinodb-user");
  args.password = sb_get_value_string("sinodb-password");
  args.db = sb_get_value_string("sinodb-db");
  args.ignored_errors = sb_get_value_list("sino-ignore-errors");
#ifdef WITH_FETCH
  FetBufSize = 31 * 1024;
#endif
  // 设置禁止ps模式
  db_globals.ps_mode = DB_PS_MODE_DISABLE;  
  return 0;
}

/* Describe database capabilities 
   描述数据库功能
   例如根据数据库版本，确定哪些功能具备，修改caps的参数值0或者1
   当前默认版本功能已知，初始化caps已设定
*/

int sinodb_drv_describe(drv_caps_t *caps)
{
  *caps = sinodb_drv_caps;
#ifndef ENABLE_CAPS_INIT
   caps->multi_rows_insert = 0;
   caps->auto_increment = 0;
#endif

  return 0;
}

/* Connect to SinoDB database */
int sinodb_drv_connect(db_conn_t* sb_conn){
	db_sinodb_conn_t* db_sinodb_con = calloc(1, sizeof(db_sinodb_conn_t));
	db_sinodb_con->informixConnData = mallocInformixConnData();
	db_sinodb_con->connectionId = getInformixIncreaseId();
	sprintf(db_sinodb_con->connectionName, "db_%d", db_sinodb_con->connectionId);

	if ( informixNewConnection(db_sinodb_con->connectionId, db_sinodb_con->connectionName, args.db, args.user, args.password, db_sinodb_con->informixConnData) == 0 ){
		SINO_DEBUG_PRINTF("%s sinodb_drv_connect success!\n", db_sinodb_con->connectionName);
		db_sinodb_con->haveConnected = 1;
	}
	else{
		SINO_ERROR_PRINTF("informix connection %s connect to %s error,sqlcode=%d,isam_error=%d\n",
			db_sinodb_con->connectionName, args.db, getSaveSqlCode(db_sinodb_con->informixConnData), 
			getSaveIsamError(db_sinodb_con->informixConnData));
		freeInformixConnData(db_sinodb_con->informixConnData);
		free(db_sinodb_con);
		return 1;
	}
#ifdef WITH_FETCH
	allocMySqlda(&db_sinodb_con->outputMySqlda, 3, 3);
#endif
	sb_conn->ptr = db_sinodb_con;
	return 0;
}


/* Disconnect from SinoDB database */
int sinodb_drv_disconnect(db_conn_t *sb_conn)
{
	db_sinodb_conn_t* db_sinodb_con = (db_sinodb_conn_t*)sb_conn->ptr;
	if ( db_sinodb_con == NULL )
		return 0;

	if ( !db_sinodb_con->haveConnected )
		return 0;

	informixDisconnect(db_sinodb_con->informixConnData);
	SINO_DEBUG_PRINTF("%s sinodb_drv_disconnect\n", db_sinodb_con->connectionName);
	freeInformixConnData(db_sinodb_con->informixConnData);
#ifdef WITH_FETCH
	freeMySqlda(&db_sinodb_con->outputMySqlda);
#endif
	free(db_sinodb_con);

	return 0;
}

/* Reset connection to the server by reconnecting with the same parameters. 
   未测试，暂时不用管
*/

static int sinodb_drv_reconnect(db_conn_t *sb_con)
{
/*
  if (sinodb_drv_disconnect(sb_con))
    return DB_ERROR_FATAL;
  while (sinodb_drv_connect(sb_con))
  {
    if (sqlca.sqlcode)
      return DB_ERROR_FATAL;
  }
*/
  return DB_ERROR_IGNORABLE;
}


/* 从DB的类型获取sinodb数据类型  */

int get_sinodb_bind_type(db_bind_type_t type){
	unsigned int i;

	for ( i = 0; db_sinodb_bind_map[i].db_type != DB_TYPE_NONE; i++ ){
		if ( db_sinodb_bind_map[i].db_type == type )
			return db_sinodb_bind_map[i].sinodb_type;
	}

	return -1;
}

struct SinoStmt{
	char statementId[1024];
	int isSelect;
	
	char cursorId[1024];
	char descriptionId[1024];

	struct sqlda* p_sqlda;
#ifdef WITH_FETCH
	struct sqlda* p_outputSqlda;
	int fetchArraySize;
	struct MySqlda outputMySqlda;
#endif
};


void freeSinoStmt(struct SinoStmt* p_stmt) {
	if ( p_stmt == 0 )
		return;

	if ( p_stmt->p_sqlda != 0  )
		free( p_stmt->p_sqlda );

#ifdef WITH_FETCH
	if ( p_stmt->p_outputSqlda != 0  )
		free( p_stmt->p_outputSqlda);

	freeMySqlda(&p_stmt->outputMySqlda);
#endif
	free(p_stmt);
}

char* ltrim(char* s){
	while ( isspace(*s) ) s++;
	return s;
}

/* Prepare statement */
// 准备语句stmt
// 使用模拟器情况下不执行prepare
int sinodb_drv_prepare(db_stmt_t *stmt, const char *query, size_t len)
{
	void* informixConnData;
	int result;
	struct SinoStmt* sinoStmt;
	db_sinodb_conn_t* db_sinodb_con = (db_sinodb_conn_t*)stmt->connection->ptr;
	query = ltrim(query);
	if ( db_sinodb_con == NULL ){
		SINO_ERROR_PRINTF("informix connection %s prepare statement in driver prepare, but db_sino_con is null, sql:%s\n", db_sinodb_con->connectionName, query);
		return 1;
	}

	if ( !db_sinodb_con->haveConnected ){
		SINO_ERROR_PRINTF("informix connection %s prepare statement in driver prepare, but db_sino_con have not connected, sql:%s\n", db_sinodb_con->connectionName, query);
		return 1;
	}

	stmt->query = strdup(query);

	sinoStmt = calloc(1, sizeof(struct SinoStmt));
	sinoStmt->isSelect = (strncmp(stmt->query, "SELECT", 6) == 0);
	SINO_DEBUG_PRINTF("informix connection %s prepare statement in prepare, is_select=%d,sql:%s\n", 
		db_sinodb_con->connectionName, sinoStmt->isSelect, stmt->query);
	if ( stmt->ptr != 0 ){
		freeSinoStmt((struct SinoStmt*)stmt->ptr);
		stmt->ptr = 0;
	}

	informixConnData = db_sinodb_con->informixConnData;
	result = statementPrepare(sinoStmt->statementId, stmt->query, db_sinodb_con->connectionId, ++db_sinodb_con->increaseId, informixConnData);
	if ( result != 0 ){
		SINO_ERROR_PRINTF("informix connection %s prepare statement in prepare error, sql code=%d, isam_error=%d, sql:%s\n",
			db_sinodb_con->connectionName, getSaveSqlCode(informixConnData), getSaveIsamError(informixConnData), stmt->query);
		free(stmt->query);
		free(sinoStmt);
		return result;
	}

#ifdef WITH_FETCH
	if ( sinoStmt->isSelect ){
		if ( statementGetDescription(sinoStmt->statementId, &sinoStmt->p_outputSqlda, informixConnData ) != 0 ){
			SINO_ERROR_PRINTF("informix connection %s in prepare get description error, sql code=%d, isam_error=%d, sql:%s\n",
				db_sinodb_con->connectionName, getSaveSqlCode(informixConnData), getSaveIsamError(informixConnData), stmt->query);
			statementFree(sinoStmt->statementId);
			free(stmt->query);
			free(sinoStmt);
			return result;
		}
		sinoStmt->fetchArraySize = calcFetchArraySize(sinoStmt->p_outputSqlda);
		allocMySqlda(&sinoStmt->outputMySqlda, getStringFieldNum(sinoStmt->p_outputSqlda), getIntFieldNum(sinoStmt->p_outputSqlda));
		setSqldaDataPtr(sinoStmt->p_outputSqlda, &sinoStmt->outputMySqlda);
	}
#endif

	stmt->ptr = sinoStmt;
	return 0;
}

// 准备语句的绑定参数
int sinodb_drv_bind_param(db_stmt_t *stmt, db_bind_t *params, size_t len)
{
	struct sqlda* p_sqlda;
	
	db_sinodb_conn_t* db_sinodb_con = (db_sinodb_conn_t*)stmt->connection->ptr;
	if ( db_sinodb_con == NULL ){
		SINO_ERROR_PRINTF("informix connection %s driver bind param, but db_sino_con is null, sql:%s\n", 
			db_sinodb_con->connectionName, stmt->query);
		return 1;
	}

	if ( !db_sinodb_con->haveConnected ){
		SINO_ERROR_PRINTF("informix connection %s driver bind param, but db_sino_con have not connected, sql:%s\n", 
			db_sinodb_con->connectionName, stmt->query);
		return 1;
	}

	p_sqlda = calloc(1, sizeof(struct sqlda));
	p_sqlda->sqld = len;
	p_sqlda->sqlvar = calloc(len, sizeof(ifx_sqlvar_t));
	for ( size_t i = 0; i < len; ++i ){
		db_bind_t* p_dbBind = &params[i];
		p_sqlda->sqlvar[i].sqltype = get_sinodb_bind_type(p_dbBind->type);
		p_sqlda->sqlvar[i].sqllen = p_dbBind->max_len;
		p_sqlda->sqlvar[i].sqldata = (char*)(p_dbBind->buffer);
	}

	((struct SinoStmt*)stmt->ptr)->p_sqlda = p_sqlda;
	return 0;
}

/* Bind results for prepared statement */
int sinodb_drv_bind_result(db_stmt_t *stmt, db_bind_t *params, size_t len)
{
  return 0;
}


void printBindParam(struct sqlda* p_sqlda){
	if ( p_sqlda == 0 ){ 
		printf("no bind param\n"); 
		return;
	}

	for ( int i = 0; i < p_sqlda->sqld; ++i ){
		printf("param[i]=");
		switch ( p_sqlda->sqlvar[i].sqltype ){
		case CINTTYPE:
			printf("%d", *((int*)p_sqlda->sqlvar[i].sqldata));
			break;
		case CBIGINTTYPE:
			printf("%lld", *((long long*)p_sqlda->sqlvar[i].sqldata));
			break;
		case CCHARTYPE:
		case CVCHARTYPE:
			printf("%s", p_sqlda->sqlvar[i].sqldata);
			break;
		default:
			printf("unknown sql type %d\n");
			assert(false);
		}
		printf("\n");
	}
}


static db_error_t check_error(db_conn_t* sb_con, int errorCode, int isamCode, const char* query, sb_counter_type_t* counter){
	sb_list_item_t* pos;
	int   tmp;
	db_sinodb_conn_t* db_sinodb_con = (db_sinodb_conn_t*)sb_con->ptr;
	//sb_con->sql_errno = errorCode;
	sprintf(db_sinodb_con->errorCode, "error code = %d", errorCode);
	snprintf(db_sinodb_con->isamCode, 1024, "isam error code = %d, sql : %s", isamCode, query);
	db_sinodb_con->isamCode[1024] = '\0';
	sb_con->sql_state = db_sinodb_con->errorCode;
	sb_con->sql_errmsg = db_sinodb_con->isamCode;

	//SINO_ERROR_PRINTF("in check error, error code = %d\n", errorCode);
	//  Check if the error code is specified in --sino-ignore-errors, and return
	//  DB_ERROR_IGNORABLE if so, or DB_ERROR_FATAL otherwise
	SB_LIST_FOR_EACH(pos, args.ignored_errors){
		const char* val = SB_LIST_ENTRY(pos, value_t, listitem)->data;
		tmp = (unsigned int)atoi(val);
		//SINO_ERROR_PRINTF("ignore error code = %d\n", tmp);
		if ( errorCode == tmp || isamCode==tmp || !strcmp(val, "all") ){
			void* informixConnData;
			int affectedRow;
			db_sinodb_conn_t* db_sinodb_con = (db_sinodb_conn_t*)sb_con->ptr;
			informixConnData = db_sinodb_con->informixConnData;
			if ( informixExecSql(&affectedRow, "rollback", db_sinodb_con->connectionId, ++db_sinodb_con->increaseId, informixConnData) != 0 ){
				SINO_ERROR_PRINTF("informix connection %s rollback error, sql code=%d, isam_error=%d\n",
					db_sinodb_con->connectionName, getSaveSqlCode(informixConnData), getSaveIsamError(informixConnData));
			}

			SINO_ERROR_PRINTF("ignore error %d, return\n", errorCode);
			*counter = SB_CNT_ERROR;
			return DB_ERROR_IGNORABLE;
		}
	}

	SINO_ERROR_PRINTF("no ignore error code = %d, isam code = %d\n", errorCode, isamCode);
#if 0
	SB_LIST_FOR_EACH(pos, args.ignored_errors){
		const char* val = SB_LIST_ENTRY(pos, value_t, listitem)->data;
		tmp = (unsigned int)atoi(val);
		SINO_ERROR_PRINTF("ignore error code = %d\n", errorCode);
	}
#endif
	*counter = SB_CNT_ERROR;
	return DB_ERROR_FATAL;
}


/* Execute prepared statement */
db_error_t sinodb_drv_execute(db_stmt_t *stmt, db_result_t *rs)
{
	void* informixConnData;
	struct SinoStmt* sinoStmt;
	struct sqlda* p_sqlda;
	int result;
	db_sinodb_conn_t* db_sinodb_con;
	db_conn_t* sb_conn = stmt->connection;
	db_sinodb_con = (db_sinodb_conn_t*)sb_conn->ptr;
	if ( db_sinodb_con == NULL ){
		SINO_ERROR_PRINTF("informix connection %s execute,but connection is null, sql:%s\n", db_sinodb_con->connectionName, stmt->query);
		return 1;
	}

	if ( !db_sinodb_con->haveConnected ){
		SINO_ERROR_PRINTF("informix connection %s execute,but connection have not connected, sql:%s\n", 
			db_sinodb_con->connectionName, stmt->query);
		return 1;
	}

	sb_conn->sql_errno = 0;
	sb_conn->sql_state = NULL;
	sb_conn->sql_errmsg = NULL;

	informixConnData = db_sinodb_con->informixConnData;
	sinoStmt = (struct SinoStmt*)stmt->ptr;
	p_sqlda = sinoStmt->p_sqlda;

	if ( sinoStmt->isSelect ){
		int fieldCount;
		int nrows;

		SINO_DEBUG_PRINTF("informix connection %s select statement in execute, sql:%s\n", db_sinodb_con->connectionName, stmt->query);
	#ifdef SINO_DEBUG
		printBindParam(p_sqlda);	
	#endif
		if ( p_sqlda == 0 ){
			result = statementOpenCursor(sinoStmt->cursorId, sinoStmt->statementId, db_sinodb_con->connectionId, 
				++db_sinodb_con->increaseId, informixConnData);
		}
		else{
			result = statementOpenCursorSqlda(sinoStmt->cursorId, sinoStmt->statementId, p_sqlda, db_sinodb_con->connectionId, 
				++db_sinodb_con->increaseId, informixConnData);
		}
		if ( result != 0 ){
			SINO_ERROR_PRINTF("informix connection %s open cursor in execute error, sql code=%d, isam_error=%d, sql:%s\n",
				db_sinodb_con->connectionName, getSaveSqlCode(informixConnData), getSaveIsamError(informixConnData), stmt->query);
			return check_error(sb_conn, getSaveSqlCode(informixConnData), getSaveIsamError(informixConnData),stmt->query, &rs->counter);
		}
	#ifdef WITH_FETCH
		rs->nfields = sinoStmt->p_outputSqlda->sqld;
		FetArrSize = sinoStmt->fetchArraySize;
		rs->nrows = getSelectRowCountSqlda(sinoStmt->cursorId, sinoStmt->p_outputSqlda);
		SINO_DEBUG_PRINTF("informix connection %s open cursor in execute, select count=%d,sql:%s\n",
			db_sinodb_con->connectionName,rs->nrows,stmt->query);
	#else
		rs->nfields = 4;
		rs->nrows = 1;
	#endif

		statementCloseCursor(sinoStmt->cursorId);
		rs->counter = SB_CNT_READ;
	}
	else{
		int affectedRowCount;
		SINO_DEBUG_PRINTF("informix connection %s execute statement in execute, sql:%s\n", db_sinodb_con->connectionName, stmt->query);
	#ifdef SINO_DEBUG
		printBindParam(p_sqlda);	
	#endif
		if ( p_sqlda == 0 )
			result = statementExecute(&affectedRowCount, sinoStmt->statementId, informixConnData);
		else
			result = statementExecuteSqlda(&affectedRowCount, sinoStmt->statementId, p_sqlda, informixConnData);

		if ( result != 0 ){
			SINO_ERROR_PRINTF("informix connection %s execute in execute error, sql code=%d, isam_error=%d, sql:%s\n",
				db_sinodb_con->connectionName, getSaveSqlCode(informixConnData), getSaveIsamError(informixConnData), stmt->query);
			return check_error(sb_conn, getSaveSqlCode(informixConnData), getSaveIsamError(informixConnData), stmt->query, &rs->counter);
		}
		SINO_DEBUG_PRINTF("informix connection %s execute statement in execute, count=%d,sql:%s\n",db_sinodb_con->connectionName,
			affectedRowCount, stmt->query);
		rs->nrows = affectedRowCount;

		if ( (strncmp(stmt->query, "DELETE",6)==0) || (strncmp(stmt->query, "UPDATE",6)==0) ||(strncmp(stmt->query, "INSERT",6)==0 ) ) {
			SINO_DEBUG_PRINTF("informix connection %s execute statement in execute, counter type is write,sql:%s\n", 
				db_sinodb_con->connectionName, stmt->query);
			rs->counter = SB_CNT_WRITE;
		}
		else{
			SINO_DEBUG_PRINTF("informix connection %s execute statement in execute, counter type is other,sql:%s\n", 
				db_sinodb_con->connectionName, stmt->query);
			rs->counter = SB_CNT_OTHER;
		}
	}

	return DB_ERROR_NONE;
}


/* Execute SQL query */
db_error_t sinodb_drv_query(db_conn_t* sb_conn, const char* query, size_t len, db_result_t* rs) {
	void* informixConnData;
	int result;
	char statementId[1024];
	query = ltrim(query);
	db_sinodb_conn_t* db_sinodb_con = (db_sinodb_conn_t*)sb_conn->ptr;
	if ( db_sinodb_con == NULL ){
		SINO_ERROR_PRINTF("informix connection %s query,but connection is null, sql:%s\n", db_sinodb_con->connectionName, query);
		return 1;
	}

	if ( !db_sinodb_con->haveConnected ){
		SINO_ERROR_PRINTF("informix connection %s query,but connection have not connected, sql:%s\n",
			db_sinodb_con->connectionName, query);
		return 1;
	}

	sb_conn->sql_errno = 0;
	sb_conn->sql_state = NULL;
	sb_conn->sql_errmsg = NULL;

	informixConnData = db_sinodb_con->informixConnData;
	SINO_DEBUG_PRINTF("informix connection %s prepare statement in query, sql:%s\n", db_sinodb_con->connectionName, query);
	result = statementPrepare(statementId, query, db_sinodb_con->connectionId, ++db_sinodb_con->increaseId,informixConnData);
	if ( result != 0 ){
		SINO_ERROR_PRINTF("informix connection %s prepare statement in query error, sql code=%d, isam_error=%d, sql:%s\n",
			db_sinodb_con->connectionName, getSaveSqlCode(informixConnData), getSaveIsamError(informixConnData), query);
		return check_error(sb_conn, getSaveSqlCode(informixConnData), getSaveIsamError(informixConnData),query, &rs->counter);
	}

	if ( strncmp(query, "SELECT",6)==0) {
		char cursorId[1024];
		int fieldCount;
		int nrows;
		struct sqlda* p_outputSqlda;

		SINO_DEBUG_PRINTF("informix connection %s select statement in query, sql:%s\n", db_sinodb_con->connectionName, query);
		result = statementOpenCursor(cursorId, statementId, db_sinodb_con->connectionId, ++db_sinodb_con->increaseId,informixConnData);
		if ( result != 0 ){
			SINO_ERROR_PRINTF("informix connection %s open cursor in query error, sql code=%d, isam_error=%d, sql:%s\n",
				db_sinodb_con->connectionName, getSaveSqlCode(informixConnData), getSaveIsamError(informixConnData), query);
			statementFree(statementId);
			return check_error(sb_conn, getSaveSqlCode(informixConnData), getSaveIsamError(informixConnData), query, &rs->counter);
		}
	#ifdef WITH_FETCH
		if ( statementGetDescription(statementId, &p_outputSqlda, informixConnData ) != 0 ){
			SINO_ERROR_PRINTF("informix connection %s open cursor in query get description error, sql code=%d, isam_error=%d, sql:%s\n",
				db_sinodb_con->connectionName, getSaveSqlCode(informixConnData), getSaveIsamError(informixConnData), query);
			statementCloseCursor(cursorId);
			statementFree(statementId);
			return check_error(sb_conn, getSaveSqlCode(informixConnData), getSaveIsamError(informixConnData), query, &rs->counter);
		}
		rs->nfields = p_outputSqlda->sqld;
		setSqldaDataPtr(p_outputSqlda, &db_sinodb_con->outputMySqlda);
		FetArrSize = calcFetchArraySize(p_outputSqlda);
		rs->nrows = getSelectRowCountSqlda(cursorId, p_outputSqlda);
		SINO_DEBUG_PRINTF("informix connection %s open cursor in query, select count=%d,sql:%s\n", 
			db_sinodb_con->connectionName, rs->nrows, query);
		free(p_outputSqlda);
	#else
		rs->nfields = 4;
		rs->nrows = 1;
	#endif
		statementCloseCursor(cursorId);
		rs->counter = SB_CNT_READ;
	}
	else{
		int affectedRowCount;
		SINO_DEBUG_PRINTF("informix connection %s execute statement in query, sql:%s\n", db_sinodb_con->connectionName, query);
		result = statementExecute(&affectedRowCount, statementId, informixConnData);

		if ( result != 0 ){
			SINO_ERROR_PRINTF("informix connection %s execute in query error, sql code=%d, isam_error=%d, sql:%s\n",
				db_sinodb_con->connectionName, getSaveSqlCode(informixConnData), getSaveIsamError(informixConnData), query);
			statementFree(statementId);
			return check_error(sb_conn, getSaveSqlCode(informixConnData), getSaveIsamError(informixConnData), query, &rs->counter);
		}
		SINO_DEBUG_PRINTF("informix connection %s execute statement in query, count=%d,sql:%s\n", 
			db_sinodb_con->connectionName, affectedRowCount, query);
		rs->nrows = affectedRowCount;
		if ( (strncmp(query, "DELETE", 6) == 0) || (strncmp(query, "UPDATE", 6) == 0) || (strncmp(query, "INSERT", 6) == 0) ){
			SINO_DEBUG_PRINTF("informix connection %s execute statement in query, counter type is write,sql:%s\n", 
				db_sinodb_con->connectionName, query);
			rs->counter = SB_CNT_WRITE;
		}
		else{
			SINO_DEBUG_PRINTF("informix connection %s execute statement in query, counter type is other,sql:%s\n", 
				db_sinodb_con->connectionName, query);
			rs->counter = SB_CNT_OTHER;
		}
	}

	statementFree(statementId);
	return DB_ERROR_NONE;
}



/* Fetch row from result set of a prepared statement */


int sinodb_drv_fetch(db_result_t *rs)
{
  return 1;
}

/* Fetch row from result set of a query */

int sinodb_drv_fetch_row(db_result_t *rs, db_row_t *row)
{
  return DB_ERROR_NONE;
}

/* Free result set */

int sinodb_drv_free_results(db_result_t *rs)
{
  return 0;
}


/* Close prepared statement */
int sinodb_drv_close(db_stmt_t *stmt)
{
	{
		SINO_DEBUG_PRINTF("informix connection %s close statement, sql:%s\n", 
			((db_sinodb_conn_t*)stmt->connection->ptr)->connectionName, stmt->query);
	}

	if ( stmt->query ){
		free(stmt->query);
		stmt->query = NULL;
	}

	if ( stmt->ptr != 0 ){
		struct SinoStmt* sinoStmt = (struct SinoStmt*)stmt->ptr;
		statementFree(sinoStmt->statementId);
		freeSinoStmt(sinoStmt);
		stmt->ptr = NULL;
	}

	return 0;
}


/* Uninitialize driver */
int sinodb_drv_done(void)
{
	return 0;
}



/***************************************************************************
 *
 * Licensed Materials - Property of IBM
 *
 * Restricted Materials of IBM
 *
 * IBM Informix Dynamic Server
 * Copyright IBM Corporation 1996, 2013, 2015
 * All Rights Reserved
 *
 * Title:       cdrapi.h
 * Description: definitions for CDR API
 *
 ***************************************************************************
 */

#ifndef CDRAPI_H
#define CDRAPI_H

#ifdef NT
#include <time.h>
#else
#include <sys/time.h>		/* time_t */
#endif /*NT*/

#include "cdrerr.h"

#ifndef CDR_NEWMSG
#define CDR_NEWMSG(_x,_y) char *_x = _y
#endif

#ifdef NT
#define  CDRAPI_EXP_DECL   __declspec(dllexport)
#define  CDRAPI_EXP_DEFN   __declspec(dllexport)
#else
#define  CDRAPI_EXP_DECL   extern
#define  CDRAPI_EXP_DEFN
#endif

#define CDR_NAMESIZE		128	/* max len for CDR identifiers */
#define CDR_PATHSIZE		256

#define SVR_NAMESIZE		CDR_NAMESIZE	/* for server identifiers */
#define CDR_USERSIZE		34	/* max name len for user name */
								/* 32 + 2bytes to allow quoted ownernames
                                   for ansi support */
#define CDR_PROTOSIZE		16
#ifndef CDR_DIRNAMESIZE
#define CDR_DIRNAMESIZE		CDR_PATHSIZE /* ATS and RIS dir names */
#endif
#define CDR_LOGFILESIZE		CDR_PATHSIZE /* log filename */

#define CDR_SELECTSIZE		30000  /* max len for select statement */
#define CDR_STOREDPROCSIZE	128	/* max len for stored procedure */
#define CDR_1K			1024	/* for 1K buffers */
#define CDR_4K			4096	/* for 4K buffers */
#define HOST_LEN        256     /* max length of host name */
#define USERSIZE        32


/* GLS support for client locale field widths */
#ifndef TURBO /* frontend */
/* max # of bytes occupied by a logical multi-byte character */
#define MB_MAXSIZE			4
#define CDR_GL_NAMESIZE		(MB_MAXSIZE * CDR_NAMESIZE)
#define SVR_GL_NAMESIZE		(MB_MAXSIZE * SVR_NAMESIZE)
#define CDR_GL_USERSIZE		(MB_MAXSIZE * CDR_USERSIZE)
#define CDR_GL_SPSIZE		(MB_MAXSIZE * CDR_STOREDPROCSIZE)
#else /* backend */
#define CDR_GL_NAMESIZE		CDR_NAMESIZE
#define SVR_GL_NAMESIZE		SVR_NAMESIZE
#define CDR_GL_USERSIZE		CDR_USERSIZE
#define CDR_GL_SPSIZE		CDR_STOREDPROCSIZE
#endif /* ifndef TURBO */
#define CDR_GL_SELECTSIZE	CDR_SELECTSIZE

#define CDR_INTERNAL_SET_NAME   "ifx_internal_set"
#define CDR_INTERNAL_SET_ID   0x8100


/*
 * API datatypes
 */
#ifdef TURBO
#include <machine.h>
#else
#include <datetime.h>
#include <decimal.h>
#ifndef MACHINE_INCL
#ifndef _IFXTYPES_H_
typedef unsigned char		uint1;
typedef unsigned short		uint2;
#ifdef M64ADDR
typedef unsigned int 		uint4;
#else
typedef unsigned long 		uint4;
#endif /* M64ADDR */
#endif /* _IFXTYPES_H_ */
#ifndef  IDENTSIZE
#define IDENTSIZE			SVR_NAMESIZE
#endif /* IDENTSIZE */
#endif /* !define(MACHINE_INCL) */
#endif /* defined(TURBO) */

/* replicate definition flags */
#define D_MASTER		0x01U
#define D_PARTICIPANTS	0x02U
/*
 * Internal command. Used while cloning shadow replicate based on the
 * primary replicate information.
 */
#define	D_CDRFEINTERNALCMD	0x04U

/* replicate participant modes */
#define A_RDONLYMODE	0x001U
#define A_PRIMARYMODE	0x002U
/* replicate participant flags stored in the same field as modes */
/* A_USEINFORMIX is not actually stored in the partdef table, but */
/* is necessary to specifically tell the engine to use informix */
#define A_USETABOWNER	0x004U
#define A_USEINFORMIX   0x008U
/* If the replicate is a external resync shadow replicate, then */
/* we need to identify the source/targets so that we know remove the */
/* replicate.  */
#define A_EXTSYNCSOURCE  0x010U
#define A_EXTSYNCTARGET  0x020U
#define A_PENDCHKREPAIR  0x040U /* Check repair pending for this participant */

/*
 *  The participant will send data only
 */
#define A_SENDONLYMODE  0x080U
#define A_FORCEACTIVE   0x100U


#define A_SENDERMODE  (A_PRIMARYMODE|A_SENDONLYMODE)
#define	GET_PART_MODE(_m)	((_m) & \
 (A_RDONLYMODE|A_PRIMARYMODE|A_SENDONLYMODE))
#define	GET_PART_FLAGS(_f)	((_f) & \
	(A_USETABOWNER|A_EXTSYNCSOURCE|A_EXTSYNCTARGET|A_PENDCHKREPAIR))
#define STPARTMODE(flags)   \
     ((flags & A_PRIMARYMODE) ? 'P' : \
      (flags & A_SENDONLYMODE) ? 'S' : 'R')
#define LDPARTMODE(buf)     ((buf) == 'P' ? A_PRIMARYMODE : \
      (buf) == 'S' ? A_SENDONLYMODE : A_RDONLYMODE)


/* replicate attribute flags */
/* #define A_NONSERIAL		0x00000001U  */   /* DEAD? */
/* #define A_PRIMARY       0x00010000U   */   /* DEAD? active in 11.70 */
/* #define A_SECONDARY     0x01000000U   */   /* DEAD? active in 11.70 */

#define A_CANONICAL	    0x00000002U
#define A_FREQ			0x00000008U

/*
 * NOTE: These 2 options (sequential and parallel) are no longer supported
 */

/* Apply sequential (serially) flag */
#define A_SEQ			0x00000020U
#define A_NONSEQ		0x00000040U		/* Default */

#define A_EXCLUSIVE		0x00000080U
#define A_TSCOPE        0x00000100U     /* txn conflict resolution */
#define A_RSCOPE        0x00000200U     /* row-by-row scope */
#define A_RIS           0x00000400U     /* activate RIS */
#define A_ATS           0x00000800U     /* activate ATS */
#define A_TRIGGER       0x00001000U     /* fire triggers */
#define A_NRIS          0x00002000U     /* deactivate RIS */
#define A_NATS          0x00004000U     /* deactivate ATS */
#define A_NTRIGGER      0x00008000U     /* don't fire triggers */
#define A_REPL4RESYNC   0x00020000U     /* Internal replicate for resync job */
#define A_EXTENDEDATTR	0x00040000U		/* Has Extended Attributes */
#define A_SHARD         0x00080000U     /* replset is a shard */
#define A_GRID          0x00100000U     /* Grid Object */
#define A_TEMPLATE		0x00200000U     /* Template replset */
#define A_NONAMEVERF    0x00400000U     /* Don't verify column names */
#define A_MASTERED      0x00800000U     /* Mastered Replicate */
#define A_NSHADOWREPL   0x01000000U     /* Unset Shadow Replicate flag*/
#define A_SHADOWREPL    0x02000000U     /* Shadow Replicate */
#define A_IGNOREDELETES 0x04000000U     /* Ignore any deletes */
#define A_NOIGNOREDELETES 0x08000000U   /* don't ignore deletes */
#define A_NOCOLSKIP     0x10000000U     /* Always send all data */
#define A_COLSKIP       0x20000000U     /* Send only changed data */
#define A_FLOATIEEE		0x40000000U     /* Send floating point as IEEE */

#define ATTRSMASK       (A_CANONICAL|A_FREQ|A_GRID|\
                         A_SEQ|A_NONSEQ|A_EXCLUSIVE|\
                         A_TEMPLATE|A_NONAMEVERF|A_MASTERED|\
                         A_SHADOWREPL|A_IGNOREDELETES|\
                         A_COLSKIP|A_FLOATIEEE)

/*
 * conflict resolution scope and spooling flags
 * NOTE: these CANNOT conflict with replicate attribute flags above.
 */

#define A_SCOPE			(A_TSCOPE | A_RSCOPE)	/* All possible scopes */
#define A_DSFLAGS       (A_SCOPE | A_TRIGGER | A_ATS | A_RIS | A_NOCOLSKIP | \
						A_COLSKIP | A_IGNOREDELETES |A_SEQ |A_SHADOWREPL | \
						A_REPL4RESYNC)		/* All possible dsFlags */
											/* should go here */
/*#define CRFLAGMASK      0x3602FF00U */ /* Mask out all but these flags */

#define CRFLAGMASK      (A_COLSKIP|A_NOCOLSKIP|\
                         A_IGNOREDELETES|A_SHADOWREPL|A_REPL4RESYNC|\
                         A_NTRIGGER|A_NATS|A_NRIS|A_TRIGGER|\
                         A_ATS|A_RIS|A_RSCOPE|A_TSCOPE)

/* conflict resolution rules */
#define R_TIMESTAMP		0x01U
#define R_STOREDPROC	0x02U
#define R_IGNORE		0x04U
#define R_DEFAULT		0x04U
#define R_ALWAYSAPPLY	0x08U
#define R_MANUAL		0x10U
#define R_DELETEWINS	0x20U

/* All possible conflict rules */
#define R_CRRULES (R_TIMESTAMP | R_STOREDPROC | R_IGNORE |R_ALWAYSAPPLY | R_DELETEWINS)

/* stored procedure options */
#define R_STOREDPROC_OPTM	0x01	/* optimized option */
#define R_STOREDPROC_EXEC	0x02	/* always execute option */

/* server attribute flags */
#define A_IDLE			0x01U
#define A_SENDQ			0x02U	/* obsolete */
#define A_RECVQ			0x04U	/* obsolete */
#define A_ATSDIR		0x08U
#define A_RISDIR		0x10U
#define A_INIT			0x20U
#define A_SYNCSERV		0x40U
#define A_ATSRISXMLFMT	0x80U   /* XML format for ATS/RIS files */

/*
 * PLEASE NOTE: If we change value of A_LEAF we need to update dbv2k.h
 */
#define A_ROOT			0x0100U
#define A_NONROOT		0x0200U
#define A_LEAF		 	0x0400U
#define A_ROUTEMASK	(A_ROOT|A_NONROOT|A_LEAF)
#define A_ATSRISTXTFMT	0x0800U   /* text(default) format for ATS/RIS files */

#define A_SUSPENDAPPLY  0x1000U	/* DS-apply suspend/resume flag */
#define A_SUSPENDED     0x4000U /* A_SUSPENDED can never be set by api,
								   used on server recovery */
#define A_ATSRISFMTMASK	(A_ATSRISXMLFMT|A_ATSRISTXTFMT)
/*
 * Internal flag for server definition.
 */
#define A_INITCONNECT	0x8000U

#define	CDR_ALTER_ON	0x01
#define	CDR_ALTER_OFF	0x02

/* flags  for  forced spooling of queues  */
#define A_FORCE_SPOOL   0x01

/* Define to truncate number of charaters in name of template objects */
#define CDR_TEMPLATE_OBJECT_LEN 30

#define	CDR_MSGSIZEBIT	0x00004000
#define	CDR_MSGSIZEBIT_MASK		0xffffbfff
typedef struct plist
{
    char	serv[SVR_GL_NAMESIZE + 1];			/* server name */
    char	db[SVR_GL_NAMESIZE + 1];			/* database */
    char	table[SVR_GL_NAMESIZE + 1];			/* table */
    char	owner[CDR_GL_USERSIZE + 1];			/* owner */
    char	selectstmt[CDR_GL_SELECTSIZE + 1];	/* select stmt */
	uint4	partmode;							/* participant mode */
    struct plist	*part_next;					/* next participant */
} CDR_plist;

/*
 *  Sharding Flags saved in syscdr:shard_tab
 */

/* Shard Expression Flags */
#define SHARD_EXPR      0x00000001          
#define SHARD_HASH      0x00000002
#define SHARD_NOKEY     0x00000004
#define SHARD_NOSQL     0x00000008
#define SHARD_CHASH     0x00000010
/* Type of shard replication */
#define SHARD_INFO      0x00000100
#define SHARD_KEEP      0x00000200 
#define SHARD_DELETE    0x00000400
/* State of shard */
#define SHARD_PENDING   0x00010000
#define SHARD_DELETED   0x00020000

typedef struct _shardList
{
    int4    flags;                              /* see below */
    char    serv[SVR_GL_NAMESIZE + 1];          /* server name */
    int4    servID;                             /* server ID */
    int4    replID;                             /* replicate ID */
    char    value[CDR_GL_SELECTSIZE + 1];       /* value list */
    char    where[CDR_GL_SELECTSIZE + 1];
    struct  _shardList  *next;
}   CDR_shardList;

/*
 *  Flags for the individual replicates within the shard collection 
 */
#define SHARDFLAGS_SENDONLY     0x00000001      /* node must send */
#define SHARDFLAGS_EXISTING_NODE  0x00000002    /* old node of change shard */
#define SHARDFLAGS_REMAINDER    0x00000004      /* node is a remainder node */

#define SHARD_CHASH_RANGE  10000
typedef struct _chash
{
  char  serv[SVR_GL_NAMESIZE + 1];          /* server name */
  char  replica[SVR_GL_NAMESIZE + 10];  /* Replica name */
  int4  rval;  /* Hash value for this replica partition */
  int4  lval;  /* Next replica hash value */
  int4  hash_wrapped;
  struct _chash *chash_next;
} CDR_chash;

typedef struct _CDR_tablist
{
    char	db[SVR_GL_NAMESIZE + 1];	/* database */
    char	table[SVR_GL_NAMESIZE + 1];	/* table */
    char	owner[CDR_GL_USERSIZE + 1];	/* owner */
    int4	flags;						/* flags */
	struct	_CDR_tablist	*tab_next;
} CDR_tablist;

/* Values for flags. */
#define	CDR_GRPBLOCK	0x01	/* Block grouper evaluation. */
#define	CDR_NODELTABALT	0x02	/* Do not alter delete table as part of alter
								 * mode unset operation.
								 */
#define	CDR_CACHEUPDATED	0x04	/* GC Cache updated for this table */
#define CDR_ALTMODESET		0x08	/* GC has set alter mode for this table */

typedef struct freq
{
    uint2	hr;		/* hour 0-32767 */
    uint1	min;	/* minute 0-59 */
    uint1	type;	/* see frequency types below values */
    uint1	day;
    uint1	month;
} CDR_freq;

/* frequency types */
#define FREQ_IMMED			1 /* immediate */
#define FREQ_INTERVAL		2 /* every hour,minute */
#define	FREQ_TIME			3 /* at specified time */
#define FREQ_DAYOFWEEK		4 /* day=day of week (1-7) */
#define FREQ_DAYOFMONTH		5 /* day=day of month (1-31,'L') */
#define FREQ_UNDEFINED      6 /* Undefined for XReplsets */

typedef struct conf
{
	uint1	method;							/* conflict method */
	char	storedproc[CDR_GL_SPSIZE + 1];	/* stored procedure */
	uint1	storedprocopt;					/* stored procedure option */
} CDR_conf;

/*
 * The structure used by control messages to pass the extended attributes.
 * the first half are the 'on' bits and the second half are the 'off bits'
 */

#define XTD_ARRAY_SIZE 8
typedef struct extended_attrs
{
    uint4       flags[XTD_ARRAY_SIZE];
} CDR_repl_extended_attrs;

/*
 *  Events for cdr (cdr_events)
 */

#define EVENT_SHARD_DELETE_ROW      1
#define EVENT_SHARD_RESYNC          2
#define EVENT_SHARD_EXECUTE         3
#define EVENT_SHARD_SYNCEND         4
#define EVENT_SHARD_SYNCEND_ACK     5
#define EVENT_SHARD_SYNC_COMPLETE   6
#define EVENT_SHARD_REFRESH_CACHE   7
#define EVENT_SHARD_APPLY_ERROR     8

/*
 *  The CDR_repl_internal_extended_attrs are contained in the
 *  grouper and the datasync structures.  It consists of the bits from the
 *  syscdr sysxtdattr table and is set by the flags in the first half of the
 *  extended_attrs array.
 */
#define XTD_INTERNAL_ARRAY_SIZE (XTD_ARRAY_SIZE/2)
typedef struct _internal_extended_attrs
{
    uint4       flags[XTD_INTERNAL_ARRAY_SIZE];
}   CDR_repl_internal_extended_attrs;

#define XTD_EXT2INT(to,from)  \
    do  {                               \
        if  ((to) == NULL) break;       \
        if  ((from) == NULL) break;     \
        (to)->flags[0] = (from)->flags[0]; \
        (to)->flags[1] = (from)->flags[1]; \
        (to)->flags[2] = (from)->flags[2]; \
        (to)->flags[3] = (from)->flags[3]; \
        }  Once

#define XTD_FLAGS IDS_INT8(0x7FFFFFFF)
#define XTD_FLAG(Flag) (((bigint)(Flag)) & XTD_FLAGS)
#define XTD_SUB(Flag) (((bigint)(Flag)) >> 32)
#define SET_XTD_ATTRS(attr,Flag) \
        (attr).flags[XTD_SUB(Flag)] |= ((int4)XTD_FLAG((Flag)))
#define CLEAR_XTD_ATTRS(attr,Flag) \
        (attr).flags[XTD_SUB(Flag)] &= (~((int4)(XTD_FLAG((Flag)))))
#define CHECK_XTD_ATTRS(attr,Flag) \
        (attr).flags[XTD_SUB(Flag)] & ((int4)XTD_FLAG((Flag)))
#define XTD_VALUE(x,y) ((x)|(((bigint)y) << 32))
#define XTD_MERGE_FLAGS(o,n) \
    do  {                                                                   \
        mint    _sub;                                                       \
        if  ((o) == NULL) break;                                            \
        if  ((n) == NULL) break;                                            \
        for (_sub = 0; _sub < (XTD_ARRAY_SIZE / 2); _sub++)                 \
            {                                                               \
            (o)->flags[_sub] |= (n)->flags[_sub];                           \
            (o)->flags[_sub] &= (~(n)->flags[_sub + (XTD_ARRAY_SIZE / 2)]); \
            }                                                               \
        }   Once

#define XTD_ATTR1       0
#define XTD_ATTR2       1
#define XTD_ATTR3       2
#define XTD_ATTR4       3
#define XTD_OFF_ATTR1   4
#define XTD_OFF_ATTR2   5
#define XTD_OFF_ATTR3   6
#define XTD_OFF_ATTR4   7

/*
 *  Max for any group of extended attributes is 0x4FFFFFFF (31 bits)
 *  Since all attributes can be either on or off, the off
 *  attributes are stored in specific locations.  This way
 *  in the code, we can explicitly check for the attribute.
 *  N.B. we have to be careful not to generate a value of 0x80000000
 *  as that is a NULL integer.
 */

#define A_XTD_UNICODE       XTD_VALUE(0x0000001,XTD_ATTR1)
#define A_XTD_NOUNICODE     XTD_VALUE(0x0000001,XTD_OFF_ATTR1)

#define A_XTD_TIMESERIES    XTD_VALUE(0x0000002,XTD_ATTR1)
#define A_XTD_NOTIMESERIES  XTD_VALUE(0x0000002,XTD_OFF_ATTR1)

#define A_XTD_SERIAL_APPLY  XTD_VALUE(0x0000004,XTD_ATTR1)
#define A_XTD_NOSERIAL_APPLY XTD_VALUE(0x000004,XTD_OFF_ATTR1)

#define A_XTD_USEUNIQUE_KEY XTD_VALUE(0x0000010,XTD_ATTR1)

#define A_XTD_ALWAYS_REPLOBS   XTD_VALUE(0x0000020,XTD_ATTR1)
#define A_XTD_NOALWAYS_REPLOBS XTD_VALUE(0x0000020,XTD_OFF_ATTR1)

#define A_XTD_SHARD_REPL       XTD_VALUE(0x0000040,XTD_ATTR1)
#define A_XTD_NOSHARD_REPL     XTD_VALUE(0x0000040,XTD_OFF_ATTR1)
#define A_XTD_SHARD_DELETE     XTD_VALUE(0x0000080,XTD_ATTR1)
#define A_XTD_NOSHARD_DELETE   XTD_VALUE(0x0000080,XTD_OFF_ATTR1)
#define A_XTD_SHARD_INFO       XTD_VALUE(0x0000100,XTD_ATTR1)
#define A_XTD_NOSHARD_INFO     XTD_VALUE(0x0000100,XTD_OFF_ATTR1)

#define A_XTD_EVENT_REPL       XTD_VALUE(0x0000200,XTD_ATTR1)
#define A_XTD_NOEVENT_REPL     XTD_VALUE(0x0000200,XTD_OFF_ATTR1)
        /* shard replicate for hash-based sharding */
#define A_XTD_SHARD_HASH       XTD_VALUE(0x0000400,XTD_ATTR1)
#define A_XTD_NOSHARD_HASH     XTD_VALUE(0x0000400,XTD_OFFATTR1)

#define A_XTD_SHARD_CHASH       XTD_VALUE(0x0000800,XTD_ATTR1)
#define A_XTD_NOSHARD_CHASH     XTD_VALUE(0x0000800,XTD_OFFATTR1)

#define A_XTD_TESTFLAG      XTD_VALUE(0x40000000,XTD_ATTR4)
#define A_XTD_NOTESTFLAG    XTD_VALUE(0x40000000,XTD_OFF_ATTR4)
#ifdef XTD_PRINT_TESTMODE   /* turn on only when special build testing */
#define XTD_PRINT_TESTFLAG(x)                                           \
    do  {                                                               \
        mt_logprintf("%s:%d A_XTD_TESTFLAG %s set",                  \
            __FILE__, __LINE__,                               \
            (CHECK_XTD_ATTRS((x), A_XTD_TESTFLAG) ? "is" : "is not"));  \
        }   Once
#else
#define XTD_PRINT_TESTFLAG(x)
#endif

typedef struct idlist
{
	char			id[SVR_GL_NAMESIZE+1];	/* identifier */
	struct idlist*	id_next;				/* next identifier */
} CDR_idlist;

typedef struct repl_attrs
{
    uint4       flags;      /* replicate attributes */
    CDR_freq*   freq;       /* freq tx's sent */
    CDR_conf*   primary;    /* primary method*/
    CDR_conf*   secondary;  /* secondary method */
    CDR_idlist* keyList;    /* key(s) for the replicate */
#ifdef TURBO
    CDR_repl_extended_attrs *xtdAttrs;
#endif
} CDR_repl_attrs;

typedef struct serv_attrs
{
	uint2		sflags;						/* definition flags */
	uint2		idle;						/* connection time-out (minutes)*/
	char		atsDir[CDR_DIRNAMESIZE+1];	/*  ATS spool dir				*/
	char		risDir[CDR_DIRNAMESIZE+1];	/*  RIS spool dir				*/
	char		syncServ[SVR_GL_NAMESIZE+1];/* catalog sync server name		*/
	CDR_freq	*freq;						/* batch frequency */
} CDR_serv_attrs;

#define 	RSNC_TGTROW_DELETE	0x0001	/* Delete the tgt. only rows  */
#define 	RSNC_TGTROW_KEEP	0x0002	/* Keep the tgt. only rows */
#define 	RSNC_TGTROW_MERGE	0x0004	/* replicate the tgt. only rows */
#define 	RSNC_CHECKONLY		0x0008	/* Just do the checking of the data */
#define 	RSNC_REPLICATE		0x0010	/* The job is for a replicate */
#define 	RSNC_REPLSET		0x0020	/* The job is for a replicate set */
#define 	RSNC_FULLSYNC		0x0040	/* This job is for initial/full synch */
#define 	RSNC_DELETEONEND	0x0080	/* Delete this job after completion */
#define 	RSNC_CASCADEDEL		0x0100	/* Delete this job after completion */
#define 	RSNC_USERFILTER		0x0200	/* Job has user defined select and
                                           where clauses */
#define     RSNC_EXTERNAL		0x0400	/* This is an external sync/resync */
#define     RSNC_FOREGROUND		0x0800	/* This is a foreground sync */

/*
 * Structure needed for resynch jobs
 */
typedef struct resynch_attrs
{
	uint4	flags;                      /* Resync attributes */
	int2	blocksize;					/* #of rows per resync block */
	int4	syncMemAdjust;				/* for foreground sync operation */
	char	objname[CDR_GL_NAMESIZE+1]; /* replicate/replicateset name */
	char	srcnode[SVR_GL_NAMESIZE+1]; /* source serv for the resync job */
	char	connsvr[SVR_GL_NAMESIZE+1]; /* Connect server */
} CDR_resynch_attrs;

/*
 * Structures needed for Define Templates
 */

typedef struct _CDR_templateTable	{
	struct	_CDR_templateTable *next;
    char                     replname[CDR_GL_NAMESIZE+1];
    void		             *replMast;
    CDR_idlist               *keyList;
	char					 owner[CDR_GL_USERSIZE + 3]; /* name + quotes */
	char					 table[SVR_GL_NAMESIZE + 1];
    mint                     usesAnyUniqueKey;
    mint                     hasTSColumn;
    } CDR_templateTable;

typedef struct _CDR_template {
    int4                templateID;
	CDR_repl_attrs		*repl_attrs;
    CDR_repl_extended_attrs *xtd_attrs;
	int4				replset_flags;
    char                template_name[CDR_GL_NAMESIZE+1];
    char	            serv[SVR_GL_NAMESIZE + 1];
    char                dbname[CDR_GL_NAMESIZE+1];
    int2                dbflags;
    CDR_templateTable   *tables;
    } CDR_template;

/*
 * Structures needed for Realize Templates
 */

typedef struct _CDR_templatePart {
    uint4                    partmode;                  /* participant mode */
	char					 database[SVR_GL_NAMESIZE+1]; /* database */
    char                     serv[SVR_GL_NAMESIZE + 1]; /* Server name      */
    struct _CDR_templatePart *next;
    } CDR_templatePart;

typedef struct _CDR_realizeTemplate {
	char				template_name[CDR_GL_NAMESIZE+1];
	uint4				flags;
	int4				syncMemAdjust;
	char				dbspace[CDR_GL_NAMESIZE+1];
	char				syncdatasource[CDR_GL_NAMESIZE+1];
	CDR_templatePart	*part;
	}	CDR_realizeTemplate;

typedef void (*CDR_msgfunc_t)  (char *);

typedef struct _CDR_check {
	char				name[CDR_GL_NAMESIZE+1];	/* object name */
	uint4				flags;
	char				master[CDR_GL_NAMESIZE+1];	/* master node */
	char				*connServer;				/* -c server */
	CDR_idlist			*target;					/* list of targets */
	mint				maxLobCheckSize;			/* amount of lob to check */
	mint				maxRowsInTxn;				/* max Txn Size */
	mint				jobID;						/* token job id */
	mint				resyncJobId;				/* internal job id */
	int4				syncMemAdjust;				/* adjustment for SENDQ */
    int4                inflightRetryTime;          /* retry time */
    int4                maxFork;           /* max number of forked subtasks */
    int4                cmdtimeout;   /* max number seconds to wait for
                                       * server state to be changed from
                                       * disabled to active state. */
	char				jobName[CDR_GL_NAMESIZE+1];		/* name of this job */
    char                *whereFilter;               /* additional where clause*/
    time_t              sinceTime;                  /* check since this time */
	char				firetrig[20];	            /* master node */
	char				checksum[CDR_GL_NAMESIZE+1];		/* checksum UDR */
	}	CDR_Check;

#define CHECK_Replset		0x0001		/* object is a set */
#define CHECK_AllNodes		0x0002		/* Check all nodes */
#define CHECK_Repair		0x0004		/* Perform repair on tables */
#define CHECK_Verbose       0x0008      /* print all inconsistancies */
#define CHECK_KeepExtra     0x0010      /* Keep extra rows on target */
#define CHECK_MergeExtra    0x0020      /* Replicate extra rows on target */
#define CHECK_DisableRI     0x0040      /* Disable RI constraints */
#define CHECK_FullSync      0x0080      /* Perform full sync */
#define CHECK_SYNC_Progress 0x0100      /* Perform full sync */
#define CHECK_SkipLOBs      0x0200      /* Skip LOBS in check */
#define CHECK_Background    0x0400      /* Perform background task */
#define CHECK_Restarted     0x0800      /* Restarted Task */
#define CHECK_Timestamp     0x1000      /* Perform timestamp comparison */
#define CHECK_Deletwins     0x2000      /* Enable deletwins in timestamp repair */
#define CHECK_AllRepls      0x4000      /* Check all replicates for the given participants */
#define CHECK_EnableServ    0x8000      /* Enable participants if they are
                                         * in disabled state */
#define CHECK_DeleteUniq    0x010000    /* delete if unique col conflict */
#define CHECK_NoMaster      0x020000
#define CHECK_ExcludeTS     0x040000    /* don't check any TimeSeries columns */
#define CHECK_IgnoreHiddenTSElements \
                            0x080000    /* ignore hidden TimeSeries elements */
#define CHECK_IgnoreParentUpd    0x100000     /* skip cascadeUpdate() */
#define CHECK_ReplStateOnly      0x200000     /* Check replication metadata state */

/* For shadowReplFlags */

/* Disable triggers for shadow replicate */
#define FLAG_4_SHADOW_DISABLE_FIRETRIG    0x00000001
/*
 * Always enable triggers for shadow replicate even if base replicate
 * do not have triggers enabled.
 */
#define FLAG_4_SHADOW_ENABLE_FIRETRIG    0x00000002
/*
 * Enable triggers for shadow replicate if base replicate has triggers enabled
 */
#define FLAG_4_SHADOW_FIRETRIG_FALLOW_BASEREPL    0x00000004

/*
 * participate group operators
 */
#define A_ADD			1U
#define A_REMOVE		2U
#define A_CREATE		4U		/* mastered replicates only - create tables */
#define A_VERIFY		8U		/* mastered replicates only - verify tables */
/*
 *	options during sync of template
 */
#define CDR_RT_DeleteExtraRows	0x00000010		/* delete extra rows */
#define CDR_RT_MergeExtraRows	0x00000020		/* merge extra rows */
#define CDR_RT_IgnoreExtraRows	0x00000040		/* ignore extra rows */
#define CDR_RT_ForegroundSync   0x00000080      /* perform foreground sync */
/*
 *	mode flags for template
 */
#define CDR_RT_TargetOnly		0x00000100		/* replication is target only */
#define CDR_RT_ApplyAsInformix  0x00000200		/* apply data as informix */
#define CDR_RT_SendOnly         0x00000400      /* perform as send only */

/*
 * template verify/create option flags
 */
#define CDR_RT_Verify			0x00010000		/* verify only */
#define CDR_RT_AutoCreate		0x00020000		/* create missing tables */
#define CDR_RT_DatabaseCreate   0x00040000      /* create database if needed */

#define ReplDictCRCOLS		    0x0001
#define ReplDictRowLocks	    0x0002
#define ReplDictNeedCRCOLS      0x0004
#define ReplDictNeedOffsets     0x0008
#define ReplDictHasDroppedCol   0x0010
#define ReplDictHasReplCheckCol 0x0020    /* replcheck shadow columns */
#define ReplDictHasERKeyCol     0x0040    /* ER key shadow columns    */
#define ReplDictUTF8            0x0080    /* names stored in UTF8 format */
#define ReplDictUsingUniqueIdx  0x0100    /* UniqueIndex is the key */
#define ReplDictHasRowVer       0x0200  
#define ReplDictHasNoSQLCollection   0x0400  /* NOSQL collection table */

/*
 *  cdr trigger types...
 */
#define CDR_INSERT_TRIGGER		1
#define CDR_UPDATE_TRIGGER		2
#define CDR_DELETE_TRIGGER		3
#define CDR_POST_COMMIT_TRIGGER 4
#define CDR_POST_FAILED_TRIGGER 5

#ifndef TURBO
/*
 * Used to make the current CLI and API work with old versions of CDR/SERVER.
 * Value of CDRversion                   Server Version(s)
 * -------------------                   -----------------
 *       1                               7.2x
 *       2                               7.3
 *       3                               7.31 and 9.20
 *       4                               9.30, 9.40, 10.00 and 11.10
 */
extern int CDRversion;

extern char	*SaveVerb;
extern char *SaveObj;
extern mint SaveVerifyOnly;
extern mint ErrorPostCnt;
extern char *ErrorPostID;

/*
 * If this variable value is set to TRUE then cache the GLS descriptor
 * information instead of loading evrytime in cdr_gl_init() for each API call.
 */
int CdrCacheGLSDesc;

CDRAPI_EXP_DECL int cdr_define_repl(const char *, const CDR_repl_attrs *,
                           const  CDR_repl_extended_attrs*, uint4,
						   CDR_plist *, void **);
CDRAPI_EXP_DECL int cdr_check(CDR_Check *);
CDRAPI_EXP_DECL int cdr_define_shadow_repl( char *,  char *,  CDR_repl_attrs *,
					CDR_repl_extended_attrs*, uint4, CDR_plist *,
                                        void *, void **);
CDRAPI_EXP_DECL int cdr_modify_repl(const char *,
                    const CDR_repl_attrs *, const CDR_repl_extended_attrs *);
CDRAPI_EXP_DECL int cdr_start_repl( char *,CDR_idlist *, time_t, void **);
int cdr_start_repl_int( char *,CDR_idlist *, time_t,  CDR_resynch_attrs *,void **);
CDRAPI_EXP_DECL int cdr_stop_repl( char *,CDR_idlist *, time_t, void **);
CDRAPI_EXP_DECL int cdr_delete_repl( char *, time_t);
CDRAPI_EXP_DECL int cdr_suspend_repl ( char*, time_t);
CDRAPI_EXP_DECL int cdr_resume_repl ( char*, time_t);
CDRAPI_EXP_DECL int cdr_modify_replmode( char *,CDR_plist *, void **);
CDRAPI_EXP_DECL int cdr_define_serv( char *, CDR_serv_attrs *);
CDRAPI_EXP_DECL int cdr_define_serv_v1( char *, CDR_serv_attrs *,
			 char *,  char *);
CDRAPI_EXP_DECL int cdr_modify_serv( char *, CDR_serv_attrs *);
CDRAPI_EXP_DECL int cdr_delete_serv( char *, mint force, mint *okToForceDel);
CDRAPI_EXP_DECL int cdr_suspend_serv( char *,CDR_idlist *, time_t, void **);
CDRAPI_EXP_DECL int cdr_resume_serv( char *,CDR_idlist *, time_t, void **);
CDRAPI_EXP_DECL int cdr_modify_servmode( char *, uint4);
CDRAPI_EXP_DECL int cdr_participate_replset( char *, uint1, CDR_idlist*, void**);
CDRAPI_EXP_DECL int cdr_participate_repl( char*, uint4, char*, CDR_plist*, void**);
CDRAPI_EXP_DECL int cdr_define_replset( char *, uint4,CDR_idlist*,
							 CDR_freq*, void**);
CDRAPI_EXP_DECL int cdr_delete_replset( char *, time_t);
CDRAPI_EXP_DEFN int
cdr_participate_repl_int(
    char        *repl,
    uint1        op,
    CDR_plist   *participants,
    void        **which
                     );

CDRAPI_EXP_DECL int cdr_delete_replset_int( char *, time_t);
CDRAPI_EXP_DECL int cdr_start_replset( char *,CDR_idlist*, time_t, void**);
CDRAPI_EXP_DECL int cdr_stop_replset( char *,CDR_idlist*, time_t, void**);
CDRAPI_EXP_DECL int cdr_suspend_replset( char *, time_t);
CDRAPI_EXP_DECL int cdr_resume_replset( char *, time_t);
CDRAPI_EXP_DECL int cdr_modify_replset( char *, uint4,
                CDR_repl_extended_attrs*,  CDR_freq*);
CDRAPI_EXP_DECL int cdr_prune_errors(dtime_t *, dtime_t *, int);
CDRAPI_EXP_DECL int cdr_prune_single_error( char *, long);
CDRAPI_EXP_DECL int cdr_error_reviewed( char *, long);
CDRAPI_EXP_DECL int cdr_move_errortab( char *, int);
CDRAPI_EXP_DECL int cdr_connect(char *, char*);
CDRAPI_EXP_DECL int cdr_connect_useoriglocale(char *, char*);
CDRAPI_EXP_DECL void cdr_closeAllConnections();
CDRAPI_EXP_DECL int cdr_open_database(char*);
CDRAPI_EXP_DECL int cdr_open_database_lockwait(char*);
CDRAPI_EXP_DECL int cdr_open_database_orgiloc_lkwait(char*);
CDRAPI_EXP_DECL int cdr_connect_to_serv(char*);
CDRAPI_EXP_DECL char* cdr_getCurrentServer();
CDRAPI_EXP_DECL int cdr_start_cdr(void);
CDRAPI_EXP_DECL int cdr_continue_cdr(void);
CDRAPI_EXP_DECL int cdr_stop_cdr(void);
CDRAPI_EXP_DECL int cdr_stop_cdr2(uint4);
CDRAPI_EXP_DECL int cdr_connect_serv( char *);
CDRAPI_EXP_DECL int cdr_disconnect_serv( char *);
CDRAPI_EXP_DECL int cdr_sync_checksum( char *,  char *, int);
CDRAPI_EXP_DECL int cdr_suspend_apply( char *, uint4);
CDRAPI_EXP_DECL int cdr_swap_shadow(char *, uint4, char *, uint4);

CDRAPI_EXP_DECL int cdr_define_resynch( char *,  CDR_resynch_attrs *,
                               CDR_idlist *,  CDR_plist *);
CDRAPI_EXP_DECL int cdr_delete_resynch( char*,  char*,  char*);
CDRAPI_EXP_DECL int cdr_start_resynch( char*);
CDRAPI_EXP_DECL int cdr_stop_resynch( char*);

CDRAPI_EXP_DECL int cdr_define_template(CDR_template*, char**);
CDRAPI_EXP_DECL int cdr_realize_template(CDR_realizeTemplate*, char*);
CDRAPI_EXP_DECL int cdr_delete_template( char*);

CDRAPI_EXP_DECL int cdr_define_trigger(int4,  char*,  char*,
		 CDR_idlist*);
CDRAPI_EXP_DECL int cdr_delete_trigger( char*,  char*);
CDRAPI_EXP_DECL mint cdr_alter_mode(int, CDR_tablist*, void **);
CDRAPI_EXP_DECL char* cdr_find_error(int);
CDRAPI_EXP_DEFN int cdr_remove_cdr(void);
CDRAPI_EXP_DEFN int cdr_cleanstart_cdr(void);
CDRAPI_EXP_DEFN mint cdr_remaster_repl(char *, char *, char *);
CDRAPI_EXP_DEFN int cdr_start_replset_int( char *, CDR_resynch_attrs *, CDR_idlist *, time_t, void **);
CDRAPI_EXP_DECL void  cdr_register_msgfunc(CDR_msgfunc_t);
CDRAPI_EXP_DECL char  *cdr_getJobState(long);
CDRAPI_EXP_DECL void  cdr_ConnectionFailed(char*, char*, mint);
CDRAPI_EXP_DECL char* cdrfmtmsg(char *fmt, ...);
CDRAPI_EXP_DECL int cdr_disable_serv(char*);
CDRAPI_EXP_DECL int cdr_enable_serv(char*);
CDRAPI_EXP_DEFN int cdr_autoconfig_server(char*, char*, char*, char*);


#endif /* TURBO */

#ifdef TURBO
#define CDRDEBUG(x,y,z)
#define CDRTRACE(x)
#define CDR_POSTERROR(x,y)
#else
CDRAPI_EXP_DECL mint cdr_post_error( mint  errorcode, char  *text);

#define CDRDEBUG(x,y,z) \
    cdrdebug(__FILE__, __LINE__, (x), (y), (z))

#define CDRTRACE(x) \
	cdrtrace(__FILE__,__LINE__, cdrfmtmsg x);
#define CDR_POSTERROR(x,y) \
	do	{								\
		char		buff[500];			\
		strcpy(buff, cdrfmtmsg y);		\
	    cdr_post_error(x, buff);	} while (0)
#endif

extern void * dictMalloc(int);
extern void * dictRealloc(void *, int);
extern void dictFree(void *);
extern void cdrdebug (char *file, int lineno, const char *msg,
        const char *buff, int size);
CDRAPI_EXP_DEFN mint
cdr_post_error(
    mint        errorcode,
    char        *text);
extern char* trim(char*);

/* Really should have an sql/network include prototype */

extern mint ifx_iputint(mint);
extern mint ifx_iputlong(mint);
extern mint ifx_iputunsigned(muint);
extern mint ifx_iwrite(char*, muint);
extern mint ifx_iflushbuff();
extern mint ifx_igetint();

/* The CDR_Config structure is used
 * -  to normalize the configuration command request.
 */
typedef struct CDR_Config {
	unsigned int optionFlags;
	int operation;
	char* onConfigLine;
} CDR_Config_t;

#ifndef CDRZAP
#define CDRZAP(x)  memset(&(x), 0, sizeof((x)))
#endif

/* onconfig operation identifiers */
#define CDR_ADD_CONFIG 		1 	/* for ADD operation */
#define CDR_REMOVE_CONFIG	2  	/* for REMOVE operation */
#define CDR_CHANGE_CONFIG	3  	/* for CHANGE operation */
#define CDR_MAX_CONFIG_OPERATION	CDR_CHANGE_CONFIG

CDRAPI_EXP_DECL int cdr_config_cmd (CDR_Config_t);

typedef enum {
    gridtable_noError = 0,
    gridtable_hasTS = -1,
    gridtable_isExternal = -2,
    gridtable_isView = -3,
    gridtable_hasLBAC = -4,
    gridtable_isSynonym = -5
    }   gridtableError_td;

#endif /* CDRAPI_H */


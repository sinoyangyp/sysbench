/***************************************************************************
 *
 * Licensed Materials - Property of IBM
 *
 * Restricted Materials of IBM
 *
 * IBM Informix Dynamic Server
 * Copyright IBM Corporation 1996, 2012
 *
 * Title:       cdrerr.h
 * Description: return codes for CDR api
 *
 ***************************************************************************
 */

#ifndef CDRERR_DOT_H
#define CDRERR_DOT_H

/*
 WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING  W
 W                                                                        W
 W  See warning at end of this file                                       W
 W                                                                        W
 WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING  W
 */

extern char cdr_errmsgstr[];

#define CDR_SUCCESS		0	/* command successful */
#define CDR_ENOCONNECT	1	/* no connection for the specified server */
#define CDR_EOPENQUEUE	2	/* Can Not Open Queue */
#define CDR_ECOLUNDEF	3	/* table column undefined */
#define CDR_ECOMPAT		4	/* incompatible server version */
#define CDR_ECONNECT	5	/* unable to connect to server specified */
#define CDR_EDBDNE		6	/* database does not exist */
#define CDR_EDBLOG		7	/* database not logged */
#define CDR_EFREQ		8	/* Bad or mismatched frequency attributes */
#define CDR_ECONNECTED	9	/* already connected to specified server */
#define CDR_EGRPSTATE	10	/* illegal group state change */
#define CDR_EGRPUNDEF	11	/* undefined replicateset */
#define CDR_ENAMEUNIQ	12	/* name already in use */
#define CDR_EIDLE		13	/* invalid idle time */
#define CDR_EINVOP		14	/* invalid operator or specifier */
#define CDR_ELEN		15	/* future use */
#define CDR_EGRPNOPART	16	/* replication not participating in group */
#define CDR_ENOPART		17	/* participants required for operation specified */
#define CDR_ENOPKEY		18	/* table does not contain primary key */
#define CDR_ENOERKEY    CDR_ENOPKEY
#define CDR_EOWNER		19	/* table does not exist */
#define CDR_EPACTIVE	20	/* server already participating in replicate */
#define CDR_ETIMEOUT	21	/* command timed out */
#define CDR_EPKEYSELCT	22	/* primary key not contained in select clause */
#define CDR_EPARTINGRP	23	/* Already a member of the replset */
#define CDR_ERECVQ		24	/* Dbspace for receive queue does not exist */
#define CDR_EREPACTIVE	25	/* replicate already participating in a group */
#define CDR_EREPDEF		26	/* group operation not permitted on replicate */
#define CDR_EREPLSYN	27	/* future use */
#define CDR_EREPLUNIQ	28	/* replicate name already in use */
#define CDR_ETBLDNE		29	/* table does not exist */
#define CDR_EREPSTATE	30	/* illegal replicate state change */
#define CDR_EREPUNDEF	31	/* undefined replicate */
#define CDR_ESENDQ		32	/* dbspace specified for the send queue does
								not exist */
#define CDR_ESERVDEF	33	/* server not participant in replicate/group */
#define CDR_ESERVMAX	34	/* not used */
#define CDR_ESERVRESOLV	35	/* server not defined in sqlhosts */
#define CDR_ESERVSET	36	/* disjoint servers for replicates */
#define CDR_ESERVUNDEF	37	/* undefined server */
#define CDR_ESPDNE		38	/* stored procedure does not exist */
#define CDR_ESQLSYN		39	/* illegal select syntax */
#define CDR_ESQLUNSUP	40	/* unsupported SQL syntax (join, etc..) */
#define CDR_ECODESET		41	/* Cannot find codeset conversion table from/to UTF8 */
#define CDR_ETIME		42	/* invalid time */
#define CDR_EVALID		43	/* participants required for specified operation */
#define CDR_ENAMERR		44	/* illegal name syntax */
#define CDR_EPART		45	/* invalid participant */
#define CDR_EREPL		46	/* future use */
#define CDR_ESERV		47	/* invalid server */
#define CDR_ENOMEM		48	/* out of memory */
#define CDR_EREPMAX		49	/* maximum number of replicates exceeded */
#define CDR_EPARTMAX	50	/* maximum participants */
#define CDR_ERUMOR		51	/* attempt to delete server remotely */
#define CDR_ESERVUNIQ	52	/* server name already in use */
#define CDR_EDUPL		53	/* duplicate server or replicate */
#define CDR_EBADCRULE	54	/* Bad conflict rule specified */
#define CDR_ENOSCOPE	55	/* Resolution scope not specified */
#define CDR_ESCOLSDNE	56	/* Shadow columns do not exist for table */
#define CDR_ECRDELTAB	57	/* Error creating delete table */
#define CDR_ENOCRULE	58	/* No conflict resolution rule specified */
#define CDR_EBADSCOLS	59	/* Table has badtype for shadow columns or has
								shadow columns at wrong place */
#define CDR_EGRPPART	60	/* illegal operation on group participant */
#define CDR_ENOPERM		61	/* user doesn't have permission to issue command */
#define CDR_ENOCDR		62	/* CDR not active */
#define CDR_ECDR		63	/* CDR already active */
#define CDR_ENOSYNC		64	/* remote/cyclic synchronization not allowed */
#define CDR_ESERVID		65	/* server identifier already in use */
#define CDR_ENOTIME		66	/* No upper time for prune error */
#define CDR_ERRNOTFOUND	67	/* Error not found for delete or update */
#define CDR_EPARTMODE	68	/* illegal participant mode */
#define CDR_ECONFLICT	69	/* conflict mode for replicate not ignore */
#define CDR_ECONSAME	70	/* connect/disconnect to/from same server */
#define CDR_EROOT		71	/* conflicting root server flags */
#define CDR_EPARENT		72	/* cannot delete server with children */
#define CDR_ESPAROOT	73	/* leaf-root configuration not allowed */
#define CDR_ELIMITED	75	/* request denied on limited server */
#define CDR_EMSGFORMAT	76	/* unsupported message format */
#define CDR_EDROPDB		77	/* couldn't drop syscdr database (sqerr -425) */
#define CDR_EATSDIR		78	/* ATS directory does not exist */
#define CDR_ERISDIR		79	/* RIS directory does not exist */
#define CDR_ECRCHANGE	80	/* Illegal Conflict resolution change */
#define CDR_EUDTBADCOL  81  /* UDT collection types not allowed */
#define CDR_EUDTEXPR	82	/* UDTs not allowed in expressions
							   (such as where clauses) */
#define CDR_ENOUDTPKEY	83	/* No UDTs in primary key allowed */
#define CDR_ESYNC		84	/* No sync server with non-root & leaf */
#define CDR_EPARTFLAGS  85	/* incorrect participant flags */
#define CDR_ELEAF       86  /* conflicting leaf server flags or
							   attempt to sync with leaf server */
#define CDR_ESTOPFLAGS  87  /* invalid cdr stop options */
#define CDR_EMODRECVQ	89	/* cannot modify dbspace for queues */
#define CDR_ECLOCKSKEW	90  /* system clocks are out of synchronizaton */
#define CDR_ESERVSTATE	91	/* invalid server state transition */
#define CDR_EALRDYDEF	92	/* CDR server already defined */
#define CDR_ESTARTUP    93  /* CDR is starting */
#define CDR_ESHUTDOWN   94  /* CDR is shutting down */
#define CDR_USAGE       99  /* Exit code for usage messages */

#define CDR_ESERVERR	100	/* fatal server error */
#define CDR_ENOSUPPORT	101	/* unsupported feature */
#define CDR_EINVSYNC	102	/* Root server can not sync with non
							 * root or leaf servers */
#define CDR_EINVCONNECT	103	/* Invalid server to connect */
#define CDR_ETEMPDB		104	/* Can not use temp dbspaces for Send/Recv
							 * queues and error tables */
#define CDR_EUDRNOTFOUND 105 /* A UDR needed for replication was not found */
#define CDR_EUDRSETUP	106	/* Setup necessary for UDR invocation could not
							 * be completed */
#define CDR_ESBSPACE	107	/* There was an error related to the smart blob */
							/* space specified for the send queue */
#define CDR_EDBSPACE	108	/* There was an error related to the dbspace */
							/* specified for the catalog */
#define CDR_EMODSPACE	109	/* server dbspace can not be modified */
#define CDR_ENOMRINWHERE 110 /* Multirep types are not allowed in the where
							  * clause of a replicate.  */
#define CDR_EFULLROWSPCR 111 /* Cannot have fullrow off and use stored
                              * procedure conflict resolution. */
#define CDR_EPARTIAL    112 /* ReplSet operation partially applied */
#define CDR_EXREPLSET	113 /* Exclusive Replset Violation */
#define CDR_ESERVQUISENT 114   /* Server is in quiescent state */
#define CDR_ENOSYSCDR 115   	/* The syscdr database is missing! */
#define CDR_ENOSYSCDRSPACE	116	/* The syscdr dbspace is not correct */
#define CDR_ENOHDR 117		/* Enterprise Replication op attempted on
                             * HDR or Mach-11 secondary server */
#define CDR_ESQLHOSTS  118   /* Error found in the sqlhost file while processing*/
#define CDR_EHOSTOPTION  119   /* Error found in option (g=) or (i=) */
#define CDR_EREMOVE  120   /* cdr remove commad executed when ER is active */
#define CDR_ENOMASTER 121  /* master participant not found */

#define CDR_ESHADOWOP 122   /* Illegal shadow operation attempted */
#define CDR_ESHADOWPART 123    /* Attempt to add an invalid participant to a shadow replicate */
#define CDR_ECDRCMD		124   /* Invalid command passed to cdrcmd */
#define CDR_EMASTER	125		/* Mastered Replicate Error */

/* These definitions are to support template feature */
#define CDR_ETEMPLATEPART 126	/* invalid template participant */
#define CDR_ETEMPLATEUNIQ 127	/* template name already in use */
#define CDR_ETEMPLATEUNDEF 128	/* undefined template */
#define CDR_EREPLSETPARTOFTEMPLATE 129	/* can not delete replset part of template */
#define CDR_ENOSERVER 130 	/* Server name not specified */
#define CDR_ENOSYNCSERV 131	/* Sync server name not specified */
#define CDR_EINVSYNSERV 132	/* Invalid syncserver option specified */
#define CDR_EINVSYNOPT 133	/* Invalid extratargetrows option specified */
#define CDR_EXLOCK	134		/* Can not lock the replicated table in exclusive mode. See the ISAM error code in server log file.*/
#define CDR_NOTINALTMODE	135		/* Replicate is not in alter mode */
#define CDR_SNOOPYDOWN	136		/* Snoopy sub-component is down */
#define CDR_EMSTRVERIFY	137		/* Mismatch between local table dictionary and master dictionary. */
#define CDR_ETAB2REPLNOTFOUND	138	/* Replicate(s) not found for table. See message log file for more information */
#define CDR_ESCHK4SWPSHADOW	139	/* Sanity check failed for 'swap shadow' operation. See message log file for more information */
#define CDR_ESWAPPARTMISMATCH	140	/* Primary and shadow replicate participant verification failure. See message log file for more information */
#define CDR_EINALTMODE	141	/* Table is already in alter mode. See message log file for more information */
#define CDR_ECLASSICREPL	142	/* Classic replicate(s) (no mastered dictionary) found on the table. See message log file for more information */
#define CDR_ETRIGGERNF	144	/* can not find requested trigger */
#define CDR_ETRIGGEREXISTS 145 /* trigger of this name already exists */

/* This definition is here to support the SQL error code for Catalog Open*/
#define EOCATALOG       211 /* Error in Opening the Catalog */
#define CDR_ESYNCUNIQ   146   /* Resync job with the same name was defined*/
#define CDR_ESYNCSHADOW 147   /* Attemp to use shadow replicate as primary
                                  for resync */
#define CDR_ETGTPARAM   148   /* Invalid target parameter(s)to resync. job */
#define CDR_ENORSNC     149   /* Attempted resync with older  servers */
#define CDR_EINVJBST    150   /* Invalid job status */
#define CDR_ENOTSRC     151   /* Rsnc job can't start on a non-source server */
#define CDR_EINACTJOB   152   /* Job is not active */
#define CDR_ENOMULTTGT  153   /* Resynch.Job can not have more than on target*/
#define CDR_EINACTREPL  154   /* replicate of the resynch. job is not active */
#define CDR_EJOBUNDEF   155   /* Resynch job is not defined */

#define CDR_ENONAMEVERF 156   /* Since replicate is not defined with name verification option (--name y), auto remastering can not be done. */
#define CDR_EGRPRBLOCK 	157   /* Can not verify/block grouper evaluation blocking condition" */
#define CDR_EGRPRUNBLOCK 158  /* Can not unblock grouper evaluation */
#define CDR_EGRPR_ALREADY_BLOCK 159  /* CDR: Grouper evaluation already blocked in the same transaction. Commit the previous alter statement then execute the current alter statement. */
#define CDR_TEMPLATE_TABLEVIEW_ERR 160  /* Table not found */
#define CDR_TEMPLATE_NOFILE 161  /* File specified not found */
#define CDR_REPAIR_NOTXH 162  /* Repair could not find group entry in TXH in ATS/RIS file */
#define CDR_ALTER_TABVERIFYERR 163  /* Table verification failed */
#define CDR_EREPAIR 164  /* ATS/RIS repair failed */
#define CDR_EREPSUSP_REPJOB 165  /* Replicate/Replset cannot be suspended due
								    to the presence of repair jobs */
#define CDR_EEMPTY_RPLSET	166 /* Replset is empty */
#define CDR_ENOCDRINEXPRESS	167 /* IDS Express edition does not support ER */
#define CDR_ETABISVIEWERR	168 /* Table specified is a view */
#define CDR_EEMPTY_TEMPLATE	169 /* Template is empty */
#define CDR_UNKNOWNTEMPLATE	170 /* Template is not yet defined */
#define CDR_NOVERIFYCREATEALLOWED 171 /* Cannot use -u/-v with classic repl */
#define CDR_ENOCHECKSUM     172 /* checksum library not installed */
#define CDR_EXTSHUTDOWN		173 /* external sync shutdown requested */
#define CDR_EXTABORT		174 /* external sync abort requested */
#define CDR_EXTSTOP         175 /* external sync stop requested */
#define CDR_SYNCONINACTIVE  176 /* Attempted to sync an inactive repl */
#define CDR_UNKNDDL     177 /* Unknown DDL Statement */

#define CDR_EREPLOUTOFSYNC      178   /* Replicate Tables are out of sync */
#define CDR_EMASTERNOTINREPL    179   /* Master Node specified in not a participant */
#define CDR_ETARGETSNOTNREPL    180   /* None of the Target Nodes are participant of the Replicate */

#define CDR_ECFGVALNOTCHANGED       181   /* Config value for the specifed variable could not be set  */
#define CDR_ECFGVALADJUSTED         182   /* Config value for the specifed variable has been adjusted  */
#define CDR_ECFGUNSUPPORTEDOPRN     183   /* Unsupported modifying operation for the onconfig variable  */
#define CDR_ECFGWRONGFORMAT         184   /* Wrong format of the onconfig variable value */
#define  CDR_ECFGUNKNOWNVAR         185   /* Unsupported/Unknown ER onconfig/CDR_ENV variable */
#define  CDR_ECFGERDEFINED         	186   /* Cannot change the value while ER is defined */
#define  CDR_ECFGHDRDEFINED        	187   /* Cannot change the value while HDR is defined */
#define  CDR_WCFGVALSAME         	188   /* Value specified is same as that currently in-memory */
#define  CDR_ENOSUPPORT_LBAC       	189   /* Replicate cannot be defined on a LBAC protected table */
#define CDR_EREPAIR_DBCLICODESETS   190   /* Repair requires DB_LOCALE and CLIENT_LOCALE to have the same codesets */
#define CDR_ESERVIDINDET            191   /* Cannot determine connection server id for server. */
#define CDR_ESYSCDRSPARSE           192   /* Unable to find or connect to a 'syscdr' database at a non-leaf server */

#define CDR_ESQLRESOURCE	193	 	/* Qry failed due to resources */
#define CDR_ESQLCONNECT		194		/* Qry failed due to connection loss */
#define CDR_ESQLOTHER		195		/* Qry failed due to other resource  */

#define CDR_ESQLERR     196   /* Error code for SQL Errors */
#define CDR_ERSNCCRTAB  197   /* Error In creating control tables */
#define CDR_ERSNCCRTRG  198   /* Error In creating triggers */
#define CDR_ERSNCCRPROC 199   /* Error In creating resync procedures */

#define CDR_ESYNCERR    200   /* Unexpected Error in Sync or Check */
#define CDR_EBADCTYPE   201   /* Unexpected column type */
#define CDR_COLMISMATCH 202   /* Column types mismatch */
#define CDR_ENODATA     203   /* Column data missing */
#define CDR_ENOTABLE    204   /* Table not found SO WHY NOT CDR_ETBLDNE */
#define CDR_ESERVGRPUNDEF           205   /* Undefined server group */
#define CDR_ETEMPL_NOTREAL_AT_SYNCSRC 206 /* Template not realized at template sync source */
#define CDR_ETEMPL_REAL 207               /* Template already realized */
#define CDR_ESERVREMRESOLV 208/* server unknown at remote server */
#define CDR_EBADCHAR    209   /* A byte sequence that is not a valid
                               * character in the specified locale was
                               * encountered */
#define CDR_EBADARG     210   /* Parameter passed to command (or
                               * internally, routine) is invalid */
#define CDR_ETOOLARGE   211   /* background job string is too large */

#define CDR_SUBABORT    212   /* Sync/Check subTask aborted */
#define CDR_ESETOUTOFSYNC 213 /* Check set out of sync */
/* make sure error numbers match across server families */
#define CDR_EREPLAY     214   /* Invalid replay position detected */
#define CDR_ETABISEXTERNALERR   215 /* Table specified is an external table */
#define CDR_EDPARTIAL   		216   /* Command partially executed */
#define CDR_EQOD        		217   /* Error with Quality of Data command */
#define CDR_ESQLNOTFOUND 218    /* SQL query returned no rows */
#define CDR_ESERVLOCAL 219      /* Server parameter cannot refer to the
                                 * local server */

/* N.B. the difference between CDR_ESTASHUTDOWN and CDR_EXTASTOP is that
 * CDR_EXTASHUTDOWN is restartable
 */

/*
 *  Grid Errors
 */
#define CDR_EGRIDPART   220  /* node not a member of grid */
#define CDR_EGRIDUNIQ   221  /* grid name is not unique */
#define CDR_EGRIDUNDEF  222  /* grid is not defined */
#define CDR_EGRIDENU    223  /* grid enable user failed */
#define CDR_EGRIDENN    224  /* grid enable node failed */

#define CDR_ESEC2ER     225  /* sec2er failure */
#define CDR_ENETERR     226  /* A network error occurred */
#define CDR_EREGION     227  /* Region Operation Failed */
#define CDR_EGRIDTABLE  228  /* Grid Table Error */

#define CDR_ESHARD      229  /* sharding error */

/* The following errors provide more details on the error via
 * cdr_errmsgstr. All such errors should be included in the range
 * defined below  so  that we can output the full message to  the user.
 */
#define CDR_EMSGSTR_MIN 300
#define CDR_EMSGSTR_MAX 500

#define CDR_ETS CDR_ESQLUNSUP  /* error related to TimeSeries */

#define CDR_EMSGSTR_SIZE  1024 /* size of the error buffer */

/*
 WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING  W
 W                                                                        W
 W  When adding new error codes, use next available error number.  Do not W
 W  try to "plug gaps" in error numbers.   For example,  as I write this, W
 W  there is no error 92 and the largest error number is 110.  A new errorW
 W  should use 111, and not 92 (since that was probably an error that was W
 W  removed and there could be server versions out there that think 92    W
 W  means something).                                                     W
 W                                                                        W
 W  If you add new error code here, you may need to add it to             W
 W  gc_genSevErrMsg.  This is necessary if the error could go into the    W
 W  severe errors table (error could occur during an operation from a     W
 W  peer).                                                                W
 W  Please also add to the function cdr_find_error().                     W
 W                                                                        W
 WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING  W
 */

#endif /* CDRERR_DOT_H */

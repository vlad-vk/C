//------------------------------------------------------------------------------
extern struct {                        
       unsigned char    Name[DBSLEN];       // database file name
       int              Desk;               // descriptor (system number)
       int              Mod;                // modification flag
       int              Init;               // scope initialization flag
} dbFile[DBMAXDBF];


extern struct {
       long  RecCount;                      // number of records in the database
       int   RecLen;                        // length of one record
       long  CRec;                          // number of the current record in the database
       long  COfs;                          // started offset in file for current record
       int   Bof;                           // flag of attempt to leave the beginning of the database
       int   Eof;                           // flag of attempt to leave the end of the database
       int   FieldCount;                    // number of fields in open database
} dbHeaw[DBMAXDBF];

extern int   dbDebug;                       // debug flag

extern void  dbDPrn(unsigned char *, ...);  // debug output function

extern char  vkdbfver[80];                  // library version information
extern long  vkdbf_dcmp;
extern long  vkdbf_tcmp;
extern void  vkdbf_VERSION(void);

//------------------------------------------------------------------------------
// FUNCTIONS FOR WORKING WITH DATABASE FILES
//------------------------------------------------------------------------------
// determines whether the database is open in the specified area P1 [1-open, <0-no database]
extern int  dbOpenTest(int, unsigned char *);
//------------------------------------------------------------------------------
// checking the integrity of the database and the header, if necessary, adjusting it
// P1-area, P2-database filename, P3-mode (0=INFO, 1=CORR)
// [<0-database error, =0-file without errors, >0-error in file has been fixed]
extern int  dbCheck (int, unsigned char *, int);
//------------------------------------------------------------------------------
// creates db file P1 from dbField array [<0-db creation error]
extern int  dbCreate(int ,unsigned char *);
//------------------------------------------------------------------------------
// opens DB file P2 in the specified area P2 [<0-error opening DB]
extern int  dbOpen  (int, unsigned char *);
//------------------------------------------------------------------------------
// closes the database file in the specified area P1
extern void dbClose (int);
//------------------------------------------------------------------------------
// initially initializes (reserves) the database array
extern int  dbInit  (int);
//------------------------------------------------------------------------------
//  defining a free area for working with a database
extern int  dbSelectAvail(void);
//------------------------------------------------------------------------------
//  clear the specified db array
extern void dbClear(int);
//------------------------------------------------------------------------------
//  clear all database arrays
extern void dbClearALL(void);
//------------------------------------------------------------------------------
//  copy the P1 database structure to a new P2 file
//  [<0-error,0-no error]
extern int  dbStructCopy(unsigned char*, unsigned char*);
//------------------------------------------------------------------------------
// Overwrite database file P1 excludes records marked for deletion,
// in temporary file P2
extern int  dbPack(unsigned char*, unsigned char*);
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
// FUNCTIONS FOR WORKING WITH DATABASE FIELDS
//------------------------------------------------------------------------------
// determines whether array P2 is filled for region P1 [<0-array not filled]
extern int  dbFieldTest(int, int);
//------------------------------------------------------------------------------
// for area P1 clears array P2
extern void dbFieldClear(int, int);
//------------------------------------------------------------------------------
// for the area P1 fills the cell P2 of the array of fields with data about the field P2 of the database
// P3-field name, P4-field type('C','N','F','L','D','B','I'),
// P5 - field length, P6 - number of decimal places for fields of type 'N','F','B'
// [<0-parameter error for filling array of DB fields]
// After defining the last field using this function, you need to call
// the dbClear() function with the number of the next field.
extern int  dbFieldFill(int, int, unsigned char *,
                        unsigned char, unsigned char, unsigned char);
//------------------------------------------------------------------------------
// for area P1 and field number P2 return field name
extern unsigned char *dbFieldGetName(int, int);
//------------------------------------------------------------------------------
// for area P1 and field number P2 return field type
extern unsigned char  dbFieldGetType(int, int);
//------------------------------------------------------------------------------
// for area P1 and field number P2 return field length
extern unsigned char  dbFieldGetLen(int, int);
//------------------------------------------------------------------------------
// for area P1 and field number P2 return the number of digits after the decimal point
extern unsigned char  dbFieldGetDec(int, int);
//------------------------------------------------------------------------------
// [field number by name] 
// P1-area, P2-(0-case-insensitive,1-case-sensitive), P3-field name
extern int  dbFieldGetNumber(int, int, unsigned char *);
//------------------------------------------------------------------------------
// returns the number of database fields from the specified area
extern int  dbFieldCount(int);
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
// FUNCTIONS FOR WORKING WITH DATABASE RECORDS
//------------------------------------------------------------------------------
// for the area P1 set (logically) the cursor to the first record of the database
// [<0-no entries]
extern long dbGoFirst(int);
//------------------------------------------------------------------------------
// for the area P1 set (logically) the cursor to the last record of the database
// [<0-no entries]
extern long dbGoLast (int);
//------------------------------------------------------------------------------
// for the area P1 set (logically) the cursor to the specified record P2 of the database
// [<0 - outside the database]
extern long dbGoRec  (int, long);
//------------------------------------------------------------------------------
// for the P1 area, set (logically) the cursor to the +P2 database entry
// [<0 - outside the database]
extern long dbGoSkip (int, long);
//------------------------------------------------------------------------------
// [1-for this area P1 there was an attempt to go beyond the upper boundary of the database]
extern int  dbBof    (int);
//------------------------------------------------------------------------------
// [1-for this area P1 there was an attempt to go beyond the lower boundary of the database]
extern int  dbEof    (int);
//------------------------------------------------------------------------------
// for area P1 returns the number of records in the database
extern long dbRecCount(int);
//------------------------------------------------------------------------------
// [logical number of the current record for this area P1]
extern long dbRecCur (int);
//------------------------------------------------------------------------------
// adds an empty entry to the database in the current region P1
// [<0-invalid record length]
extern int  dbRecAdd (int);
//------------------------------------------------------------------------------
// write write buffer for region P1 to disk
extern int  dbRecWrite(int);
//------------------------------------------------------------------------------
// read write buffer for region P1 from disk
// [<0-record not read, >0-normal record(number of bytes read)]
extern int  dbRecRead(int);
//------------------------------------------------------------------------------
// copy write buffer from one area P1 to another area P2 [always 0]
extern int  dbRecCopy(int, int);
//------------------------------------------------------------------------------
// check if record is P2 (<0-current record,>0-record number)
// remote in the database open in area P1. [<0-error,0-not removed,1-deleted]
extern int  isRecDelete(int, long);
//------------------------------------------------------------------------------
// set record deletion sign P2 (<0-current record,>0-record number)
// remote in the database open in area P1. P3>0 - set the flag, P3=0 - remove the flag.
// [<0-error,0-not removed,1-deleted]
extern int  dbRecDelete(int, long, int);
//------------------------------------------------------------------------------
// check of record deletion sign for area P1
// [=0-deletion flag not set, >0-record deletion flag set]
extern int  isRecDFL(int);
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
// FUNCTIONS FOR WRITING AND READING DATABASE FIELD VALUES
//------------------------------------------------------------------------------
// the last parameter in all functions of this group is the operating mode
// 0-work with the buffer, 1-work directly with the disk
// when working with a buffer, a buffer for one record is used
// to read the entire record into the buffer, use the dbRecRead(Sel) command;
// to write the entire record from the buffer, the dbRecWrite(Sel) command is used;
//------------------------------------------------------------------------------
// for the current record of the area P1 and the field P2(number) P3(name) write the string P4
// if P2=-1 field is determined by name, otherwise by number
extern long dbFieldPutStr(int, int, unsigned char *, unsigned char *, int);
//------------------------------------------------------------------------------
// for the current record of the area P1 and the field P2(number) P3(name) write the date P4
// if P2=-1 field is determined by name, otherwise by number
// P4="D" -writes the current date in the field
extern long dbFieldPutDat(int, int, unsigned char *, unsigned char *, int);
//------------------------------------------------------------------------------
// for the current record of area P1 and field P2(number) P3(name) write logical value P4
// if P2=-1 field is determined by name, otherwise by number
extern long dbFieldPutLog(int, int, unsigned char *, unsigned char, int);
//------------------------------------------------------------------------------
// for the current record of the area P1 and the field P2(number) P3(name) write the value P4
// if P2=-1 field is determined by name, otherwise by number
extern long dbFieldPutVal(int, int, unsigned char *, double, int);
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
// for the current record of the area P1 from the field P2(number) P3(name) get a string
// if P2=-1 field is determined by name, otherwise by number
extern unsigned char *dbFieldGetStr(int, int, unsigned char *, int);
//------------------------------------------------------------------------------
// for the current record of the area P1 from the field P2(number) P3(name) get the date 
// if P2=-1 field is determined by name, otherwise by number
// [8-character date in YYYYMMDD format]
extern unsigned char *dbFieldGetDat(int, int, unsigned char *, int);
//------------------------------------------------------------------------------
// for the current record of the area P1 from the field P2(number) P3(name) get a logical value
// if P2=-1 field is determined by name, otherwise by number.[1 symbol]
extern unsigned char  dbFieldGetLog(int, int, unsigned char *, int);
//------------------------------------------------------------------------------
// for the current record of the area P1 from the field P2(number) P3(name) get the value
// if P2=-1 field is determined by name, otherwise by number.
extern double dbFieldGetVal(int, int, unsigned char *, int);
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
// ADDITIONAL FUNCTIONS
//------------------------------------------------------------------------------
// [memory size of library structures]
extern int  dbGetMemLib(void);
//------------------------------------------------------------------------------
// [calculated database size]
extern long dbFileSize(int);
//------------------------------------------------------------------------------
// populates the dbtm structure with the current date and time
// P1='D'-[current date YYYYMMDD], P1='T'-[current time HHMMSS]
extern unsigned long dbCurDate(unsigned char);
//------------------------------------------------------------------------------
// for area P1 and field P2 sets the pointer
// if P3=0 - in the write buffer, P3=1 - in the database file.
// [Offset in the database file or -1 if there are no records in the database]
extern long dbFieldOffset(int, int, int);
//------------------------------------------------------------------------------
// modifies the date of the last update for the DB header
extern void dbModifyDate(int);
//------------------------------------------------------------------------------
// modifies the number of db records for the db header
extern int  dbModifyRecCount(int, int);
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
// BUFFER FUNCTIONS
//------------------------------------------------------------------------------
// reserve buffer P1 for future use
// [-1 invalid buffer number, -2 buffer already taken, >=0 buffer number to work with]
extern int  dbBFInit(int);
//------------------------------------------------------------------------------
// search for free buffer [free buffer number]
extern int  dbBFBufferAvail(void);
//------------------------------------------------------------------------------
// clearing the buffer with the specified number [0< - the buffer number is set incorrectly]
extern int  dbBFClear(int);
//------------------------------------------------------------------------------
// clearing all buffers
extern void dbBFClearALL(void);
//------------------------------------------------------------------------------
// [maximum number of records in the buffer] P1-database area, P2-buffer number
extern int  dbBFRecInBuf(int, int);
//------------------------------------------------------------------------------
// reading the database file from the P1 area into the P2 buffer from the current record of the P1 area.
// P3 - number of records to be read
// [0< wrong area or buffer number,
// 0= nothing to read from the database, >0 number of buffered records read]
extern int  dbBFRead(int, int, int);
//------------------------------------------------------------------------------
// writing buffer P2 to the database file of area P1,
// P3 - number of records to be written if there were no additions
// P4 Write Mode (DBBFADD,DBBFWRITE,DBBFWRADD)
// [0< wrong area or buffer number,
//  0= nothing written, >0 number of records written to database file]
extern int  dbBFWrite(int, int, int, int);
//------------------------------------------------------------------------------
// Jump to the specified P2 entry in the P1 buffer
// [0< wrong buffer number or buffer entry number
//  >0 number of records in buffer]
extern int  dbBFGoRec(int, int);
//------------------------------------------------------------------------------
// transition to a record in the buffer P1 for the number of records specified in P2
// [0< wrong buffer number or buffer entry number
// >=0 number of the current record in the specified area]
extern int  dbBFGoSkip(int, int);
//------------------------------------------------------------------------------
// set pointer to first record(N0) in buffer P1
// [0< buffer number set incorrectly
//  =0 number of the current record in the specified area (N0)]
extern int  dbBFGoFirst(int);
//------------------------------------------------------------------------------
// set pointer to last entry in buffer P1
// [0< buffer number set incorrectly
//  >0 number of the current (last) record in the specified area]
extern int  dbBFGoLast(int);
//------------------------------------------------------------------------------
// checking for overshoot in buffer P1 after the last one
// move pointer in buffer [0-no exit, 1-exit]
extern int  dbBFBof(int);
//------------------------------------------------------------------------------
// check for underflow in buffer P1 after the last one
// move pointer in buffer [0-no exit, 1-exit]
extern int  dbBFEof(int);
//------------------------------------------------------------------------------
// [number of current entry in buffer P1]
extern int  dbBFRecCur(int);
//------------------------------------------------------------------------------
// [number of entries in buffer P1 counting of deleted]
extern int  dbBFRecCount(int);
//------------------------------------------------------------------------------
// copy current database record from area P1 to current buffer record P2
// [0< buffer number set incorrectly
//  >0 entry copied normally]
extern int  dbBFRecCopyBuf(int, int);
//------------------------------------------------------------------------------
// copy the current record from buffer P2 to the current record in area P1 
// [0< buffer number set incorrectly
//  >0 entry copied normally]
extern int  dbBFBufCopyRec(int, int);
//------------------------------------------------------------------------------
// setting the deletion flag P2 in the buffer P1.
// P3=0-remove deletion sign, P3>0-set deletion sign.
extern int  dbBFRecDelete(int, int, int);
//------------------------------------------------------------------------------
// checking the sign of deletion of record P2 in buffer P1
extern int  isBFRecDelete(int, int);
//------------------------------------------------------------------------------

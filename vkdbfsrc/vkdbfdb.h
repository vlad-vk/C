//------------------------------------------------------------------------------
//  description of some internal functions
int dbOpenTest(int, uc*);

//------------------------------------------------------------------------------
//  working variables and arrays
struct   tm  *dbtm;
time_t   dbTMV = 0;               // structures for getting the current time
uc  BHead[DBHDLEN];               // header read buffer
uc  BRead[DBBSIZ];                // buffer for reading data
uc  BRec [DBMAXDBF][MRECLEN];     // single entry buffer
uc  BCRD [DBMAXDBF][1];           // signs of deletion of records

uc  BSFld[5][DBSLEN];             // buffer for internal work with strings
_i  dbDebug = 0;                  // debug output flag

//------------------------------------------------------------------------------
//  filename structure
struct {
       uc  Name[DBSLEN];          // database file name
       _i  Desk;                  // descriptor (system number)
       _i  Mod;                   // file modification flag 1 - there was a modification
       _i  Init;                  // array initialization flag
} dbFile[DBMAXDBF];

//------------------------------------------------------------------------------
//  database header structure
struct {
       uc  ID;                    // [1] Identifier
       uc  Year;                  // [3] Last update date
       uc  Mon;
       uc  Day;
       uc  RecCount[4];           // [4] number of entries including deleted ones
       uc  HeadSize[2];           // [2] total header length
       uc  RecLen[2];             // [2] length of one record
       uc  LibFlg[2];             // [2] flag for creating a database by this library (LB)
       uc  Tranz;                 // [1] 0/1 transaction delay for DB4
       uc  HFld[2];               // [2] number of fields in the database (LB)
       uc  CRec[4];               // [4] number of the current record in the database (LB)
       uc  COfs[4];               // [4] file offset for the current record (LB)
       uc  Rezerv1[2];            // [2]
       uc  Rezerv2[1];            // [1]
       uc  MDX;                   // [1] whether the set file is an MDX index
       uc  Reserv3[3];            // [3] reserve
} dbHead[DBMAXDBF];               // 

//------------------------------------------------------------------------------
//  extended header structure
struct {
       uc  Buf[DBBEXT];           // extended file headers
       _i  Len;                   // extended header length
} dbHExt[DBMAXDBF];

//------------------------------------------------------------------------------
//  database header working structure
struct {
       _l  RecCount;              // number of records in the database
       _i  RecLen;                // length of one record
       _l  CRec;                  // number of the current record in the database
       _l  COfs;                  // started offset in file for current record
       _i  Bof;                   // flag of attempt to leave the beginning of the database
       _i  Eof;                   // flag of attempt to leave the end of the database
       _i  FieldCount;            // number of fields in open database
} dbHeaw[DBMAXDBF];
_i     CurDBF  = 0;               // current database number

//------------------------------------------------------------------------------
//  database field structure
struct {
       uc  Name[11];              // [11] name, ends
       uc  Tip;                   // [1] type C,L,D,M,N,F
       uc  OfsFld[4];             // [4] a pointer to a field within a record
       uc  Len;                   // [1] field length
       uc  Dec;                   // [1] a number of simbols after comma
       uc  Rezer1[2];             // [2] field offset
       uc  Rezer2[11];            // [11] 
       uc  TegMTX;                // [1] 0/1 presence of the MTX for DB4 multi-index file tag
} dbField[DBMAXDBF][DBMAXFLD];    // Total 32 bytes for one field

//------------------------------------------------------------------------------
//  working structure of database fields
struct {
       _l  OfsFld;                // a pointer to a field within a record
} dbFielw[DBMAXDBF][DBMAXFLD];

//------------------------------------------------------------------------------
//  working structure for working with database buffers
struct {
       uc  Arr[DBBSIZ];           // database buffer
       _i  RecCount;              // number of buffer entries
       _i  RecCur;                // current buffer entry
       _i  RecLen;                // length of record in the buffer (this field is filled after read/write operations)
       _i  Init;                  // buffer initialization (capture) flag
       _i  Bof;                   // buffer overflow flag
       _i  Eof;                   // buffer underflow flag
} dbBUFF  [DBMAXBUFF];


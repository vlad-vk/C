//------------------------------------------------------------------------------
#define DBBEXT          1000            // extended header buffer size

#ifdef OBC
  #define DBBSIZ        10000           // data buffer size
  #define DBSLEN        255             // max line length
  #define DBMAXDBF      1               // max open DB files at the same time
  #define DBMAXFLD      50              // max number of fields in the database
#else
  #define DBBSIZ        100000          // data buffer size
  #define DBSLEN        255             // max line length
  #define DBMAXDBF      10              // max open files at the same time
  #define DBMAXFLD      500             // max number of fields in the database
  #define DBMAXBUFF     12              // max number of buffers for working with the database
#endif

#define DBMAXREC        0x7ffffff0      // max number of records in the database
#define DBHDLEN         32              // DBF file header length
#define LENDOUBLE       8               // length of a DOUBLE type field for a binary record
#define LENINTEGR       4               // length of INTEGER type field for binary record
#define LENLOGIC        1               // length of LOGIC type field
#define LENDATE         8               // length of DATE type field

#ifdef OBC
  #define MRECLEN       0x7ffe          // maximum record length
#else
  #define MRECLEN       0xffff          // maximum record length
#endif

#define DBWBUFF         0               // work with current buffer entry
#define DBWDISK         1               // work with the current disk entry

#define DBBFADD         1               // buffer write mode with adding entries
#define DBBFWRITE       2               // ... write without adding (already existing)
#define DBBFWRADD       3               // add entries if needed

#define INFO            0               // get information only
#define CORR            1               // correct if an error is found
//------------------------------------------------------------------------------

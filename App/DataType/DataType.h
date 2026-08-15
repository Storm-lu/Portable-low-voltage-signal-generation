#ifndef _DATA_TYPE_H_
#define _DATA_TYPE_H_


typedef signed char         i8;
typedef signed short        i16;
typedef signed int          i32;
typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;

typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      HWORD;
typedef unsigned int        WORD;
typedef long                LONG;

#define LOHWORD(w)          ((HWORD)(w))
#define HIHWORD(w)          ((HWORD)(((WORD)(w) >> 16) & 0xFFFF))

#define LOBYTE(hw)          ((BYTE)(hw) )
#define HIBYTE(hw)          ((BYTE)(((WORD)(hw) >> 8) & 0xFF))


#define MAKEHWORD(bH, bL)   ((HWORD)(((BYTE)(bL)) | ((HWORD)((BYTE)(bH))) << 8))


#define MAKEWORD(hwH, hwL)  ((WORD)(((HWORD)(hwL)) | ((WORD)((HWORD)(hwH))) << 16))

#define TRUE          1
#define FALSE         0
#define NULL          0
#define INVALID_DATA  -100


#endif

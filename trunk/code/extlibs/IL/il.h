//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2008 by Denton Woods
// Last modified: 12/23/2008
//
// Filename: IL/il.h
//
// Description: The main include file for DevIL
//
//-----------------------------------------------------------------------------


#ifndef __il_h_
#ifndef __IL_H__

#define __il_h_
#define __IL_H__

#ifdef __cplusplus
extern "C" {
#endif

//this define controls if floats and doubles are clampled to [0..1]
//during conversion. It takes a little more time, but it is the correct
//way of doing this. If you are sure your floats are always valid,
//you can undefine this value...
//#define CLAMP_HALF		1
//#define CLAMP_FLOATS	1
//#define CLAMP_DOUBLES	1

#ifdef _WIN32_WCE
	#define IL_NO_GIF
	#define IL_NO_JPG
	#define IL_NO_MNG
	#define IL_NO_PNG
	#define IL_NO_TIF
	#define IL_NO_LCMS
#endif //_WIN32_WCE

#ifdef DJGPP
	#define IL_NO_GIF
	#define IL_NO_JPG
	#define IL_NO_MNG
	#define IL_NO_PNG
	#define IL_NO_TIF
	#define IL_NO_LCMS
#endif //DJGPP

#ifdef _WIN32
	#if (defined(IL_USE_PRAGMA_LIBS)) && (!defined(_IL_BUILD_LIBRARY))
		#if defined(_MSC_VER) || defined(__BORLANDC__)
			#pragma comment(lib, "DevIL.lib")
		#endif
	#endif
#endif

#ifdef RESTRICT_KEYWORD
#define RESTRICT restrict
#define CONST_RESTRICT const restrict
#else
#define RESTRICT
#define CONST_RESTRICT const
#endif

#include <stdio.h>

typedef unsigned int   ILenum;
typedef unsigned char  ILboolean;
typedef unsigned int   ILbitfield;
typedef signed char    ILbyte;
typedef signed short   ILshort;
typedef int     	   ILint;
typedef size_t         ILsizei;
typedef unsigned char  ILubyte;
typedef unsigned short ILushort;
typedef unsigned int   ILuint;
typedef float          ILfloat;
typedef float          ILclampf;
typedef double         ILdouble;
typedef double         ILclampd;
#include <limits.h>
#ifdef _UNICODE
	#ifndef _WIN32_WCE
		#include <wchar.h>
	#endif
	//if we use a define instead of a typedef,
	//ILconst_string works as intended
	#define ILchar wchar_t
	#define ILstring wchar_t*
	#define ILconst_string  wchar_t const *
#else
	//if we use a define instead of a typedef,
	//ILconst_string works as intended
	#define ILchar char
	#define ILstring char*
	#define ILconst_string char const *
#endif //_UNICODE

#define IL_FALSE							0
#define IL_TRUE								1

// Matches OpenGL's right now.
#define IL_COLOUR_INDEX     0x1900
#define IL_COLOR_INDEX      0x1900
#define IL_RGB              0x1907
#define IL_RGBA             0x1908
#define IL_BGR              0x80E0
#define IL_BGRA             0x80E1
#define IL_LUMINANCE        0x1909
#define IL_LUMINANCE_ALPHA  0x190A
#define IL_ALPHA			0x190B


#define IL_BYTE           0x1400
#define IL_UNSIGNED_BYTE  0x1401
#define IL_SHORT          0x1402
#define IL_UNSIGNED_SHORT 0x1403
#define IL_INT            0x1404
#define IL_UNSIGNED_INT   0x1405
#define IL_FLOAT          0x1406
#define IL_DOUBLE         0x140A
#define IL_HALF           0x140B


#define IL_MAX_BYTE		  		SCHAR_MAX
#define IL_MAX_UNSIGNED_BYTE  	UCHAR_MAX
#define IL_MAX_SHORT	  		SHRT_MAX
#define IL_MAX_UNSIGNED_SHORT 	USHRT_MAX
#define IL_MAX_INT		  		INT_MAX
#define IL_MAX_UNSIGNED_INT   	UINT_MAX

#define IL_LIMIT(x,m,M)		(x<m?m:(x>M?M:x))
#define IL_CLAMP(x) 		IL_LIMIT(x,0,1)

#define IL_VENDOR   0x1F00
#define IL_LOAD_EXT 0x1F01
#define IL_SAVE_EXT 0x1F02


//
// IL-specific #define's
//

#define IL_VERSION_1_7_5 1
#define IL_VERSION       175


// Attribute Bits
#define IL_ORIGIN_BIT          0x00000001
#define IL_FILE_BIT            0x00000002
#define IL_PAL_BIT             0x00000004
#define IL_FORMAT_BIT          0x00000008
#define IL_TYPE_BIT            0x00000010
#define IL_COMPRESS_BIT        0x00000020
#define IL_LOADFAIL_BIT        0x00000040
#define IL_FORMAT_SPECIFIC_BIT 0x00000080
#define IL_ALL_ATTRIB_BITS     0x000FFFFF


// Palette types
#define IL_PAL_NONE   0x0400
#define IL_PAL_RGB24  0x0401
#define IL_PAL_RGB32  0x0402
#define IL_PAL_RGBA32 0x0403
#define IL_PAL_BGR24  0x0404
#define IL_PAL_BGR32  0x0405
#define IL_PAL_BGRA32 0x0406


// Image types
#define IL_TYPE_UNKNOWN 0x0000
#define IL_BMP          0x0420
#define IL_CUT          0x0421
#define IL_DOOM         0x0422
#define IL_DOOM_FLAT    0x0423
#define IL_ICO          0x0424
#define IL_JPG          0x0425
#define IL_JFIF         0x0425
#define IL_LBM          0x0426
#define IL_PCD          0x0427
#define IL_PCX          0x0428
#define IL_PIC          0x0429
#define IL_PNG          0x042A
#define IL_PNM          0x042B
#define IL_SGI          0x042C
#define IL_TGA          0x042D
#define IL_TIF          0x042E
#define IL_CHEAD        0x042F
#define IL_RAW          0x0430
#define IL_MDL          0x0431
#define IL_WAL          0x0432
#define IL_LIF          0x0434
#define IL_MNG          0x0435
#define IL_JNG          0x0435
#define IL_GIF          0x0436
#define IL_DDS          0x0437
#define IL_DCX          0x0438
#define IL_PSD          0x0439
#define IL_EXIF         0x043A
#define IL_PSP          0x043B
#define IL_PIX          0x043C
#define IL_PXR          0x043D
#define IL_XPM          0x043E
#define IL_HDR          0x043F
#define IL_ICNS			0x0440
#define IL_JP2			0x0441
#define IL_EXR			0x0442
#define IL_WDP			0x0443
#define IL_VTF			0x0444


#define IL_JASC_PAL     0x0475


// Error Types
#define IL_NO_ERROR             0x0000
#define IL_INVALID_ENUM         0x0501
#define IL_OUT_OF_MEMORY        0x0502
#define IL_FORMAT_NOT_SUPPORTED 0x0503
#define IL_INTERNAL_ERROR       0x0504
#define IL_INVALID_VALUE        0x0505
#define IL_ILLEGAL_OPERATION    0x0506
#define IL_ILLEGAL_FILE_VALUE   0x0507
#define IL_INVALID_FILE_HEADER  0x0508
#define IL_INVALID_PARAM        0x0509
#define IL_COULD_NOT_OPEN_FILE  0x050A
#define IL_INVALID_EXTENSION    0x050B
#define IL_FILE_ALREADY_EXISTS  0x050C
#define IL_OUT_FORMAT_SAME      0x050D
#define IL_STACK_OVERFLOW       0x050E
#define IL_STACK_UNDERFLOW      0x050F
#define IL_INVALID_CONVERSION   0x0510
#define IL_BAD_DIMENSIONS       0x0511
#define IL_FILE_READ_ERROR      0x0512  // 05/12/2002: Addition by Sam.
#define IL_FILE_WRITE_ERROR     0x0512

#define IL_LIB_GIF_ERROR  0x05E1
#define IL_LIB_JPEG_ERROR 0x05E2
#define IL_LIB_PNG_ERROR  0x05E3
#define IL_LIB_TIFF_ERROR 0x05E4
#define IL_LIB_MNG_ERROR  0x05E5
#define IL_LIB_JP2_ERROR  0x05E6
#define IL_UNKNOWN_ERROR  0x05FF


// Origin Definitions
#define IL_ORIGIN_SET        0x0600
#define IL_ORIGIN_LOWER_LEFT 0x0601
#define IL_ORIGIN_UPPER_LEFT 0x0602
#define IL_ORIGIN_MODE       0x0603


// Format and Type Mode Definitions
#define IL_FORMAT_SET  0x0610
#define IL_FORMAT_MODE 0x0611
#define IL_TYPE_SET    0x0612
#define IL_TYPE_MODE   0x0613


// File definitions
#define IL_FILE_OVERWRITE	0x0620
#define IL_FILE_MODE		0x0621


// Palette definitions
#define IL_CONV_PAL			0x0630


// Load fail definitions
#define IL_DEFAULT_ON_FAIL	0x0632


// Key colour and alpha definitions
#define IL_USE_KEY_COLOUR	0x0635
#define IL_USE_KEY_COLOR	0x0635
#define IL_BLIT_BLEND		0x0636


// Interlace definitions
#define IL_SAVE_INTERLACED	0x0639
#define IL_INTERLACE_MODE	0x063A


// Quantization definitions
#define IL_QUANTIZATION_MODE 0x0640
#define IL_WU_QUANT          0x0641
#define IL_NEU_QUANT         0x0642
#define IL_NEU_QUANT_SAMPLE  0x0643
#define IL_MAX_QUANT_INDEXS  0x0644 //XIX : ILint : Maximum number of colors to reduce to, default of 256. and has a range of 2-256


// Hints
#define IL_FASTEST          0x0660
#define IL_LESS_MEM         0x0661
#define IL_DONT_CARE        0x0662
#define IL_MEM_SPEED_HINT   0x0665
#define IL_USE_COMPRESSION  0x0666
#define IL_NO_COMPRESSION   0x0667
#define IL_COMPRESSION_HINT 0x0668


// Subimage types
#define IL_SUB_NEXT   0x0680
#define IL_SUB_MIPMAP 0x0681
#define IL_SUB_LAYER  0x0682


// Compression definitions
#define IL_COMPRESS_MODE 0x0700
#define IL_COMPRESS_NONE 0x0701
#define IL_COMPRESS_RLE  0x0702
#define IL_COMPRESS_LZO  0x0703
#define IL_COMPRESS_ZLIB 0x0704


// File format-specific values
#define IL_TGA_CREATE_STAMP        0x0710
#define IL_JPG_QUALITY             0x0711
#define IL_PNG_INTERLACE           0x0712
#define IL_TGA_RLE                 0x0713
#define IL_BMP_RLE                 0x0714
#define IL_SGI_RLE                 0x0715
#define IL_TGA_ID_STRING           0x0717
#define IL_TGA_AUTHNAME_STRING     0x0718
#define IL_TGA_AUTHCOMMENT_STRING  0x0719
#define IL_PNG_AUTHNAME_STRING     0x071A
#define IL_PNG_TITLE_STRING        0x071B
#define IL_PNG_DESCRIPTION_STRING  0x071C
#define IL_TIF_DESCRIPTION_STRING  0x071D
#define IL_TIF_HOSTCOMPUTER_STRING 0x071E
#define IL_TIF_DOCUMENTNAME_STRING 0x071F
#define IL_TIF_AUTHNAME_STRING     0x0720
#define IL_JPG_SAVE_FORMAT         0x0721
#define IL_CHEAD_HEADER_STRING     0x0722
#define IL_PCD_PICNUM              0x0723
#define IL_JPG_PROGRESSIVE         0x0725

#define IL_PNG_ALPHA_INDEX 0x0724 //XIX : ILint : the color in the palette at this index value (0-255) is considered transparent, -1 for no trasparent color

// DXTC definitions
#define IL_DXTC_FORMAT      0x0705
#define IL_DXT1             0x0706
#define IL_DXT2             0x0707
#define IL_DXT3             0x0708
#define IL_DXT4             0x0709
#define IL_DXT5             0x070A
#define IL_DXT_NO_COMP      0x070B
#define IL_KEEP_DXTC_DATA   0x070C
#define IL_DXTC_DATA_FORMAT 0x070D
#define IL_3DC              0x070E
#define IL_RXGB             0x070F
#define IL_ATI1N            0x0710

// Cube map definitions
#define IL_CUBEMAP_POSITIVEX 0x00000400
#define IL_CUBEMAP_NEGATIVEX 0x00000800
#define IL_CUBEMAP_POSITIVEY 0x00001000
#define IL_CUBEMAP_NEGATIVEY 0x00002000
#define IL_CUBEMAP_POSITIVEZ 0x00004000
#define IL_CUBEMAP_NEGATIVEZ 0x00008000


// Values
#define IL_VERSION_NUM           0x0DE2
#define IL_IMAGE_WIDTH           0x0DE4
#define IL_IMAGE_HEIGHT          0x0DE5
#define IL_IMAGE_DEPTH           0x0DE6
#define IL_IMAGE_SIZE_OF_DATA    0x0DE7
#define IL_IMAGE_BPP             0x0DE8
#define IL_IMAGE_BYTES_PER_PIXEL 0x0DE8
#define IL_IMAGE_BPP             0x0DE8
#define IL_IMAGE_BITS_PER_PIXEL  0x0DE9
#define IL_IMAGE_FORMAT          0x0DEA
#define IL_IMAGE_TYPE            0x0DEB
#define IL_PALETTE_TYPE          0x0DEC
#define IL_PALETTE_SIZE          0x0DED
#define IL_PALETTE_BPP           0x0DEE
#define IL_PALETTE_NUM_COLS      0x0DEF
#define IL_PALETTE_BASE_TYPE     0x0DF0
#define IL_NUM_IMAGES            0x0DF1
#define IL_NUM_MIPMAPS           0x0DF2
#define IL_NUM_LAYERS            0x0DF3
#define IL_ACTIVE_IMAGE          0x0DF4
#define IL_ACTIVE_MIPMAP         0x0DF5
#define IL_ACTIVE_LAYER          0x0DF6
#define IL_CUR_IMAGE             0x0DF7
#define IL_IMAGE_DURATION        0x0DF8
#define IL_IMAGE_PLANESIZE       0x0DF9
#define IL_IMAGE_BPC             0x0DFA
#define IL_IMAGE_OFFX            0x0DFB
#define IL_IMAGE_OFFY            0x0DFC
#define IL_IMAGE_CUBEFLAGS       0x0DFD
#define IL_IMAGE_ORIGIN          0x0DFE
#define IL_IMAGE_CHANNELS        0x0DFF

# if defined __GNUC__ && (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ > 0))
// __attribute__((deprecated)) is supported by GCC 3.1 and later.
#  define DEPRECATED(D) D __attribute__((deprecated))
# elif defined _MSC_VER && _MSC_VER >= 1300
// __declspec(deprecated) is supported by MSVC 7.0 and later.
#  define DEPRECATED(D) __declspec(deprecated) D
# else
#  define DEPRECATED (D) D
# endif

//
// Section shamelessly modified from the glut header.
//

// This is from Win32's <windef.h>
#if (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED) || defined(__BORLANDC__) || defined(__LCC__)
	#define ILAPIENTRY __stdcall 
	#define IL_PACKSTRUCT
//#elif defined(linux) || defined(MACOSX) || defined(__CYGWIN__) //fix bug 840364
#elif defined( __GNUC__ )
  // this should work for any of the above commented platforms 
  // plus any platform using GCC
	#ifdef __MINGW32__
		#define ILAPIENTRY __stdcall
	#else
		#define ILAPIENTRY
	#endif
	#define IL_PACKSTRUCT __attribute__ ((packed))
#else
	#define ILAPIENTRY
	#define IL_PACKSTRUCT
#endif

// This is from Win32's <wingdi.h> and <winnt.h>
#if defined(__LCC__)
	#define ILAPI __stdcall
#elif defined(_WIN32) //changed 20031221 to fix bug 840421
	#ifdef IL_STATIC_LIB
		#define ILAPI
	#else
		#ifdef _IL_BUILD_LIBRARY
			#define ILAPI __declspec(dllexport)
		#else
			#define ILAPI __declspec(dllimport)
		#endif
	#endif
#elif __APPLE__
	#define ILAPI extern
#else
	#define ILAPI
#endif


#define IL_SEEK_SET	0
#define IL_SEEK_CUR	1
#define IL_SEEK_END	2
#define IL_EOF		-1


// Callback functions for file reading
typedef void* ILHANDLE;
typedef void      (ILAPIENTRY *fCloseRProc)(ILHANDLE);
typedef ILboolean (ILAPIENTRY *fEofProc)   (ILHANDLE);
typedef ILint     (ILAPIENTRY *fGetcProc)  (ILHANDLE);
typedef ILHANDLE  (ILAPIENTRY *fOpenRProc) (ILconst_string);
typedef ILint     (ILAPIENTRY *fReadProc)  (void*, ILuint, ILuint, ILHANDLE);
typedef ILint     (ILAPIENTRY *fSeekRProc) (ILHANDLE, ILint, ILint);
typedef ILint     (ILAPIENTRY *fTellRProc) (ILHANDLE);

// Callback functions for file writing
typedef void     (ILAPIENTRY *fCloseWProc)(ILHANDLE);
typedef ILHANDLE (ILAPIENTRY *fOpenWProc) (ILconst_string);
typedef ILint    (ILAPIENTRY *fPutcProc)  (ILubyte, ILHANDLE);
typedef ILint    (ILAPIENTRY *fSeekWProc) (ILHANDLE, ILint, ILint);
typedef ILint    (ILAPIENTRY *fTellWProc) (ILHANDLE);
typedef ILint    (ILAPIENTRY *fWriteProc) (const void*, ILuint, ILuint, ILHANDLE);

// Callback functions for allocation and deallocation
typedef void* (ILAPIENTRY *mAlloc)(const ILsizei);
typedef void  (ILAPIENTRY *mFree) (const void* CONST_RESTRICT);

// Registered format procedures
typedef ILenum (ILAPIENTRY *IL_LOADPROC)(ILconst_string);
typedef ILenum (ILAPIENTRY *IL_SAVEPROC)(ILconst_string);


// ImageLib Functions
ILAPI ILboolean ILAPIENTRY ilActiveImage(ILuint Number);
ILAPI ILboolean ILAPIENTRY ilActiveLayer(ILuint Number);
ILAPI ILboolean ILAPIENTRY ilActiveMipmap(ILuint Number);
ILAPI ILboolean ILAPIENTRY ilApplyPal(ILconst_string FileName);
ILAPI ILboolean ILAPIENTRY ilApplyProfile(ILstring InProfile, ILstring OutProfile);
ILAPI void		ILAPIENTRY ilBindImage(ILuint Image);
ILAPI ILboolean ILAPIENTRY ilBlit(ILuint Source, ILint DestX, ILint DestY, ILint DestZ, ILuint SrcX, ILuint SrcY, ILuint SrcZ, ILuint Width, ILuint Height, ILuint Depth);
ILAPI void		ILAPIENTRY ilClearColour(ILclampf Red, ILclampf Green, ILclampf Blue, ILclampf Alpha);
ILAPI ILboolean ILAPIENTRY ilClearImage(void);
ILAPI ILuint    ILAPIENTRY ilCloneCurImage(void);
ILAPI ILboolean ILAPIENTRY ilCompressFunc(ILenum Mode);
ILAPI ILboolean ILAPIENTRY ilConvertImage(ILenum DestFormat, ILenum DestType);
ILAPI ILboolean ILAPIENTRY ilConvertPal(ILenum DestFormat);
ILAPI ILboolean ILAPIENTRY ilCopyImage(ILuint Src);
ILAPI ILuint    ILAPIENTRY ilCopyPixels(ILuint XOff, ILuint YOff, ILuint ZOff, ILuint Width, ILuint Height, ILuint Depth, ILenum Format, ILenum Type, void *Data);
ILAPI ILuint    ILAPIENTRY ilCreateSubImage(ILenum Type, ILuint Num);
ILAPI ILboolean ILAPIENTRY ilDefaultImage(void);
ILAPI void		ILAPIENTRY ilDeleteImage(const ILuint Num);
ILAPI void      ILAPIENTRY ilDeleteImages(ILsizei Num, const ILuint *Images);
ILAPI ILboolean ILAPIENTRY ilDisable(ILenum Mode);
ILAPI ILboolean ILAPIENTRY ilEnable(ILenum Mode);
ILAPI ILboolean ILAPIENTRY ilFormatFunc(ILenum Mode);
ILAPI void	    ILAPIENTRY ilGenImages(ILsizei Num, ILuint *Images);
ILAPI ILuint	ILAPIENTRY ilGenImage();
ILAPI ILubyte*  ILAPIENTRY ilGetAlpha(ILenum Type);
ILAPI ILboolean ILAPIENTRY ilGetBoolean(ILenum Mode);
ILAPI void      ILAPIENTRY ilGetBooleanv(ILenum Mode, ILboolean *Param);
ILAPI ILubyte*  ILAPIENTRY ilGetData(void);
ILAPI ILuint    ILAPIENTRY ilGetDXTCData(void *Buffer, ILuint BufferSize, ILenum DXTCFormat);
ILAPI ILenum    ILAPIENTRY ilGetError(void);
ILAPI ILint     ILAPIENTRY ilGetInteger(ILenum Mode);
ILAPI void      ILAPIENTRY ilGetIntegerv(ILenum Mode, ILint *Param);
ILAPI ILuint    ILAPIENTRY ilGetLumpPos(void);
ILAPI ILubyte*  ILAPIENTRY ilGetPalette(void);
ILAPI ILstring  ILAPIENTRY ilGetString(ILenum StringName);
ILAPI void      ILAPIENTRY ilHint(ILenum Target, ILenum Mode);
ILAPI void      ILAPIENTRY ilInit(void);
ILAPI ILboolean ILAPIENTRY ilIsDisabled(ILenum Mode);
ILAPI ILboolean ILAPIENTRY ilIsEnabled(ILenum Mode);
ILAPI ILenum	ILAPIENTRY ilDetermineTypeF(ILHANDLE File);
ILAPI ILboolean ILAPIENTRY ilIsImage(ILuint Image);
ILAPI ILboolean ILAPIENTRY ilIsValid(ILenum Type, ILstring FileName);
ILAPI ILboolean ILAPIENTRY ilIsValidF(ILenum Type, ILHANDLE File);
ILAPI ILboolean ILAPIENTRY ilIsValidL(ILenum Type, void *Lump, ILuint Size);
ILAPI void      ILAPIENTRY ilKeyColour(ILclampf Red, ILclampf Green, ILclampf Blue, ILclampf Alpha);
ILAPI ILboolean ILAPIENTRY ilLoad(ILenum Type, ILconst_string FileName);
ILAPI ILboolean ILAPIENTRY ilLoadF(ILenum Type, ILHANDLE File);
ILAPI ILboolean ILAPIENTRY ilLoadImage(ILconst_string FileName);
ILAPI ILboolean ILAPIENTRY ilLoadL(ILenum Type, const void *Lump, ILuint Size);
ILAPI ILboolean ILAPIENTRY ilLoadPal(ILconst_string FileName);
ILAPI void      ILAPIENTRY ilModAlpha(ILdouble AlphaValue);
ILAPI ILboolean ILAPIENTRY ilOriginFunc(ILenum Mode);
ILAPI ILboolean ILAPIENTRY ilOverlayImage(ILuint Source, ILint XCoord, ILint YCoord, ILint ZCoord);
ILAPI void      ILAPIENTRY ilPopAttrib(void);
ILAPI void      ILAPIENTRY ilPushAttrib(ILuint Bits);
ILAPI void      ILAPIENTRY ilRegisterFormat(ILenum Format);
ILAPI ILboolean ILAPIENTRY ilRegisterLoad(ILconst_string Ext, IL_LOADPROC Load);
ILAPI ILboolean ILAPIENTRY ilRegisterMipNum(ILuint Num);
ILAPI ILboolean ILAPIENTRY ilRegisterNumImages(ILuint Num);
ILAPI void      ILAPIENTRY ilRegisterOrigin(ILenum Origin);
ILAPI void      ILAPIENTRY ilRegisterPal(void *Pal, ILuint Size, ILenum Type);
ILAPI ILboolean ILAPIENTRY ilRegisterSave(ILconst_string Ext, IL_SAVEPROC Save);
ILAPI void      ILAPIENTRY ilRegisterType(ILenum Type);
ILAPI ILboolean ILAPIENTRY ilRemoveLoad(ILconst_string Ext);
ILAPI ILboolean ILAPIENTRY ilRemoveSave(ILconst_string Ext);
ILAPI void      ILAPIENTRY ilResetMemory(void); // Deprecated
ILAPI void      ILAPIENTRY ilResetRead(void);
ILAPI void      ILAPIENTRY ilResetWrite(void);
ILAPI ILboolean ILAPIENTRY ilSave(ILenum Type, ILstring FileName);
ILAPI ILuint    ILAPIENTRY ilSaveF(ILenum Type, ILHANDLE File);
ILAPI ILboolean ILAPIENTRY ilSaveImage(ILconst_string FileName);
ILAPI ILuint    ILAPIENTRY ilSaveL(ILenum Type, void *Lump, ILuint Size);
ILAPI ILboolean ILAPIENTRY ilSavePal(ILconst_string FileName);
ILAPI ILboolean ILAPIENTRY ilSetAlpha( ILdouble AlphaValue );
ILAPI ILboolean ILAPIENTRY ilSetData(void *Data);
ILAPI ILboolean ILAPIENTRY ilSetDuration(ILuint Duration);
ILAPI void      ILAPIENTRY ilSetInteger(ILenum Mode, ILint Param);
ILAPI void      ILAPIENTRY ilSetMemory(mAlloc, mFree);
ILAPI void      ILAPIENTRY ilSetPixels(ILint XOff, ILint YOff, ILint ZOff, ILuint Width, ILuint Height, ILuint Depth, ILenum Format, ILenum Type, void *Data);
ILAPI void      ILAPIENTRY ilSetRead(fOpenRProc, fCloseRProc, fEofProc, fGetcProc, fReadProc, fSeekRProc, fTellRProc);
ILAPI void      ILAPIENTRY ilSetString(ILenum Mode, const char *String);
ILAPI void      ILAPIENTRY ilSetWrite(fOpenWProc, fCloseWProc, fPutcProc, fSeekWProc, fTellWProc, fWriteProc);
ILAPI void      ILAPIENTRY ilShutDown(void);
ILAPI ILboolean ILAPIENTRY ilTexImage(ILuint Width, ILuint Height, ILuint Depth, ILubyte numChannels, ILenum Format, ILenum Type, void *Data);
ILAPI ILenum    ILAPIENTRY ilTypeFromExt(ILconst_string FileName);
ILAPI ILboolean ILAPIENTRY ilTypeFunc(ILenum Mode);
ILAPI ILboolean ILAPIENTRY ilLoadData(ILconst_string FileName, ILuint Width, ILuint Height, ILuint Depth, ILubyte Bpp);
ILAPI ILboolean ILAPIENTRY ilLoadDataF(ILHANDLE File, ILuint Width, ILuint Height, ILuint Depth, ILubyte Bpp);
ILAPI ILboolean ILAPIENTRY ilLoadDataL(void *Lump, ILuint Size, ILuint Width, ILuint Height, ILuint Depth, ILubyte Bpp);
ILAPI ILboolean ILAPIENTRY ilSaveData(ILconst_string FileName);

// For all those weirdos that spell "colour" without the 'u'.
#define ilClearColor	ilClearColour
#define ilKeyColor      ilKeyColour

#define imemclear(x,y) memset(x,0,y);

#ifdef __cplusplus
}
#endif

#endif // __IL_H__
#endif // __il_h__

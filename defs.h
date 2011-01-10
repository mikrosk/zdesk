#define ZDESK_VERSION	"0.1"

#ifndef MAXNAMLEN
	#define MAXNAMLEN	256
#endif

#ifndef MAX_PATH
	#define MAX_PATH	1024
#endif

#ifndef TRUE
# define TRUE 1
# define FALSE 0
#endif

#ifndef WHITE
# define WHITE    0
# define BLACK    1
# define RED      2
# define GREEN    3
# define BLUE     4
# define CYAN     5
# define YELLOW   6
# define MAGENTA  7
# define LWHITE   8
# define LBLACK   9
# define LRED     10
# define LGREEN   11
# define LBLUE    12
# define LCYAN    13
# define LYELLOW  14
# define LMAGENTA 15
#endif

#ifndef __SCANCODE__
#define __SCANCODE__

#define SC_ESC  	0x01
#define SC_BACK		0x0E
#define SC_TAB		0x0F
#define SC_RETURN	0x1C
#define SC_DEL		0x53
#define SC_SPACE	0x39
#define SC_HELP		0x62
#define SC_UNDO 	0x61
#define SC_INSERT	0x52
#define SC_HOME		0x47
#define SC_UPARW	0x48
#define SC_LFARW	0x4B
#define SC_DNARW	0x50
#define SC_DWARW SC_DNARW
#define SC_RTARW	0x4D
#define	SC_LFPAR	0x63	
#define SC_RTPAR	0x64
#define SC_SLASH	0x65
#define SC_STAR		0x66
#define SC_SEVEN	0x67
#define SC_EIGHT	0x68
#define SC_NINE		0x69
#define SC_MINUS	0x4A
#define SC_FOUR		0x6A
#define SC_FIVE		0x6B
#define SC_SIX		0x6C
#define SC_PLUS		0x4E
#define SC_ONE		0x6D
#define SC_TWO		0x6E
#define SC_THREE	0x6F
#define SC_ENTER	0x72
#define SC_ZERO		0x70
#define SC_POINT	0x71
#define SC_F1		0x3B
#define SC_F2		0x3C
#define SC_F3		0x3D
#define SC_F4		0x3E
#define SC_F5		0x3F
#define SC_F6		0x40
#define SC_F7		0x41
#define SC_F8		0x42
#define SC_F9		0x43
#define SC_F10		0x44
#define SC_F11		0x54
#define SC_F12		0x55
#define SC_F13		0x56
#define SC_F14		0x57
#define SC_F15		0x58
#define SC_F16		0x59
#define SC_F17		0x5A
#define SC_F18		0x5B
#define SC_F19		0x5C
#define SC_F20		0x5D
/* Available with MagiC Mac and Magic PC */
#define SC_PGUP		0x49
#define SC_PGDN		0x51

#endif


/* Window's Attrib. */
#define WINATTRIB  MOVER|NAME|VSLIDE|CLOSER|DNARROW|UPARROW|SIZER|SMALLER|FULLER    // |BORDER


/* text size */
#define TAB_NAME_SHOW_CHARS			24
#define PATHBAR_NAME_SHOW_CHARS		24
#define ENTRY_NAME_SHOW_CHARS		64
#define POPUP_ITEM_CHARS			32

/* Custom error code */
#define ECODEC		-10001
#define ERSC		-10002
#define EEDDI		-10003
#define EMINT		-10004
#define EWINDOW		-10005


/* default tab path */
#define T_HOME			0
#define T_ROOT			1
#define T_CDRIVE		2
#define T_CURRENTDIR	3


/* tab options */
#define TAB_AUTOHIDE					0x0001
#define TAB_CLOSE_WITH_DOUBLE_CLICK		0x0002


/* ram disk options */
#define RAMDISK_MOUNTASDRIVE	0x0001
#define RAMDISK_SAVETODISK		0x0002


/* view mode */
#define ICON_MODE		0
#define LIST_MODE		1


/*  Windows status */
#define WS_OPEN						0x0001		/* Window is opened						*/
#define WS_ICONIFY					0x0002		/* Window is iconified					*/
#define WS_UNTOPPABLE				0x0020		/* Window is untoppable					*/
#define WS_FORM						0x0040		/* Window is a formular					*/
#define WS_ALLICNF					0x0800		/* All windows are iconified			*/
#define WS_FULLSIZE					0x1000		/* Window has fulled size				*/
#define WS_INFOBAR					0x2000		/* Window owns the status bar			*/
#define WS_TOOLBAR					0x00010000	/* Window owns the toolbar				*/
#define WS_SIDEBAR					0x00020000	/* Window owns the side bar				*/
#define WS_PATHBAR					0x00040000	/* Window owns the path bar				*/
#define WS_TABBAR					0x00080000	/* Window owns the tabs bar				*/
#define WS_TABHIDDEN				0x00100000	/* At least a tab is hidden				*/
#define WS_LAST_TAB_IS_SELECTED		0x00200000	/* The last tab is selected				*/


/*  Entry status */
#define ES_DELETED			0x0001		/* The Entry has been deleted			*/
#define ES_DRIVE			0x0002		/* The Entry is a drive					*/
#define ES_DIR				0x0020		/* The Entry is a directory				*/
#define ES_HIDDEN			0x0040		/* The Entry is hidden					*/
#define ES_LINK				0x0800		/* The Entry is a link					*/
#define ES_LOCKED			0x1000		/* The Entry is locked					*/
#define ES_DISABLED			0x2000		/* The Entry is disabled( ex: *.prx)	*/
#define ES_THUMBCREATED     0x00010000	/* The thumbnail has been created		*/


/* Various definitions */
#define MINI_ICON_SIZE			16
#define STANDARD_ICON_SIZE		32
#define BIG_ICON_SIZE			48
#define HUGE1_ICON_SIZE			64
#define HUGE2_ICON_SIZE			96
#define HUGE3_ICON_SIZE			128
#define TABBAR_HEIGHT			20
#define INFOBAR_HEIGHT			18
#define TOOLBAR_HEIGHT			20
#define COLUMN_TITLE_HEIGHT		18
#define PATHBAR_HEIGHT			18
#define MINIMAL_TAB_WIDTH		64
#define MAXIMAL_TAB_WIDTH		128
#define MINIMAL_SIDEBAR_WIDTH	48
#define DEFAULT_SIDEBAR_WIDTH	120
#define MAX_POPUP_ITEM			32


/* MACRO */
#ifndef ABS
# define ABS(val) ((( val) < 0) ? -( val) : ( val))
#endif

#define IS_IN( xp, yp, x, y, w, h) ( xp >= x && yp >= y && xp < x + w && yp < y + h)

#ifndef ADR
# define ADR(c) (int16)((int32)(c)>>16), (int16)(int32)(c)	/* Convert a 32 bit adress value into a couple of 16 bit values */
#endif

#ifndef MIN
#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))
#endif

#ifndef FA_HIDDEN
#define FA_UPDATE       0x00
#define FA_RDONLY       0x01
#define FA_HIDDEN       0x02
#define FA_SYSTEM       0x04
#define FA_LABEL        0x08
#define FA_DIREC        0x10
#define FA_ARCH         0x20
#endif

#ifndef S_ISDIR
#define	S_ISDIR(mode)	 (((mode) & 0170000) == (0040000))
#define	S_ISCHR(mode)	 (((mode) & 0170000) == (0020000))
#define	S_ISBLK(mode)	 (((mode) & 0170000) == (0060000))
#define	S_ISREG(mode)	 (((mode) & 0170000) == (0100000))
#define S_ISFIFO(mode)	 (((mode) & 0170000) == (0120000))
#define S_ISLNK(mode)	 (((mode) & 0170000) == (0160000))
#define S_ISMEM(mode)	 (((mode) & 0170000) == (0140000))
#endif

#ifndef S_ISUID
#define S_ISUID			04000
#define S_ISGID			02000
#define S_ISVTX			01000
#endif

/* file access modes for user, group, and other*/
#ifndef S_IRUSR
#define S_IRUSR         0400
#define S_IWUSR         0200
#define S_IXUSR         0100
#define S_IRGRP         0040
#define S_IWGRP         0020
#define S_IXGRP         0010
#define S_IROTH         0004
#define S_IWOTH         0002
#define S_IXOTH         0001
#define DEFAULT_DIRMODE (0777)
#define DEFAULT_MODE    (0666)
#endif

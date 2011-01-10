/*
 * zDesk.
 * Copyright (c) 2010 Zorro ( zorro270@yahoo.fr)
 *
 * This application is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This application is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types2b.h"
#include <osbind.h>
#include <mintbind.h>
#include <unistd.h>
#include <errno.h>
#include <mt_gem.h>
#include <mt_gemx.h>
#include <ldg.h>
#include "pic_load.h"
#include "debug.h"
#include "defs.h"
#include "zdesk.rsh"


typedef struct
{
	int16  ph_branch;        /* Branch to start of the program (must be 0x601a!)	*/
	int32  ph_tlen;          /* Length of the TEXT segment							*/
	int32  ph_dlen;          /* Length of the DATA segment							*/
	int32  ph_blen;          /* Length of the BSS segment							*/
	int32  ph_slen;          /* Length of the symbol table							*/
	int32  ph_res1;          /* Reserved, should be 0								*/
	int32  ph_prgflags;      /* Program flags										*/
	int16  ph_absflag;       /* 0 = Relocation info present							*/
} PH;


typedef struct edit 
{
	char	buffer[MAXNAMLEN];	/* text buffer */
	int		len;				/* text lenght */	
	int		cursor_row;			/* cursor position */
	int		edition_mode;
} EDIT;	


struct _appvar;


typedef struct
{
	uint16	mode;
	int32	ino;	
	uint16	dev;	
	int16	rdev;	
	uint16	nlink;
	uint16	uid;	
	uint16	gid;	
	int32	size;
	int32	blksize;
	int32	blocks;
	uint32	mtime;
	uint32	atime;
	uint32	ctime;
	int16	attr;
	int16	res1;
	int32	res2[2];
} XATTR;


typedef struct
{
	LDG *ldg;

	/* Functions */
	void* CDECL (*ldg_malloc)	( int32, int32);
	void* CDECL (*ldg_realloc)	( void *, int32, int32);
	void  CDECL (*ldg_free)		( void *, int32);
	char* CDECL (*ldg_allocmode)( char *, int32);
} LDG_MEM;


typedef struct
{
	int16	codecs_nbr;				/* number of codecs loaded */
	LDG		*codec_ptr[50];			/* codecs pointer */

	/* Codecs functions */
	void	CDECL( *codec_init)( void);
	void 	CDECL( *decoder_quit)( img_info *);
	boolean	CDECL( *decoder_init)( const char *, img_info *);
	boolean	CDECL( *decoder_read)( img_info *, uint8 *);
} CODEC;



typedef struct
{
	/* window options */
	int 	show_toolbar;
	int 	show_pathbar;
	int 	show_sidebar;
	int 	show_infobar;
	
	/* file sorting */
	int 	sortingmode;
	int 	sort_dirs_before_files;
	
	/* file area */
	int		view_mode;
	int 	smooth_thumbnail;
	int 	show_hidden;
	int 	win_background_color;
	int 	win_txt_color;	
	int		win_highlight_color;
	int		win_highlight_style;
	int		win_txt_highlight_color;
	int		file_separator_style;	
	int		file_separator_color;
	
	/* the columns in list mode */
	int		selected_column_color;
	int		resize_column_in_realtime;	
	
	/* buttons and widgets */
	int		button_off_background;
	int		button_off_light_color;
	int		button_off_dark_color;
	int		button_off_text_color;
	int		button_off_text_shadow_color;
	int		button_on_background;
	int		button_on_light_color;
	int		button_on_dark_color;
	int		button_on_text_color;
	int		button_on_text_shadow_color;
	int		object_text_is_3D;
	
	/* tabs */
	int 	show_tabbar;	
	int		tab_off_background_color;
	int		tab_off_text_color;	
	int		tab_on_highlight_color;			/* -1 if none */
	int 	default_tab_path;				/* Where to open the tabs by default ? */
	int		tab_options;

	/* drives */
	int		ram_disk_option;	
	char 	skip_drive[32];
	
	/* desktop */
	char 	desk_pic[MAX_PATH+MAXNAMLEN];
	int 	desk_background_color;
} PREFERENCE;	


typedef struct 
{
	uint8 satur;
	uint8 red, green, blue;
} SRGB;



typedef struct
{
	SRGB 	screen_colortab[256];

	int16	pixel_val[256];
	uint32	cube216[216];
	uint32	graymap[32];
	int16	planar;

	void ( *raster)( dec_data *, void *, struct _appvar *);
	void ( *raster_cmap)( dec_data *, void *, struct _appvar *);
	void ( *raster_true)( dec_data *, void *, struct _appvar *);
	void ( *cnvpal_color)( img_info *, dec_data *, struct _appvar *);
	void ( *raster_gray)( dec_data *, void *, struct _appvar *);
	void ( *getPixel)( int, int, uint32, uint8*, uint8*, uint8*, uint8*);
	void ( *setPixel)( int, int, uint32, uint8, uint8, uint8, uint8*);
} RASTER;


typedef struct _entry
{
	char 		name[MAXNAMLEN];					/* The real file name	 					*/
	char 		name_shown[ENTRY_NAME_SHOW_CHARS];	/* The file name shown in the window		*/

	int32		status;								/* status of the file						*/

	uint32		file_size;
	uint32		m_time;								/* time of last modification, DOS			*/

	char 		tsize[16];							/* File size in human readable format		*/
  	char        tdate[32];							/* Date & time of last modification  		*/

	int16 		icon_txt_w;							/* Text width for "name_shown"	 			*/
	int16 		real_txt_w;							/* Text width for "name"		 			*/
	int16 		tsize_txt_w;						/* Text width for "tsize"					*/	
	int16		tdate_txt_w;						/* Text width for "tdate"					*/
	
	GRECT 		txt_pos;							/* text position in the window	 			*/
	GRECT 		icn_pos;							/* icon position in the window				*/
	GRECT 		box_pos;							/* box( containing the icon and the text) "	*/

	MFDB		thumbnail;							/* the preview data and picture				*/

	int16	 	icon;
	int16	 	mini_icon;

	struct _entry	*next_selected;
} Entry;


typedef struct _tab
{
	/* the tab */
	char 		name[MAXNAMLEN];					/* The repertory name	 					*/
	char 		name_shown[TAB_NAME_SHOW_CHARS];	/* The repertory name shown					*/
	int16		tab_xpos;
	int16		tab_width;							/* The width of the tab						*/
	int16 		real_txt_w;							/* Text width for "name"		 			*/	
	int16 		tab_txt_w;							/* Text width for "name_shown"	 			*/
	int16		tab_icon;							/* The icon index for the tab				*/
	char		directory[MAX_PATH];				/* current directory						*/	
	int16		sortingmode;
	int16		sort_dirs_before_files;
	int16		view_mode;
	int16		icon_size;							/* current icons size						*/
	int16		icons_last_line;					/* number of icons in the last line			*/
	
	Entry		*entry;								/* pointer to 'Entry' struct. 				*/
	Entry		*first_selected;					/* Pointer to the first icon selected		*/
	int16		nbr_icons;							/* number of valid icons  					*/
	
	/* for the sizer */
	int32		xpos, ypos;							/* relative data position in the window 	*/
	int32		xpos_max, ypos_max;					/* Maximal values of previous variables 	*/
	int16		w_u, h_u;							/* vertical and horizontal scroll offset	*/		
	
	/* "column" datas */
	int16		name_column_width;					/* width of the "name" column				*/	
	int16		size_column_width;					/* width of the "size" column				*/
	int16		date_column_width;					/* width of the "date" column				*/	
	
	/* Info bar data */
	char		total_files_size[32];				/* total files size							*/	
	char		info[256];							/* Infobar text								*/
	char		free_size_txt[48];					/* free space on the disk					*/
	int16		free_size_txt_w;					/* Text width for "free_size_txt"			*/
	
	/* virtual filesystem functions */	
	int32		( *rename_entry)( struct _tab *, char *, struct _appvar *);
	int32		( *scan_dir)( struct _tab *, char *, char *, struct _appvar *);
	void		( *get_disk_size)( struct _tab *, struct _appvar *);
	
	struct _tab	 *next;

} TAB;


typedef struct _pathbar
{
	char 		name[MAXNAMLEN];						/* The repertory name	 					*/
	char 		name_shown[PATHBAR_NAME_SHOW_CHARS];	/* The repertory name shown					*/
	int16		dir_xpos;
	int16		dir_width;								/* The width of the item					*/
	int16 		real_txt_w;								/* Text width for "name"		 			*/	
	int16 		dir_txt_w;								/* Text width for "name_shown"	 			*/

	struct _pathbar	 *next;
} PATHBAR;


typedef struct _window
{
	/* window's datas */
	int16		handle;								/* AES handle of the window 				*/
	int16		w_min, h_min;						/* minimal window size 						*/
	int32		status;								/* status of the window 					*/
	GRECT		uniconified_pos;					/* original coordinate/size of the uniconified window */

	/* Datas for the window in "iconified" mode */
	char		title_iconified[3];					/* Window's title in iconified mode			*/
	char		text_iconified[16];					/* Icon's text in iconified mode			*/
	int16		text_iconified_txt_w;				/* Icon's text width						*/	
	
	/* "folder tree" datas */
	int16		sidebar_width;						/* Sidebar width( left of the window)		*/	
	
	/* "Tab bar" datas */
	TAB			*tab;								/* The tab datas							*/
	TAB			*selected_tab;
	int16		nbr_tabs;							/* Number of tabs							*/
	
	EDIT		edit;								/* Text struct for the edition mode			*/
	
	/* AES Event functions */
	void		( *WinRedraw)( struct _window *, struct _appvar *);			/* WM_REDRAW		*/		
	void		( *WinIconifyRedraw)( struct _window *, struct _appvar *);	/* WM_ICONDRAW		*/		
	void		( *WinClose)( struct _window *, struct _appvar *);			/* WM_CLOSED		*/
	void		( *WinTop)( struct _window *, struct _appvar *);			/* WM_TOPPED		*/	
	void		( *WinBottom)( struct _window *, struct _appvar *);			/* WM_BOTTOMED		*/	
	void		( *WinMove)( struct _window *, struct _appvar *);			/* WM_MOVED			*/
	void		( *WinSize)( struct _window *, struct _appvar *);			/* WM_SIZED			*/
	void		( *WinFull)( struct _window *, struct _appvar *);			/* WM_FULLED		*/
	void		( *WinIconify)( struct _window *, struct _appvar *);		/* WM_ICONIFY		*/
	void		( *WinUniconify)( struct _window *, struct _appvar *);		/* WM_UNICONIFY		*/
	void		( *WinAlliconify)( struct _window *, struct _appvar *);		/* WM_ALLICONIFY	*/
	void		( *WinArrow)( struct _window *, struct _appvar *);			/* WM_ARROWED		*/
	void		( *WinVslide)( struct _window *, struct _appvar *);			/* WM_VSLID			*/
	void		( *WinKeyboard)( struct _window *, struct _appvar *);		/* WM_XKEYBD		*/
	void		( *WinMouse)( struct _window *, struct _appvar *);			/* WM_XBUTTON		*/

	/* Next window's pointer */
	struct _window	*next;
} WINDOW;


typedef struct
{
	char		item_name[MAX_POPUP_ITEM][POPUP_ITEM_CHARS];
	GRECT		item_position[MAX_POPUP_ITEM];
	int16		item_icon[MAX_POPUP_ITEM];		
	int16		item_selectable[MAX_POPUP_ITEM];
	int16		nbr_items;
	int16		selected, old_selected;
	GRECT		popup_position;
} POPUP;


typedef struct
{
	WINDOW		*win;
	OBJECT		*rsc;
	int			icon;
	int			mini_icon;
	char		file_path[MAX_PATH];	
	char 		file_name[MAXNAMLEN];
	PH			prg_header;
	XATTR		file_xattr;
} INFO_BOX;


typedef struct _appvar
{
	int16		aes_global[16];						/* The AES global array (use for mt_AES calls)					*/
	int16		aes_buff[8];						/* Buffer for the AES messages									*/	
	int16		aes_handle;
	int16		vdi_handle;							/* The screen VDI handle used by AES							*/		
	int16		x, y, w, h;    						/* Coordinates and size of desktop workarea (not screen size)	*/
	int16		mx, my, mbut, 
				mkstate, keybd, nb_click;			/* Mouse and keyboard states									*/
	int16		nplanes;    						/* Number of planes of screen display 							*/

	int16 		x_space, y_space, hcell; 			/* Default value for some WINDOW variables.						*/
	
	char		zdesk_path[MAX_PATH];
	MFDB		desktop_picture;

	OBJECT		*desktop_rsc;
	OBJECT		*menu_rsc;
	OBJECT		*icon_rsc;
	OBJECT		*about_rsc;
	
	/* Text pointers for the infobar */
	char		*info_simple;
	char		*info_multi;
	char		*info_selected_simple;
	char		*info_selected_multi;
	char		*info_available;
	/* Text pointers for the files */
	char		*file_today;
	char		*file_yesterday;
	/* Text pointers and datas for the columns in list mode */
	char		*name_title;
	char		*size_title;
	char		*date_title;
	int16 		size_title_txt_w;					/* Text width for "size_title"		 			*/
	int16 		date_title_txt_w;					/* Text width for "date_title"					*/

	INFO_BOX	*infobox;
	POPUP		popup;
	LDG_MEM		ldg_mem;
	CODEC		codecs;
	RASTER		raster;
	PREFERENCE	pref;

	WINDOW		*about_win;
	WINDOW		*win;								/* the windows list				*/

	/* Workaround for TOS( and maybe MagiC) and its small supervisor stack:			*/
	/* The userdraw objects( like the desktop) can't use many locals variables 		*/
	/* in their redraw functions; so we put the variables here and not				*/
	/* as global variable to keep zdesk's functions reentrant.						*/
	uint8		buffer[8192];
	int16		pxy[10], color[2];
	MFDB 		pic, screen;
	int32		src_line_octets;
} APPVar;



extern int pic_load( const char *file, int16 thumbnail, MFDB *img, APPVar *app);
extern int16 CDECL draw_text( int16 xf, int16 yf, int16 txt_w, int16 color, int16 shadow_color, const char *str, APPVar *app);

#include "form/form.h"
#include "mem.h"

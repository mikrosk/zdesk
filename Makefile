STACKSIZE		= 256k
OPTIMISATION		= -O2 -fomit-frame-pointer
CPU			= -m68020-60
LIB			= -lgem -lldg
CFLAGS			= $(CPU) $(OPTIMISATION) -DDEBUG -DUSE_ASM -Wall -Wshadow -I.

DESK_OBJ		= desktop/desktop.o desktop/scan_desktop.o
RASTER_OBJ		= raster/resample.o raster/color.o raster/dither.o				\
			  raster/raster.o raster/pixel.o raster/vdi.o
FORM_OBJ		= form/alert.o form/about.o form/info.o
ICON_OBJ		= icon/set_icon.o icon/draw_icon.o
FILE_OBJ		= file/sort.o file/file.o file/rename.o file/get_disk_size.o
WINFILE_OBJ		= winfile/WinFile_New.o winfile/WinFile_Tab.o winfile/WinFile_Update_Icon_Position.o	\
			  winfile/WinFile_Calc_Areas.o winfile/WinFile_Iconify.o winfile/WinFile_Slider.o 	\
			  winfile/WinFile_Other_Event.o winfile/WinFile_Close.o 			\
			  winfile/WinFile_Entry.o winfile/WinFile_Mouse.o 				\
			  winfile/WinFile_Exec_Entry.o winfile/WinFile_Sizer.o winfile/WinFile_Popup.o		\
			  winfile/WinFile_Rubberbox.o winfile/WinFile_Frame_Sizer.o winfile/WinFile_Keyb.o 	\
			  winfile/WinFile_Infobar.o winfile/WinFile_Tabbar.o winfile/WinFile_Redraw.o

OBJ 			= $(RASTER_OBJ) $(FILE_OBJ) $(ICON_OBJ) $(WINFILE_OBJ)			\
			  $(FORM_OBJ) $(DESK_OBJ) misc.o debug.o custom_font.o			\
			  ztext.o appl.o codecs.o mfdb.o mem.o pic_load.o	rename_entry.o	\
			  edit.o prefs.o menu.o evnt_loop.o main.o

PROGRAM 		= zdesk.app

# this is shared along all Zorro's products but let's use separate copy here
include common.mak

#ifndef SCANDIRFILE_H
#define SCANDIRFILE_H

#ifdef __cplusplus
extern "C" {
#endif

int mychdir( char *fullpath_dir);
int32 scan_dir_file( TAB *tab, char *current_fullpath_dir, char *new_dir, APPVar *app);
int32 check_file( char *full_path_name);

#ifdef __cplusplus
}
#endif

#endif


//
// file.h
//
// Copyright (c) 2000 IPA and Keio University SFC Research Institution
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//

#ifndef _FILE_H
#define _FILE_H

#ifdef __cplusplus
extern "C" {
#endif

// defines
#define comment(c) (c == '\n' || c == '!' || c == '%' || c == '#' || c == '*')

// functions
extern void find_fileheader(char *, char *);
extern void find_filename(char *, char *);
extern void filehead(char *, char *);
extern char *open_filename(char *, char *, char *);
extern char *cut(char *, char *);
extern void display(char *, ...);
extern void display_file_open(char *);
extern void display_file_close(void);

#ifdef __cplusplus
}
#endif

#endif // _FILE_H


//
// Bitmap.h
//
// Copyright (c) 2000 IPA and Keio University SFC Research Institution
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php
//

#ifndef _BITMAP_H
#define _BITMAP_H

extern BOOL SetupPixelFormat( HDC , CPalette **, DWORD, int );
extern BOOL CreateRGBPalette( HDC , CPalette ** );
extern void setupBmpHeader(BITMAPINFOHEADER *, int, int, int);
extern void BmpSave( char *, ScreenAtr * );
extern void DibSave( char *, ScreenAtr * );
extern void oglGetImage( int, int, unsigned char * );

#endif // _BITMAP_H

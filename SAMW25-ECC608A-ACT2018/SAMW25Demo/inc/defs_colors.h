/*******************************************************************************
* Copyright (c) 2018 Arrow Electronics, Inc.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Apache License 2.0
* which accompanies this distribution, and is available at
* http://apache.org/licenses/LICENSE-2.0
*
* Contributors:
*     IoT Engineering Group - Arrow Electronics, Inc.
*******************************************************************************/
#ifndef __DEFS_COLORS__
#define __DEFS_COLORS__

// This header file can be used for setting output console colors
// on VT100 consoles. 


#define FOREGROUND_COLOR_NORMAL	'3'
#define FOREGROUND_COLOR_LIGHT	'9'

#define FGCOLORSET		FOREGROUND_COLOR_NORMAL

#define VTCOLOR_BLACK	'0'
#define VTCOLOR_RED		'1'
#define VTCOLOR_GREEN	'2'
#define VTCOLOR_YELLOW	'3'
#define VTCOLOR_BLUE	'4'
#define VTCOLOR_MAGENTA	'5'
#define VTCOLOR_CYAN	'6'
#define VTCOLOR_GRAY	'7'
#define VTCOLOR_DEFAULT	'9'

#define FOREGROUND_COLOR(B,C)		printf("%c%c%c%c%c", 0x1B, '[', (B), (C),'m')

#define RED()		FOREGROUND_COLOR(FGCOLORSET, VTCOLOR_RED)
#define GREEN()		FOREGROUND_COLOR(FGCOLORSET, VTCOLOR_GREEN)
#define YELLOW()	FOREGROUND_COLOR(FGCOLORSET, VTCOLOR_YELLOW)
#define BLUE()		FOREGROUND_COLOR(FGCOLORSET, VTCOLOR_BLUE)
#define WHITE()		FOREGROUND_COLOR(FGCOLORSET, VTCOLOR_GRAY)
#define MAGENTA()	FOREGROUND_COLOR(FGCOLORSET, VTCOLOR_MAGENTA)
#define DEFAULT()	FOREGROUND_COLOR(FGCOLORSET, VTCOLOR_GRAY)

#define NORMAL()	printf("%c%c%c%c", 0x1B, '[', '0', 'm')
#define BOLD()		printf("%c%c%c%c", 0x1B, '[', '1', 'm')



#endif

// EOF

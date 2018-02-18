#ifndef __GSIEGE_INCLUDES_HEADER__
#define __GSIEGE_INCLUDES_HEADER__

// Hopefully, I know what I am doing. >:)
#ifdef _MSC_VER
	#define _CRT_SECURE_NO_WARNINGS // Disable warning for using non-secure versions of C functions in Visual Studio
#endif

/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <windows.h>
#include <mmsystem.h>
#include "MainWindow.h"
#include "TabWindow.h"
#include "ChatWindow.h"
#include "FlameRam.h"
#include "Catapult.h"
#include "Trebuchet.h"
#include "Bot.h"
#include "Menu.h"
#include "SendKeys.h"
#include "SendKeysQue.h"
#include "resource.h"
#include "Globals.h"
*/

#pragma comment(lib, "winmm.lib")  // timeEndPeriod()/timeBeginPeriod() (deprecated, slotted for removal)

#endif //__GSIEGE_INCLUDES_HEADER__
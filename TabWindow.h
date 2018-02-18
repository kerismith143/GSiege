#ifndef __GSIEGE_TABWINDOW_HEADER__DRKHJ4X8C3GV5NH87CM43XJKK89C4__
#define __GSIEGE_TABWINDOW_HEADER__DRKHJ4X8C3GV5NH87CM43XJKK89C4__
#include <windows.h>

// Start/Stop button state defines
#define SHOWSTART	0x00
#define SHOWSTOP	0x01

unsigned int TAB_CHAT_HEIGHT;
#define TAB_STATIC_HEIGHT	16
#define TAB_BUT_WIDTH		45
#define TAB_CHAT_MAXTEXT	SKCHARMAX

int g_nCWpadding;

void TabWindowClassBuilder(); // Create and register the tab elements
//int PaintTabs(); // Repaint tabs for updating
int ShowTab(HWND hWnd); // Select a specific tab for displaying its elements
void SetStartStop(int nStartStop); // Set the text for the Start/Stop buttons
int GetStartStop(); // Retrieve the state of the Start/Stop buttons

#endif //__GSIEGE_TABWINDOW_HEADER__DRKHJ4X8C3GV5NH87CM43XJKK89C4__

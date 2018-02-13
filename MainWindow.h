#ifndef __GSIEGE_MAINWINDOW_HEADER__
#define __GSIEGE_MAINWINDOW_HEADER__

#define GSIEGE_MAIN_CLASS	"GSiegeMainWindow"

// Deffault button width and height
#define DEFBUT_WIDTH		70
#define DEFBUT_HEIGHT		20

// Create and register the main window class
HWND MainWindowClassBuilder();
void SetWindowState();

#endif //__GSIEGE_MAINWINDOW_HEADER__
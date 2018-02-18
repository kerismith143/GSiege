//#include "includes.h"
#include <stdint.h>
#include <time.h>
#include <windows.h>
#include "Globals.h"
#include "SignalQue.h"
#include "SendKeys.h"
#include "SendKeysQue.h"
#include "MainWindow.h"
#include "TabWindow.h"
#include "ChatWindow.h"
#include "FlameRam.h"
#include "Catapult.h"
#include "Trebuchet.h"

// Signal Que state counter
static time_t StateCounter;

uint8_t SignalQueProcessor()
{
	uint8_t SignalMask = 0;
	uint8_t SIGNAL = 0;

	// encapsulate the current state check to obfuscate the state update
	{
		time_t CurrentCounter;
		CurrentCounter = time(NULL);
		if ( StateCounter != CurrentCounter ) StateCounter = CurrentCounter;
	}

	// SIGNAL = SignalQueNext();
	// case ( SIGNAL )
	// [...]

	// case INTERRUPT_EJECT:
	// {
	//     SignalQueRemove(SIGNAL_START);
	//     SignalQueRemove(SIGNAL_STOP);
	//     SignalQueRemove(INTERRUPT_EJECT);
	//     [...]
	
	// OR

	// if ( SIGNAL && INTERRUPT_EJECT )
	// {
	//     uint8_t SignalMask = 0x00;
	//     SignalMask |= SIGNAL_START;
	//     SignalMask |= SIGNAL_STOP;
	//     SignalMask |= INTERRUPT_EJECT;
	//     SignalQueRemove(SignalMask);
	//     [...]

	return 1;
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;

	StateCounter = 0; // set StateCounter for signal Que initialisation

	timeBeginPeriod(SLEEPMIN);
	SKQueInit();

	memset(&g_GlobalSettings, 0, sizeof(g_GlobalSettings));
	memset(&g_UserSettings, 0, sizeof(g_UserSettings));

	g_UserSettings.MajorVersion = GSIEGE_MAJOR_VERSION;
	g_UserSettings.MinorVersion = GSIEGE_MINOR_VERSION;

	// Initialise global variables
	g_GlobalSettings.hInstance = hInstance;
	g_GlobalSettings.hPrevInstance = hPrevInstance;
	g_GlobalSettings.nCmdShow = nCmdShow;
	g_GlobalSettings.dwThreadId = 0;

	// Set font
	g_GlobalSettings.hFont = CreateFont(11,6,0,0,0,0,0,0,0,0,0,0,0,TEXT("Tahoma"));
	g_GlobalSettings.hBoldFont = CreateFont(11,6,0,0,FW_BOLD,0,0,0,0,0,0,0,0,TEXT("Tahoma"));

	// Build windows
	LoadSettings();
	MainWindowClassBuilder();
	TabWindowClassBuilder();
	FlameRamFormBuilder();
	CatapultFormBuilder();
	TrebuchetFormBuilder();
	ChatWindowClassBuilder();
	FlameRamToggleElements();

	// Start main procedure loop
	while ( GetMessage(&msg, NULL, 0, 0) > 0 )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// Cleanup before exit
	if ( g_GlobalSettings.dwThreadId )
		TerminateThread(g_GlobalSettings.hThread, 0);
	if ( g_GlobalSettings.dwIdleThreadId )
		TerminateThread(g_GlobalSettings.hIdleThread, 0);
	DeleteObject(g_GlobalSettings.hFont);
	DeleteObject(g_GlobalSettings.hBoldFont);

	timeEndPeriod(SLEEPMIN);

	return msg.wParam;
}

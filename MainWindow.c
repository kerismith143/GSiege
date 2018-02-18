#include <windows.h>
#include "resource.h"
#include "Globals.h"
#include "TabWindow.h"
#include "Menu.h"
#include "MainWindow.h"

void SetWindowTopmost()
{
	if ( g_UserSettings.OnTop )
	{
		SetWindowPos(g_GlobalSettings.hMainWindow, HWND_TOPMOST,
			g_UserSettings.MainX, g_UserSettings.MainY,
			g_UserSettings.MainWidth, g_UserSettings.MainHeight, 0);
	}
	else
	{
		SetWindowPos(g_GlobalSettings.hMainWindow, HWND_NOTOPMOST,
			g_UserSettings.MainX, g_UserSettings.MainY,
			g_UserSettings.MainWidth, g_UserSettings.MainHeight, 0);
	}
}

void SetWindowState()
{
	if ( g_UserSettings.EnableAdv ) ShowWindow(g_GlobalSettings.hOffset, TRUE);
	else ShowWindow(g_GlobalSettings.hOffset, FALSE);
	if ( g_UserSettings.EnableAdv ) ShowWindow(g_GlobalSettings.hOffsetStatic, TRUE);
	else ShowWindow(g_GlobalSettings.hOffsetStatic, FALSE);

	if ( g_GlobalSettings.SelectedTab == g_GlobalSettings.hSiegeTab[0] )
		FlameRamToggleElements();
	else if ( g_GlobalSettings.SelectedTab == g_GlobalSettings.hSiegeTab[1] )
		CatapultToggleElements();
	else if ( g_GlobalSettings.SelectedTab == g_GlobalSettings.hSiegeTab[2] )
		TrebuchetToggleElements();

	if ( g_UserSettings.EnableChat )
	{
		TAB_CHAT_HEIGHT = TAB_STATIC_HEIGHT;
		ShowWindow(g_GlobalSettings.hChatButton, TRUE);
		ShowWindow(g_GlobalSettings.hChatWindow, TRUE);
		UpdateWindow(g_GlobalSettings.hChatButton);
		UpdateWindow(g_GlobalSettings.hChatWindow);
		UpdateWindow(g_GlobalSettings.hOffset);
		g_UserSettings.MainHeight = (MAINHEIGHT + GetSystemMetrics(SM_CYMENU)) + TAB_CHAT_HEIGHT;
		InvalidateRect(g_GlobalSettings.hMainWindow, NULL, FALSE);
	}
	else
	{
		TAB_CHAT_HEIGHT = 0;
		ShowWindow(g_GlobalSettings.hChatButton, FALSE);
		ShowWindow(g_GlobalSettings.hChatWindow, FALSE);
		UpdateWindow(g_GlobalSettings.hChatButton);
		UpdateWindow(g_GlobalSettings.hChatWindow);
		UpdateWindow(g_GlobalSettings.hOffset);
		g_UserSettings.MainHeight = MAINHEIGHT + GetSystemMetrics(SM_CYMENU);
		InvalidateRect(g_GlobalSettings.hMainWindow, NULL, FALSE);
	}
	SetWindowTopmost();
}

// Main window procedure to control the program
LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch ( msg )
	{
		// Process click messages
		case WM_COMMAND:
		{
			ShowTab((HWND)lParam); // Set tab states
			ModifyEdits((HWND)lParam);
			FlameRamCheckState((HWND)lParam); // Check processing for Flame Ram tab and elements
			CatapultCheckState((HWND)lParam); // Check processing for Catapult tab and elements
			TrebuchetCheckState((HWND)lParam); // Check processing for Trbuchet tab and elements
			MenuCheckState(hWnd, msg, wParam, lParam);
			if ( ((HWND)lParam) == g_GlobalSettings.hChatButton )
				SendChat();
			break;
		}
		// Process Scrollbar messages
		case WM_HSCROLL:
		{
			int nValue;
			char buffer[32];

			// Set global values for Scrollbar
			if ( ((HWND)lParam) == g_GlobalSettings.hCatSliderBar )
			{
				if ( g_UserSettings.CatRange < g_UserSettings.CatMin )
					nValue = g_UserSettings.CatMin;
				else if ( g_UserSettings.CatRange > g_UserSettings.CatMax )
					nValue = g_UserSettings.CatMax;
				else
					nValue = g_UserSettings.CatRange;
			}
			else if ( ((HWND)lParam) == g_GlobalSettings.hTrebSliderBar )
			{
				if ( g_UserSettings.TrebRange < g_UserSettings.TrebMin )
					nValue = g_UserSettings.TrebMin;
				else if ( g_UserSettings.TrebRange > g_UserSettings.TrebMax )
					nValue = g_UserSettings.TrebMax;
				else
					nValue = g_UserSettings.TrebRange;
			}
			else
				break;

			// Process Scrollbar message types
			switch ( LOWORD(wParam) )
			{
				case SB_PAGELEFT:
				case SB_LINELEFT:
					if ( (nValue - 1) >= 0 ) nValue--; // Decrease by 1 unit
					break;

				case SB_PAGERIGHT:
				case SB_LINERIGHT:
					if ( ((HWND)lParam) == g_GlobalSettings.hCatSliderBar )
					{
						if ( (nValue + 1) <= g_UserSettings.CatMax ) nValue++; // Increase by 1 unit
					}
					else if ( ((HWND)lParam) == g_GlobalSettings.hTrebSliderBar )
					{
						if ( (nValue + 1) <= g_UserSettings.TrebMax ) nValue++; // Increase by 1 unit
					}
					break;

				case SB_LEFT:
				case SB_RIGHT:
				case SB_THUMBPOSITION:
				case SB_THUMBTRACK:
					nValue = HIWORD(wParam); // Set nValue to selected value
					break;

				default:
					break;
			}
			// Set Scrollbar selector to nValue
			SetScrollPos((HWND)lParam, SB_CTL, nValue, TRUE);

			// Set range readout for Catapult Scrollbar
			if ( ((HWND)lParam) == g_GlobalSettings.hCatSliderBar )
			{
				g_UserSettings.CatRange = nValue;

				memset(buffer, 0, sizeof(buffer));
				// Calculate range to the 10ths
				_snprintf(buffer, sizeof(buffer)-1, "Range: %.2f", (((float)(g_UserSettings.CatRange - g_UserSettings.CatMin) / (float)(g_UserSettings.CatMax - g_UserSettings.CatMin)) * 100) );
				SendMessage(g_GlobalSettings.hCatSliderValue, WM_SETTEXT, 0, (LPARAM)buffer);
				// Set Catapult state values
				CatapultCheckState((HWND)lParam);
			}
			// Set range readout for Trebuchet Scrollbar
			else if ( ((HWND)lParam) == g_GlobalSettings.hTrebSliderBar )
			{
				g_UserSettings.TrebRange = nValue;

				memset(buffer, 0, sizeof(buffer));
				// Calculate range to the 10ths
				_snprintf(buffer, sizeof(buffer)-1, "Range: %.2f", (((float)(g_UserSettings.TrebRange - g_UserSettings.TrebMin) / (float)(g_UserSettings.TrebMax - g_UserSettings.TrebMin)) * 100) );
				SendMessage(g_GlobalSettings.hTrebSliderValue, WM_SETTEXT, 0, (LPARAM)buffer);
				// Set Trebuchet state values
				TrebuchetCheckState((HWND)lParam);
			}

			break;
		}
		// Build main window and relocate to last known position
		case WM_CREATE:
		{
			WINDOWPLACEMENT wp;
			HICON hMainIcon;

			// Load main program icon for window
			hMainIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MAINICON));
			SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hMainIcon);

			// Restore last known coordinates
			memset(&wp, 0, sizeof(wp));
			wp.length = sizeof(WINDOWPLACEMENT);
			GetWindowPlacement(hWnd, &wp);
			g_UserSettings.MainX = wp.rcNormalPosition.left;
			g_UserSettings.MainY = wp.rcNormalPosition.top;

			break;
		} //WM_CREATE
		case WM_SIZE:
		{
			SetWindowState();
			break;
		}
		// Get window placement information
		case WM_MOVE:
		{
			WINDOWPLACEMENT wp;

			wp.length = sizeof(WINDOWPLACEMENT);
			GetWindowPlacement(hWnd, &wp);
			g_UserSettings.MainX = wp.rcNormalPosition.left;
			g_UserSettings.MainY = wp.rcNormalPosition.top;
			break;
		}
		// Cleanup threads and windows, and save settings
		case WM_CLOSE:
			SaveSettings();
			if ( g_GlobalSettings.dwThreadId )
				TerminateThread(g_GlobalSettings.hThread, 0);
			DestroyWindow(hWnd);
			break;
		// Cleanup threads and windows, and save settings
		case WM_DESTROY:
			SaveSettings();
			if ( g_GlobalSettings.dwThreadId )
				TerminateThread(g_GlobalSettings.hThread, 0);
			PostQuitMessage(0);
			break;
		// Pass all other messages
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

// Builds the GSiegeWindowClass main class and registers it
HWND MainWindowClassBuilder()
{
	WNDCLASSEX wc;
	RECT rc;

	// Load user settings
	g_UserSettings.MainWidth = MAINWIDTH;
	if ( g_UserSettings.EnableChat )
	{
		TAB_CHAT_HEIGHT = TAB_STATIC_HEIGHT;
		g_UserSettings.MainHeight = (MAINHEIGHT + GetSystemMetrics(SM_CYMENU)) + TAB_CHAT_HEIGHT;
	}
	else
	{
		TAB_CHAT_HEIGHT = 0;
		g_UserSettings.MainHeight = MAINHEIGHT + GetSystemMetrics(SM_CYMENU);
	}

	// Define class properties and register.
	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= 0;
	wc.lpfnWndProc		= MainWindowProc;
    wc.cbClsExtra		= 0;
    wc.cbWndExtra		= 0;
    wc.hInstance		= g_GlobalSettings.hInstance;
    wc.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground	= (HBRUSH)(COLOR_WINDOW);
    wc.lpszMenuName		= GSIEGE_MENU_NAME;
    wc.lpszClassName	= GSIEGE_MAIN_CLASS;
    wc.hIconSm			= LoadIcon(NULL, IDI_APPLICATION);

	if( !RegisterClassEx(&wc) )
	{
		MessageBox(NULL, "Failed to register g_szMainWindow class.", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	// Build main window.
	g_GlobalSettings.hMainWindow = CreateWindowEx(
		WS_EX_WINDOWEDGE,
		GSIEGE_MAIN_CLASS,
		GSIEGE_VERSION_STRING,
		WS_OVERLAPPEDWINDOW,
		(g_UserSettings.MainX ? g_UserSettings.MainX : CW_USEDEFAULT),
		(g_UserSettings.MainY ? g_UserSettings.MainY : CW_USEDEFAULT),
		g_UserSettings.MainWidth, g_UserSettings.MainHeight,
		NULL, NULL, g_GlobalSettings.hInstance, NULL);

	if( g_GlobalSettings.hMainWindow == NULL )
	{
		MessageBox(NULL, "Failed to create g_hMainWindow.", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	// Load menu items
	g_GlobalSettings.hMenu = GetMenu(g_GlobalSettings.hMainWindow);

	// Set window position in z-order
	SetWindowState();
	SetMenuState();

	memset(&rc, 0, sizeof(rc));
	GetClientRect(g_GlobalSettings.hMainWindow, &rc);
	g_UserSettings.ClientWidth = rc.right - rc.left;
	g_UserSettings.ClientHeight = rc.bottom - rc.top;

	// Draw main window.
	ShowWindow(g_GlobalSettings.hMainWindow, g_GlobalSettings.nCmdShow);
	UpdateWindow(g_GlobalSettings.hMainWindow);
	return g_GlobalSettings.hMainWindow;
}

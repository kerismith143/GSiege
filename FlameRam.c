#include "includes.h"

HWND hRechargeStatic;

int FlameRamHideElements()
{
	ShowWindow(g_GlobalSettings.hRamDurationBox, SW_HIDE);
	ShowWindow(g_GlobalSettings.hRamStartButton, SW_HIDE);
	ShowWindow(g_GlobalSettings.hRamMasteryCheckbox, SW_HIDE);
	ShowWindow(g_GlobalSettings.hRamRecharge, SW_HIDE);
	ShowWindow(hRechargeStatic, SW_HIDE);
	InvalidateRect(g_GlobalSettings.hMainWindow, NULL, FALSE);
	return 1;
}

int FlameRamShowElements()
{
	if ( g_UserSettings.EnableAdv ) ShowWindow(g_GlobalSettings.hRamRecharge, g_GlobalSettings.nCmdShow);
	else ShowWindow(g_GlobalSettings.hRamRecharge, SW_HIDE);
	if ( g_UserSettings.EnableAdv ) ShowWindow(hRechargeStatic, g_GlobalSettings.nCmdShow);
	else ShowWindow(hRechargeStatic, SW_HIDE);
	UpdateWindow(g_GlobalSettings.hRamRecharge);
	UpdateWindow(hRechargeStatic);

	ShowWindow(g_GlobalSettings.hRamDurationBox, g_GlobalSettings.nCmdShow);
	UpdateWindow(g_GlobalSettings.hRamDurationBox);

	ShowWindow(g_GlobalSettings.hRamStartButton, g_GlobalSettings.nCmdShow);
	UpdateWindow(g_GlobalSettings.hRamStartButton);

	ShowWindow(g_GlobalSettings.hRamMasteryCheckbox, g_GlobalSettings.nCmdShow);
	UpdateWindow(g_GlobalSettings.hRamMasteryCheckbox);

	InvalidateRect(g_GlobalSettings.hMainWindow, NULL, FALSE);
	return g_GlobalSettings.nCmdShow;
}

void FlameRamToggleElements()
{
	int n;

	// Hide
	CatapultHideElements();
	TrebuchetHideElements();

	// Show
	FlameRamShowElements();

	// Set bold font and repaint
	SendMessage(g_GlobalSettings.hSiegeTab[0], WM_SETFONT, (WPARAM)g_GlobalSettings.hBoldFont, 0); // Flame Ram
	SendMessage(g_GlobalSettings.hSiegeTab[1], WM_SETFONT, (WPARAM)g_GlobalSettings.hFont, 0); // Catapult
	SendMessage(g_GlobalSettings.hSiegeTab[2], WM_SETFONT, (WPARAM)g_GlobalSettings.hFont, 0); // Trebuchet
	g_GlobalSettings.SelectedTab = g_GlobalSettings.hSiegeTab[0];
	for ( n = 0; n < MAXTABS; n++ )
		InvalidateRect(g_GlobalSettings.hSiegeTab[n], NULL, TRUE);
}

void FlameRamSetMastery()
{
	int nState;
	char buffer[32];

	memset(buffer, 0, sizeof(buffer));
	nState = SendMessage(g_GlobalSettings.hRamMasteryCheckbox, BM_GETCHECK, 0, 0);
	switch ( nState )
	{
		case BST_UNCHECKED:
			g_UserSettings.RamMastery = 0;
			_snprintf(buffer, sizeof(buffer)-1, "%lu", g_UserSettings.RamNoMastRecharge);
			SendMessage(g_GlobalSettings.hRamRecharge, WM_SETTEXT, 0, (LPARAM)buffer);
			break;
		case BST_CHECKED:
			g_UserSettings.RamMastery = 1;
			_snprintf(buffer, sizeof(buffer)-1, "%lu", g_UserSettings.RamMastRecharge);
			SendMessage(g_GlobalSettings.hRamRecharge, WM_SETTEXT, 0, (LPARAM)buffer);
			break;
		default:
			g_UserSettings.RamMastery = 0;
			_snprintf(buffer, sizeof(buffer)-1, "%lu", g_UserSettings.RamNoMastRecharge);
			SendMessage(g_GlobalSettings.hRamRecharge, WM_SETTEXT, 0, (LPARAM)buffer);
			SendMessage(g_GlobalSettings.hRamMasteryCheckbox, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
			break;
	}
}

void FlameRamStartBot()
{
	if ( g_GlobalSettings.BOT_EXECUTE )
	{
		DWORD dwDuration;

		// Set bot execution control variable
		g_GlobalSettings.BOT_EXECUTE = 0;

		memset(&g_tpp, 0, sizeof(g_tpp));

		// Stop bot thread and set form elements
		if ( g_GlobalSettings.dwThreadId )
		{
			GetExitCodeThread(g_GlobalSettings.hThread, &dwDuration);
			g_GlobalSettings.dwThreadId = 0;
		}
	}
	else if ( GetStartStop() )
	{
		// Set bot execution control variable
		g_GlobalSettings.BOT_EXECUTE = 1;

		g_tpp.nType = BOT_RAM;

		// Start bot thread and set form elements
		g_GlobalSettings.hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&TimerProc, &g_tpp, 0, &g_GlobalSettings.dwThreadId); 
		if ( g_GlobalSettings.hThread != NULL )
			SetStartStop(SHOWSTOP);
	}
}

void FlameRamCheckState(HWND hWnd)
{
	if ( hWnd == g_GlobalSettings.hRamMasteryCheckbox )
		FlameRamSetMastery();
	else if ( hWnd == g_GlobalSettings.hRamStartButton )
		FlameRamStartBot();
}

int FlameRamFormBuilder()
{
	char buffer[32];
	int nPadding; // Padding between elements
	int nMasteryCheckY, nMasteryCheckWidth, nMasteryCheckHeight; // Checkbox button position
	int nRechargeY, nRechargeWidth, nRechargeHeight, nOffsetY, nOffsetHeight;
	int nRechargeStaticX, nRechargeStaticY, nRechargeStaticWidth;
	int nFRSBx, nFRSBwidth, nFRSBheight; // Start button position
	int nFRDBx, nFRDBy, nFRDBwidth, nFRDBheight; // Duration static position

	nPadding = 20;

	// Mastery checkbox position
	nMasteryCheckY = DEFBUT_HEIGHT + nPadding;
	nMasteryCheckWidth = g_UserSettings.ClientWidth - (nPadding * 2);
	nMasteryCheckHeight = nPadding;

	nOffsetY = (nPadding / 2) + nMasteryCheckY + (TAB_CHAT_HEIGHT ? TAB_CHAT_HEIGHT : TAB_STATIC_HEIGHT);
	nOffsetHeight = (nPadding / 2);

	// Recharge Edit
	nRechargeY = (nPadding / 2) + nOffsetY + nOffsetHeight;
	nRechargeWidth = nPadding *2;
	nRechargeHeight = TAB_CHAT_HEIGHT ? TAB_CHAT_HEIGHT : TAB_STATIC_HEIGHT;
	nRechargeStaticX = nPadding + nRechargeWidth + 5;
	nRechargeStaticY = nRechargeY + 2;
	nRechargeStaticWidth = 60;

	// Start button position
	nFRSBwidth = DEFBUT_WIDTH;
	nFRSBheight = DEFBUT_HEIGHT;
	nFRSBx = (g_UserSettings.ClientWidth - nPadding) - DEFBUT_WIDTH;
	g_nSBy = (g_UserSettings.ClientHeight - nPadding) - DEFBUT_HEIGHT - TAB_CHAT_HEIGHT;

	// Duration timer position
	nFRDBx = nPadding;
	nFRDBy = (g_UserSettings.ClientHeight - nPadding) - abs(nPadding / 2) - TAB_CHAT_HEIGHT;
	nFRDBwidth = g_UserSettings.ClientWidth - (nPadding * 2) - DEFBUT_WIDTH;
	nFRDBheight = nPadding;

	// Mastery checkbox
	g_GlobalSettings.hRamMasteryCheckbox = CreateWindow(
		TEXT("button"), "Flame Ram Mastery",
		WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
		nPadding, nMasteryCheckY, nMasteryCheckWidth, nMasteryCheckHeight,
		g_GlobalSettings.hMainWindow, NULL, g_GlobalSettings.hInstance, NULL);
	SendMessage(g_GlobalSettings.hRamMasteryCheckbox, WM_SETFONT, (WPARAM)g_GlobalSettings.hFont, 0);
	if ( g_UserSettings.RamMastery == 1 )
		SendMessage(g_GlobalSettings.hRamMasteryCheckbox, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);

	// Recharge edit box
	memset(buffer, 0, sizeof(buffer));
	_snprintf(buffer, sizeof(buffer)-1, "%lu",
		(g_UserSettings.RamMastery ? g_UserSettings.RamMastRecharge : g_UserSettings.RamNoMastRecharge));
	g_GlobalSettings.hRamRecharge = CreateWindow(
		TEXT("edit"), NULL,
		WS_CHILDWINDOW | ES_RIGHT | WS_BORDER | ES_AUTOHSCROLL,
		nPadding, nRechargeY, nRechargeWidth, nRechargeHeight,
		g_GlobalSettings.hMainWindow, NULL, g_GlobalSettings.hInstance, NULL);
	hRechargeStatic = CreateWindow(
		TEXT("static"), "Recharge",
		WS_CHILD | SS_EDITCONTROL,
		nRechargeStaticX, nRechargeStaticY, nRechargeStaticWidth, nRechargeHeight,
		g_GlobalSettings.hMainWindow, NULL, g_GlobalSettings.hInstance, NULL);
	SendMessage(g_GlobalSettings.hRamRecharge, EM_SETLIMITTEXT, (WPARAM)MAX_EDIT_LEN, 0);
	SendMessage(g_GlobalSettings.hRamRecharge, WM_SETFONT, (WPARAM)g_GlobalSettings.hFont, 0);
	SendMessage(g_GlobalSettings.hRamRecharge, WM_SETTEXT, 0, (LPARAM)buffer);
	SendMessage(hRechargeStatic, WM_SETFONT, (WPARAM)g_GlobalSettings.hFont, 0);

	// Start button
	g_GlobalSettings.hRamStartButton = CreateWindow(
		TEXT("button"), "Start",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		nFRSBx, g_nSBy, nFRSBwidth, nFRSBheight,
		g_GlobalSettings.hMainWindow, NULL, g_GlobalSettings.hInstance, NULL);
	SendMessage(g_GlobalSettings.hRamStartButton, WM_SETFONT, (WPARAM)g_GlobalSettings.hFont, 0);

	// Duration box
	g_GlobalSettings.hRamDurationBox = CreateWindow(
		TEXT("static"), "Runtime: 0.00s",
		WS_CHILD | WS_VISIBLE | SS_EDITCONTROL,
		nFRDBx, nFRDBy, nFRDBwidth, nFRDBheight,
		g_GlobalSettings.hMainWindow, NULL, g_GlobalSettings.hInstance, NULL);
	SendMessage(g_GlobalSettings.hRamDurationBox, WM_SETFONT, (WPARAM)g_GlobalSettings.hFont, 0);

	return 1;
}
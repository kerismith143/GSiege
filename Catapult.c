#include "includes.h"

HWND hMinStatic, hMaxStatic, hOffsetStatic, hRechargeStatic;

int CatapultShowElements()
{
	if ( g_UserSettings.EnableAdv ) ShowWindow(g_GlobalSettings.hCatMinEdit, g_GlobalSettings.nCmdShow);
	else ShowWindow(g_GlobalSettings.hCatMinEdit, SW_HIDE);
	if ( g_UserSettings.EnableAdv ) ShowWindow(hMinStatic, g_GlobalSettings.nCmdShow);
	else ShowWindow(hMinStatic, SW_HIDE);
	UpdateWindow(g_GlobalSettings.hCatMinEdit);
	UpdateWindow(hMinStatic);

	if ( g_UserSettings.EnableAdv ) ShowWindow(g_GlobalSettings.hCatMaxEdit, g_GlobalSettings.nCmdShow);
	else ShowWindow(g_GlobalSettings.hCatMaxEdit, SW_HIDE);
	if ( g_UserSettings.EnableAdv ) ShowWindow(hMaxStatic, g_GlobalSettings.nCmdShow);
	else ShowWindow(hMaxStatic, SW_HIDE);
	UpdateWindow(g_GlobalSettings.hCatMaxEdit);
	UpdateWindow(hMaxStatic);

	if ( g_UserSettings.EnableAdv ) ShowWindow(g_GlobalSettings.hCatRecharge, g_GlobalSettings.nCmdShow);
	else ShowWindow(g_GlobalSettings.hCatRecharge, SW_HIDE);
	if ( g_UserSettings.EnableAdv ) ShowWindow(hRechargeStatic, g_GlobalSettings.nCmdShow);
	else ShowWindow(hRechargeStatic, SW_HIDE);
	UpdateWindow(g_GlobalSettings.hCatRecharge);
	UpdateWindow(hRechargeStatic);

	ShowWindow(g_GlobalSettings.hCatSliderBar, g_GlobalSettings.nCmdShow);
	UpdateWindow(g_GlobalSettings.hCatSliderBar);

	ShowWindow(g_GlobalSettings.hCatSliderValue, g_GlobalSettings.nCmdShow);
	UpdateWindow(g_GlobalSettings.hCatSliderValue);

	ShowWindow(g_GlobalSettings.hCatStartButton, g_GlobalSettings.nCmdShow);
	UpdateWindow(g_GlobalSettings.hCatStartButton);

	ShowWindow(g_GlobalSettings.hCatDurationBox, g_GlobalSettings.nCmdShow);
	UpdateWindow(g_GlobalSettings.hCatDurationBox);

	InvalidateRect(g_GlobalSettings.hMainWindow, NULL, FALSE);
	return 1;
}

int CatapultHideElements()
{
	ShowWindow(g_GlobalSettings.hCatMinEdit, SW_HIDE);
	ShowWindow(hMinStatic, SW_HIDE);
	ShowWindow(g_GlobalSettings.hCatMaxEdit, SW_HIDE);
	ShowWindow(hMaxStatic, SW_HIDE);
	ShowWindow(g_GlobalSettings.hCatRecharge, SW_HIDE);
	ShowWindow(hRechargeStatic, SW_HIDE);
	ShowWindow(g_GlobalSettings.hCatSliderBar, SW_HIDE);
	ShowWindow(g_GlobalSettings.hCatSliderValue, SW_HIDE);
	ShowWindow(g_GlobalSettings.hCatStartButton, SW_HIDE);
	ShowWindow(g_GlobalSettings.hCatDurationBox, SW_HIDE);
	InvalidateRect(g_GlobalSettings.hMainWindow, NULL, FALSE);
	return 1;
}

void CatapultToggleElements()
{
	int n;

	// Hide
	FlameRamHideElements();
	TrebuchetHideElements();

	// Show
	CatapultShowElements();

	// Set bold font and repaint
	SendMessage(g_GlobalSettings.hSiegeTab[0], WM_SETFONT, (WPARAM)g_GlobalSettings.hFont, 0); // Flame Ram
	SendMessage(g_GlobalSettings.hSiegeTab[1], WM_SETFONT, (WPARAM)g_GlobalSettings.hBoldFont, 0); // Catapult
	SendMessage(g_GlobalSettings.hSiegeTab[2], WM_SETFONT, (WPARAM)g_GlobalSettings.hFont, 0); // Trebuchet
	g_GlobalSettings.SelectedTab = g_GlobalSettings.hSiegeTab[1];
	for ( n = 0; n < MAXTABS; n++ )
		InvalidateRect(g_GlobalSettings.hSiegeTab[n], NULL, TRUE);
}

void CatStartBot()
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

		// Build params struct
		g_tpp.nType = BOT_CATAPULT;

		// Start bot thread and set form elements
		g_GlobalSettings.hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&TimerProc, &g_tpp, 0, &g_GlobalSettings.dwThreadId); 
		if ( g_GlobalSettings.hThread != NULL )
			SetStartStop(SHOWSTOP);
	}
}

void CatapultCheckState(HWND hWnd)
{
	if ( hWnd == g_GlobalSettings.hCatStartButton )
		CatStartBot();
}

int CatapultFormBuilder()
{
	char buffer[32];

	int nPadding; // Padding between elements
	//int nCIBx, nCIBy, nCIBwidth, nCIBheight; // Information static position
	int nRangeY, nRangeWidth, nRangeHeight, nRangeStaticY, nRangeStaticWidth; // Range edit and static
	int nRechargeY, nRechargeWidth, nRechargeHeight, nRechargeStaticX, nRechargeStaticY, nRechargeStaticWidth;
	int nMinX, nSMinX; // Minimum range edit box
	int nMaxX, nSMaxX; // Maximum range edit box
	int nCSCBx, nCSCBy, nCSCBwidth, nCSCBheight; // Scrollbar position
	int nCVRx, nCVRy, nCVRwidth, nCVRheight; // Value static position
	int nCSBx, nCSBwidth, nCSBheight; // Start button position
	int nCDBx, nCDBy, nCDBwidth, nCDBheight; // Duration static position

	nPadding = 20;

	// Range Edits
	nRangeY = DEFBUT_HEIGHT + nPadding;
	nRangeWidth = nPadding * 2;
	nRangeHeight = TAB_CHAT_HEIGHT ? TAB_CHAT_HEIGHT : TAB_STATIC_HEIGHT;
	nRangeStaticY = nRangeY + 2;
	nRangeStaticWidth = 60;

	nMinX = nPadding;
	nSMinX = nMinX + nRangeWidth + 5;

	nMaxX = nSMinX + nRangeStaticWidth + nPadding;
	nSMaxX = nMaxX + nRangeWidth + 5;

	// Scroll Bar position
	nCSCBx = nPadding;
	nCSCBy = (nPadding / 2) + nRangeY + nRangeHeight;
	nCSCBwidth = g_UserSettings.ClientWidth - (nPadding * 7);
	nCSCBheight = (nPadding / 2);

	// Recharge Edit
	nRechargeY = (nPadding / 2) + nCSCBy + nCSCBheight;
	nRechargeWidth = nPadding *2;
	nRechargeHeight = TAB_CHAT_HEIGHT ? TAB_CHAT_HEIGHT : TAB_STATIC_HEIGHT;
	nRechargeStaticX = nPadding + nRechargeWidth + 5;
	nRechargeStaticY = nRechargeY + 2;
	nRechargeStaticWidth = 60;

	// Value Readout position
	nCVRx = nCSCBx + nCSCBwidth + (nPadding / 2);
	nCVRy = nCSCBy;
	nCVRwidth = nPadding * 5;
	nCVRheight = nPadding;

	// Start button position
	nCSBwidth = DEFBUT_WIDTH;
	nCSBheight = DEFBUT_HEIGHT;
	nCSBx = (g_UserSettings.ClientWidth - nPadding) - DEFBUT_WIDTH;
	g_nSBy = (g_UserSettings.ClientHeight - nPadding) - DEFBUT_HEIGHT - TAB_CHAT_HEIGHT;

	// Duration timer position
	nCDBx = nPadding;
	nCDBy = (g_UserSettings.ClientHeight - nPadding) - abs(nPadding / 2) - TAB_CHAT_HEIGHT;
	nCDBwidth = g_UserSettings.ClientWidth - (nPadding * 2) - DEFBUT_WIDTH;
	nCDBheight = nPadding;

	// Minimum range edit box
	memset(buffer, 0, sizeof(buffer));
	_snprintf(buffer, sizeof(buffer)-1, "%lu", g_UserSettings.CatMin);
	g_GlobalSettings.hCatMinEdit = CreateWindow(
		TEXT("edit"), NULL,
		WS_CHILDWINDOW | ES_RIGHT | WS_BORDER | ES_AUTOHSCROLL,
		nMinX, nRangeY, nRangeWidth, nRangeHeight,
		g_GlobalSettings.hMainWindow, NULL, g_GlobalSettings.hInstance, NULL);
	hMinStatic = CreateWindow(
		TEXT("static"), "Minimum",
		WS_CHILD | SS_EDITCONTROL,
		nSMinX, nRangeStaticY, nRangeStaticWidth, nRangeHeight,
		g_GlobalSettings.hMainWindow, NULL, g_GlobalSettings.hInstance, NULL);
	SendMessage(g_GlobalSettings.hCatMinEdit, EM_SETLIMITTEXT, (WPARAM)MAX_EDIT_LEN, 0);
	SendMessage(g_GlobalSettings.hCatMinEdit, WM_SETFONT, (WPARAM)g_GlobalSettings.hFont, 0);
	SendMessage(g_GlobalSettings.hCatMinEdit, WM_SETTEXT, 0, (LPARAM)buffer);
	SendMessage(hMinStatic, WM_SETFONT, (WPARAM)g_GlobalSettings.hFont, 0);

	// Maximum range edit box
	memset(buffer, 0, sizeof(buffer));
	_snprintf(buffer, sizeof(buffer)-1, "%lu", g_UserSettings.CatMax);
	g_GlobalSettings.hCatMaxEdit = CreateWindow(
		TEXT("edit"), NULL,
		WS_CHILDWINDOW | WS_VISIBLE | ES_RIGHT | WS_BORDER | ES_AUTOHSCROLL,
		nMaxX, nRangeY, nRangeWidth, nRangeHeight,
		g_GlobalSettings.hMainWindow, NULL, g_GlobalSettings.hInstance, NULL);
	hMaxStatic = CreateWindow(
		TEXT("static"), "Maximum",
		WS_CHILD | SS_EDITCONTROL,
		nSMaxX, nRangeStaticY, nRangeStaticWidth, nRangeHeight,
		g_GlobalSettings.hMainWindow, NULL, g_GlobalSettings.hInstance, NULL);
	SendMessage(g_GlobalSettings.hCatMaxEdit, EM_SETLIMITTEXT, (WPARAM)MAX_EDIT_LEN, 0);
	SendMessage(g_GlobalSettings.hCatMaxEdit, WM_SETFONT, (WPARAM)g_GlobalSettings.hFont, 0);
	SendMessage(g_GlobalSettings.hCatMaxEdit, WM_SETTEXT, 0, (LPARAM)buffer);
	SendMessage(hMaxStatic, WM_SETFONT, (WPARAM)g_GlobalSettings.hFont, 0);

	// Scroll bar
	g_GlobalSettings.hCatSliderBar = CreateWindow(
		TEXT("scrollbar"), NULL,
		WS_CHILD | WS_VISIBLE | SBS_HORZ,
		nCSCBx, nCSCBy, nCSCBwidth, nCSCBheight,
		g_GlobalSettings.hMainWindow, NULL, g_GlobalSettings.hInstance, NULL);
	SetScrollRange(g_GlobalSettings.hCatSliderBar, SB_CTL, g_UserSettings.CatMin, g_UserSettings.CatMax, FALSE);
	SetScrollPos(g_GlobalSettings.hCatSliderBar, SB_CTL, g_UserSettings.CatRange, FALSE);

	// Value readout
	g_GlobalSettings.hCatSliderValue = CreateWindow(
		TEXT("static"), "Range: 0.00",
		WS_CHILD | WS_VISIBLE | SS_EDITCONTROL,
		nCVRx, nCVRy, nCVRwidth, nCVRheight,
		g_GlobalSettings.hMainWindow, NULL, g_GlobalSettings.hInstance, NULL);
	memset(buffer, 0, sizeof(buffer));
	_snprintf(buffer, sizeof(buffer)-1, "Range: %.2f", (((float)(g_UserSettings.CatRange - g_UserSettings.CatMin) / (float)(g_UserSettings.CatMax - g_UserSettings.CatMin)) * 100) );
	SendMessage(g_GlobalSettings.hCatSliderValue, WM_SETFONT, (WPARAM)g_GlobalSettings.hFont, 0);
	SendMessage(g_GlobalSettings.hCatSliderValue, WM_SETTEXT, 0, (LPARAM)buffer);

	// Recharge edit box
	memset(buffer, 0, sizeof(buffer));
	_snprintf(buffer, sizeof(buffer)-1, "%lu", g_UserSettings.CatRecharge);
	g_GlobalSettings.hCatRecharge = CreateWindow(
		TEXT("edit"), NULL,
		WS_CHILDWINDOW | ES_RIGHT | WS_BORDER | ES_AUTOHSCROLL,
		nPadding, nRechargeY, nRechargeWidth, nRechargeHeight,
		g_GlobalSettings.hMainWindow, NULL, g_GlobalSettings.hInstance, NULL);
	hRechargeStatic = CreateWindow(
		TEXT("static"), "Recharge",
		WS_CHILD | SS_EDITCONTROL,
		nRechargeStaticX, nRechargeStaticY, nRechargeStaticWidth, nRangeHeight,
		g_GlobalSettings.hMainWindow, NULL, g_GlobalSettings.hInstance, NULL);
	SendMessage(g_GlobalSettings.hCatRecharge, EM_SETLIMITTEXT, (WPARAM)MAX_EDIT_LEN, 0);
	SendMessage(g_GlobalSettings.hCatRecharge, WM_SETFONT, (WPARAM)g_GlobalSettings.hFont, 0);
	SendMessage(g_GlobalSettings.hCatRecharge, WM_SETTEXT, 0, (LPARAM)buffer);
	SendMessage(hRechargeStatic, WM_SETFONT, (WPARAM)g_GlobalSettings.hFont, 0);

	// Start button
	g_GlobalSettings.hCatStartButton = CreateWindow(
		TEXT("button"), "Start",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		nCSBx, g_nSBy, nCSBwidth, nCSBheight,
		g_GlobalSettings.hMainWindow, NULL, g_GlobalSettings.hInstance, NULL);
	SendMessage(g_GlobalSettings.hCatStartButton, WM_SETFONT, (WPARAM)g_GlobalSettings.hFont, 0);

	// Duration box
	g_GlobalSettings.hCatDurationBox = CreateWindow(
		TEXT("static"), "Runtime: 0.00s",
		WS_CHILD | WS_VISIBLE | SS_EDITCONTROL,
		nCDBx, nCDBy, nCDBwidth, nCDBheight,
		g_GlobalSettings.hMainWindow, NULL, g_GlobalSettings.hInstance, NULL);
	SendMessage(g_GlobalSettings.hCatDurationBox, WM_SETFONT, (WPARAM)g_GlobalSettings.hFont, 0);

	return 1;
}
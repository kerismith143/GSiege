#include "includes.h"

HWND hMinStatic, hMaxStatic, hOffsetStatic, hRechargeStatic;

int TrebuchetShowElements()
{
	if ( g_UserSettings.EnableAdv ) ShowWindow(g_GlobalSettings.hTrebMinEdit, g_GlobalSettings.nCmdShow);
	else ShowWindow(g_GlobalSettings.hTrebMinEdit, SW_HIDE);
	if ( g_UserSettings.EnableAdv ) ShowWindow(hMinStatic, g_GlobalSettings.nCmdShow);
	else ShowWindow(hMinStatic, SW_HIDE);
	UpdateWindow(g_GlobalSettings.hTrebMinEdit);
	UpdateWindow(hMinStatic);

	if ( g_UserSettings.EnableAdv ) ShowWindow(g_GlobalSettings.hTrebMaxEdit, g_GlobalSettings.nCmdShow);
	else ShowWindow(g_GlobalSettings.hTrebMaxEdit, SW_HIDE);
	if ( g_UserSettings.EnableAdv ) ShowWindow(hMaxStatic, g_GlobalSettings.nCmdShow);
	else ShowWindow(hMaxStatic, SW_HIDE);
	UpdateWindow(g_GlobalSettings.hTrebMaxEdit);
	UpdateWindow(hMaxStatic);

	if ( g_UserSettings.EnableAdv ) ShowWindow(g_GlobalSettings.hTrebRecharge, g_GlobalSettings.nCmdShow);
	else ShowWindow(g_GlobalSettings.hTrebRecharge, SW_HIDE);
	if ( g_UserSettings.EnableAdv ) ShowWindow(hRechargeStatic, g_GlobalSettings.nCmdShow);
	else ShowWindow(hRechargeStatic, SW_HIDE);
	UpdateWindow(g_GlobalSettings.hTrebRecharge);
	UpdateWindow(hRechargeStatic);

	ShowWindow(g_GlobalSettings.hTrebSliderBar, g_GlobalSettings.nCmdShow);
	UpdateWindow(g_GlobalSettings.hTrebSliderBar);

	ShowWindow(g_GlobalSettings.hTrebSliderValue, g_GlobalSettings.nCmdShow);
	UpdateWindow(g_GlobalSettings.hTrebSliderValue);

	ShowWindow(g_GlobalSettings.hTrebStartButton, g_GlobalSettings.nCmdShow);
	UpdateWindow(g_GlobalSettings.hTrebStartButton);

	ShowWindow(g_GlobalSettings.hTrebDurationBox, g_GlobalSettings.nCmdShow);
	UpdateWindow(g_GlobalSettings.hTrebDurationBox);

	InvalidateRect(g_GlobalSettings.hMainWindow, NULL, FALSE);
	return 1;
}

int TrebuchetHideElements()
{
	ShowWindow(g_GlobalSettings.hTrebMinEdit, SW_HIDE);
	ShowWindow(hMinStatic, SW_HIDE);
	ShowWindow(g_GlobalSettings.hTrebMaxEdit, SW_HIDE);
	ShowWindow(hMaxStatic, SW_HIDE);
	ShowWindow(g_GlobalSettings.hTrebRecharge, SW_HIDE);
	ShowWindow(hRechargeStatic, SW_HIDE);
	ShowWindow(g_GlobalSettings.hTrebSliderBar, SW_HIDE);
	ShowWindow(g_GlobalSettings.hTrebSliderValue, SW_HIDE);
	ShowWindow(g_GlobalSettings.hTrebStartButton, SW_HIDE);
	ShowWindow(g_GlobalSettings.hTrebDurationBox, SW_HIDE);
	InvalidateRect(g_GlobalSettings.hMainWindow, NULL, FALSE);
	return 1;
}

void TrebuchetToggleElements()
{
	int n;

	// Hide
	FlameRamHideElements();
	CatapultHideElements();

	// Show
	TrebuchetShowElements();
	
	// Set bold font and repaint
	SendMessage(g_GlobalSettings.hSiegeTab[0], WM_SETFONT, (WPARAM)g_GlobalSettings.hFont, 0); // Flame Ram
	SendMessage(g_GlobalSettings.hSiegeTab[1], WM_SETFONT, (WPARAM)g_GlobalSettings.hFont, 0); // Catapult
	SendMessage(g_GlobalSettings.hSiegeTab[2], WM_SETFONT, (WPARAM)g_GlobalSettings.hBoldFont, 0); // Trebuchet
	g_GlobalSettings.SelectedTab = g_GlobalSettings.hSiegeTab[2];
	for ( n = 0; n < MAXTABS; n++ )
		InvalidateRect(g_GlobalSettings.hSiegeTab[n], NULL, TRUE);
}

void TrebStartBot()
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
		g_tpp.nType = BOT_TREBUCHET;

		// Start bot thread and set form elements
		g_GlobalSettings.hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&TimerProc, &g_tpp, 0, &g_GlobalSettings.dwThreadId); 
		if ( g_GlobalSettings.hThread != NULL )
			SetStartStop(SHOWSTOP);
	}
}

void TrebuchetCheckState(HWND hWnd)
{
	if ( hWnd == g_GlobalSettings.hTrebStartButton )
		TrebStartBot();
}

int TrebuchetFormBuilder()
{
	char buffer[32];

	int nPadding; // Padding between elements
	int nRangeY, nRangeWidth, nRangeHeight, nRangeStaticY, nRangeStaticWidth; // Range edit and static
	int nRechargeY, nRechargeWidth, nRechargeHeight, nRechargeStaticX, nRechargeStaticY, nRechargeStaticWidth;
	int nMinX, nSMinX; // Minimum range edit box
	int nMaxX, nSMaxX; // Maximum range edit box
	int nTSCBx, nTSCBy, nTSCBwidth, nTSCBheight; // Scrollbar position
	int nTVRx, nTVRy, nTVRwidth, nTVRheight; // Value static position
	int nTSBx, nTSBwidth, nTSBheight; // Start button position
	int nTDBx, nTDBy, nTDBwidth, nTDBheight; // Duration static position

	nPadding = 20;

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
	nTSCBx = nPadding;
	nTSCBy = (int)(nPadding / 2) + nRangeY + nRangeHeight;
	nTSCBwidth = g_UserSettings.ClientWidth - (nPadding * 7);
	nTSCBheight = (nPadding / 2);

	// Recharge Edit
	nRechargeY = (nPadding / 2) + nTSCBy + nTSCBheight;
	nRechargeWidth = nPadding *2;
	nRechargeHeight = TAB_CHAT_HEIGHT ? TAB_CHAT_HEIGHT : TAB_STATIC_HEIGHT;
	nRechargeStaticX = nPadding + nRechargeWidth + 5;
	nRechargeStaticY = nRechargeY + 2;
	nRechargeStaticWidth = 60;

	// Value Readout position
	nTVRx = nTSCBx + nTSCBwidth + (nPadding / 2);
	nTVRy = nTSCBy;
	nTVRwidth = nPadding * 5;
	nTVRheight = nPadding;

	// Start button position
	nTSBwidth = DEFBUT_WIDTH;
	nTSBheight = DEFBUT_HEIGHT;
	nTSBx = (g_UserSettings.ClientWidth - nPadding) - DEFBUT_WIDTH;
	g_nSBy = (g_UserSettings.ClientHeight - nPadding) - DEFBUT_HEIGHT - TAB_CHAT_HEIGHT;

	// Duration timer position
	nTDBx = nPadding;
	nTDBy = (g_UserSettings.ClientHeight - nPadding) - abs(nPadding / 2) - TAB_CHAT_HEIGHT;
	nTDBwidth = g_UserSettings.ClientWidth - (nPadding * 2) - DEFBUT_WIDTH;
	nTDBheight = nPadding;

	// Minimum range edit box
	memset(buffer, 0, sizeof(buffer));
	_snprintf(buffer, sizeof(buffer)-1, "%lu", g_UserSettings.TrebMin);
	g_GlobalSettings.hTrebMinEdit = CreateWindow(
		TEXT("edit"), NULL,
		WS_CHILDWINDOW | WS_VISIBLE | ES_RIGHT | WS_BORDER | ES_AUTOHSCROLL,
		nMinX, nRangeY, nRangeWidth, nRangeHeight,
		g_GlobalSettings.hMainWindow, NULL, g_GlobalSettings.hInstance, NULL);
	hMinStatic = CreateWindow(
		TEXT("static"), "Minimum",
		WS_CHILD | SS_EDITCONTROL,
		nSMinX, nRangeStaticY, nRangeStaticWidth, nRangeHeight,
		g_GlobalSettings.hMainWindow, NULL, g_GlobalSettings.hInstance, NULL);
	SendMessage(g_GlobalSettings.hTrebMinEdit, EM_SETLIMITTEXT, (WPARAM)MAX_EDIT_LEN, 0);
	SendMessage(g_GlobalSettings.hTrebMinEdit, WM_SETFONT, (WPARAM)g_GlobalSettings.hFont, 0);
	SendMessage(g_GlobalSettings.hTrebMinEdit, WM_SETTEXT, 0, (LPARAM)buffer);
	SendMessage(hMinStatic, WM_SETFONT, (WPARAM)g_GlobalSettings.hFont, 0);

	// Maximum range edit box
	memset(buffer, 0, sizeof(buffer));
	_snprintf(buffer, sizeof(buffer)-1, "%lu", g_UserSettings.TrebMax);
	g_GlobalSettings.hTrebMaxEdit = CreateWindow(
		TEXT("edit"), NULL,
		WS_CHILDWINDOW | ES_RIGHT | WS_BORDER | ES_AUTOHSCROLL,
		nMaxX, nRangeY, nRangeWidth, nRangeHeight,
		g_GlobalSettings.hMainWindow, NULL, g_GlobalSettings.hInstance, NULL);
	hMaxStatic = CreateWindow(
		TEXT("static"), "Maximum",
		WS_CHILD | SS_EDITCONTROL,
		nSMaxX, nRangeStaticY, nRangeStaticWidth, nRangeHeight,
		g_GlobalSettings.hMainWindow, NULL, g_GlobalSettings.hInstance, NULL);
	SendMessage(g_GlobalSettings.hTrebMaxEdit, EM_SETLIMITTEXT, (WPARAM)MAX_EDIT_LEN, 0);
	SendMessage(g_GlobalSettings.hTrebMaxEdit, WM_SETFONT, (WPARAM)g_GlobalSettings.hFont, 0);
	SendMessage(g_GlobalSettings.hTrebMaxEdit, WM_SETTEXT, 0, (LPARAM)buffer);
	SendMessage(hMaxStatic, WM_SETFONT, (WPARAM)g_GlobalSettings.hFont, 0);

	// Scroll bar
	g_GlobalSettings.hTrebSliderBar = CreateWindow(
		TEXT("scrollbar"), NULL,
		WS_CHILD | WS_VISIBLE | SBS_HORZ,
		nTSCBx, nTSCBy, nTSCBwidth, nTSCBheight,
		g_GlobalSettings.hMainWindow, NULL, g_GlobalSettings.hInstance, NULL);
	SetScrollRange(g_GlobalSettings.hTrebSliderBar, SB_CTL, g_UserSettings.TrebMin, g_UserSettings.TrebMax, FALSE);
	SetScrollPos(g_GlobalSettings.hTrebSliderBar, SB_CTL, g_UserSettings.TrebRange, FALSE);

	// Value readout
	g_GlobalSettings.hTrebSliderValue = CreateWindow(
		TEXT("static"), "Range: 0.00",
		WS_CHILD | WS_VISIBLE | SS_EDITCONTROL,
		nTVRx, nTVRy, nTVRwidth, nTVRheight,
		g_GlobalSettings.hMainWindow, NULL, g_GlobalSettings.hInstance, NULL);
	memset(buffer, 0, sizeof(buffer));
	_snprintf(buffer, sizeof(buffer)-1, "Range: %.2f", (((float)(g_UserSettings.TrebRange - g_UserSettings.TrebMin) / (float)(g_UserSettings.TrebMax - g_UserSettings.TrebMin)) * 100) );
	SendMessage(g_GlobalSettings.hTrebSliderValue, WM_SETFONT, (WPARAM)g_GlobalSettings.hFont, 0);
	SendMessage(g_GlobalSettings.hTrebSliderValue, WM_SETTEXT, 0, (LPARAM)buffer);

	// Recharge edit box
	memset(buffer, 0, sizeof(buffer));
	_snprintf(buffer, sizeof(buffer)-1, "%lu", g_UserSettings.TrebRecharge);
	g_GlobalSettings.hTrebRecharge = CreateWindow(
		TEXT("edit"), NULL,
		WS_CHILDWINDOW | ES_RIGHT | WS_BORDER | ES_AUTOHSCROLL,
		nPadding, nRechargeY, nRechargeWidth, nRechargeHeight,
		g_GlobalSettings.hMainWindow, NULL, g_GlobalSettings.hInstance, NULL);
	hRechargeStatic = CreateWindow(
		TEXT("static"), "Recharge",
		WS_CHILD | SS_EDITCONTROL,
		nRechargeStaticX, nRechargeStaticY, nRechargeStaticWidth, nRangeHeight,
		g_GlobalSettings.hMainWindow, NULL, g_GlobalSettings.hInstance, NULL);
	SendMessage(g_GlobalSettings.hTrebRecharge, EM_SETLIMITTEXT, (WPARAM)MAX_EDIT_LEN, 0);
	SendMessage(g_GlobalSettings.hTrebRecharge, WM_SETFONT, (WPARAM)g_GlobalSettings.hFont, 0);
	SendMessage(g_GlobalSettings.hTrebRecharge, WM_SETTEXT, 0, (LPARAM)buffer);
	SendMessage(hRechargeStatic, WM_SETFONT, (WPARAM)g_GlobalSettings.hFont, 0);

	// Start button
	g_GlobalSettings.hTrebStartButton = CreateWindow(
		TEXT("button"), "Start",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		nTSBx, g_nSBy, nTSBwidth, nTSBheight,
		g_GlobalSettings.hMainWindow, NULL, g_GlobalSettings.hInstance, NULL);
	SendMessage(g_GlobalSettings.hTrebStartButton, WM_SETFONT, (WPARAM)g_GlobalSettings.hFont, 0);

	// Duration box
	g_GlobalSettings.hTrebDurationBox = CreateWindow(
		TEXT("static"), "Runtime: 0.00s",
		WS_CHILD | WS_VISIBLE | SS_EDITCONTROL,
		nTDBx, nTDBy, nTDBwidth, nTDBheight,
		g_GlobalSettings.hMainWindow, NULL, g_GlobalSettings.hInstance, NULL);
	SendMessage(g_GlobalSettings.hTrebDurationBox, WM_SETFONT, (WPARAM)g_GlobalSettings.hFont, 0);

	return 1;
}
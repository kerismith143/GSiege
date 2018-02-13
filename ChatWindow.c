#include "includes.h"

WNDPROC oldChatProc;

LRESULT CALLBACK ChatProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch ( msg )
	{
		case WM_COMMAND:
			switch ( HIWORD(wParam) )
			{
				case EN_MAXTEXT:
					SendMessage(g_GlobalSettings.hChatWindow, EM_SETLIMITTEXT, (WPARAM)TAB_CHAT_MAXTEXT, 0);
					break;
				default:
					break;
			}
		case WM_KEYDOWN:
			switch ( wParam )
			{
				case VK_RETURN:
				{
					SendChat();
					return 0;
				}
				default:
					break;
			}
	}

	return CallWindowProc(oldChatProc, hWnd, msg, wParam, lParam);
}

int SendChat()
{
	HWND hGuildWars;

	hGuildWars = FindWindow(FINDCLASS, NULL);
	if ( hGuildWars != NULL )
	{
		char szText[TAB_CHAT_MAXTEXT+3];
		char buffer[TAB_CHAT_MAXTEXT+1];
		unsigned char len;

		len = (unsigned char)SendMessage(g_GlobalSettings.hChatWindow, WM_GETTEXTLENGTH, 0, 0);
		if ( !len )
			return 0;

		memset(buffer, 0, sizeof(buffer));
		memset(szText, 0, sizeof(szText));
		SendMessage(g_GlobalSettings.hChatWindow, WM_GETTEXT, (WPARAM)len+1, (LPARAM)buffer);

		_snprintf(szText, sizeof(szText)-1, "\n%s\n", buffer);
		if ( g_GlobalSettings.BOT_EXECUTE )
			SendKeysQueAdd(hGuildWars, szText);
		else
			SendKeysEx(hGuildWars, szText, len+2, SLEEPMIN);

		memset(buffer, 0, sizeof(buffer));
		SendMessage(g_GlobalSettings.hChatWindow, WM_SETTEXT, 0, (LPARAM)buffer);
	}
	return 1;
}

void ChatWindowClassBuilder()
{
	int nCWx, nCWy, nCWwidth, nCWheight;
	int nCBx, nCBy, nCBwidth, nCBheight;
	int nPadding, nOffsetX, nOffsetY, nOffsetWidth, nOffsetHeight;
	int nOffsetStaticX, nOffsetStaticY, nOffsetStaticWidth, nOffsetStaticHeight;
	char buffer[32];


	g_nCWpadding = 10;
	nPadding = 20;
	TAB_CHAT_HEIGHT = TAB_STATIC_HEIGHT;

	nCBwidth = TAB_BUT_WIDTH;
	nCBheight = TAB_CHAT_HEIGHT;
	nCBx = (g_UserSettings.ClientWidth - g_nCWpadding) - nCBwidth;
	nCBy = g_nSBy + DEFBUT_HEIGHT + g_nCWpadding;

	nCWx = g_nCWpadding;
	nCWy = nCBy;
	nCWwidth = (g_UserSettings.ClientWidth - nCBwidth) - (g_nCWpadding * 2);
	nCWheight = TAB_CHAT_HEIGHT;

	nOffsetX = nPadding;
	nOffsetY = g_nSBy - (nPadding / 2);
	nOffsetWidth = nPadding * 2;
	nOffsetHeight = TAB_STATIC_HEIGHT;
	nOffsetStaticX = nOffsetX + nOffsetWidth + 5;
	nOffsetStaticY = nOffsetY + 2;
	nOffsetStaticWidth = 80;
	nOffsetStaticHeight = nOffsetHeight;

	if ( !g_UserSettings.EnableChat )
		TAB_CHAT_HEIGHT = 0;

	g_GlobalSettings.hChatWindow = CreateWindow(
		TEXT("edit"), NULL,
		WS_CHILDWINDOW | WS_VISIBLE | ES_LEFT | WS_BORDER | ES_AUTOHSCROLL,
		nCWx, nCWy, nCWwidth, nCWheight,
		g_GlobalSettings.hMainWindow, NULL, g_GlobalSettings.hInstance, NULL);
	SendMessage(g_GlobalSettings.hChatWindow, EM_SETLIMITTEXT, (WPARAM)TAB_CHAT_MAXTEXT, 0);

	g_GlobalSettings.hChatButton = CreateWindow(
		TEXT("button"), TEXT("Send"),
		WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON,
		nCBx, nCBy, nCBwidth, nCBheight,
		g_GlobalSettings.hMainWindow, NULL, g_GlobalSettings.hInstance, NULL);

	SendMessage(g_GlobalSettings.hChatWindow, WM_SETFONT, (WPARAM)g_GlobalSettings.hFont, 0);
	SendMessage(g_GlobalSettings.hChatButton, WM_SETFONT, (WPARAM)g_GlobalSettings.hFont, 0);

	oldChatProc = (WNDPROC)SetWindowLongPtr(g_GlobalSettings.hChatWindow, GWLP_WNDPROC, (LONG_PTR)ChatProc);

	// Offset edit box
	memset(buffer, 0, sizeof(buffer));
	_snprintf(buffer, sizeof(buffer)-1, "%lu", g_UserSettings.Offset);
	g_GlobalSettings.hOffset = CreateWindow(
		TEXT("edit"), NULL,
		WS_CHILDWINDOW | WS_VISIBLE | ES_RIGHT | WS_BORDER | ES_AUTOHSCROLL,
		nOffsetX, nOffsetY, nOffsetWidth, nOffsetHeight,
		g_GlobalSettings.hMainWindow, NULL, g_GlobalSettings.hInstance, NULL);
	g_GlobalSettings.hOffsetStatic = CreateWindow(
		TEXT("static"), "Offset (Xns)",
		WS_CHILD | WS_VISIBLE | SS_EDITCONTROL,
		nOffsetStaticX, nOffsetStaticY, nOffsetStaticWidth, nOffsetStaticHeight,
		g_GlobalSettings.hMainWindow, NULL, g_GlobalSettings.hInstance, NULL);
	SendMessage(g_GlobalSettings.hOffset, EM_SETLIMITTEXT, (WPARAM)g_UserSettings.Offset, 0);
	SendMessage(g_GlobalSettings.hOffset, WM_SETFONT, (WPARAM)g_GlobalSettings.hFont, 0);
	SendMessage(g_GlobalSettings.hOffset, WM_SETTEXT, 0, (LPARAM)buffer);
	SendMessage(g_GlobalSettings.hOffsetStatic, WM_SETFONT, (WPARAM)g_GlobalSettings.hFont, 0);

	SetWindowState();
}
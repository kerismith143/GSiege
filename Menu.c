#include "includes.h"

void SetMenuState()
{
	if ( g_UserSettings.OnTop )
		CheckMenuItem(g_GlobalSettings.hMenu, IDM_OPT_ONTOP, MF_CHECKED);
	else if ( !g_UserSettings.OnTop )
		CheckMenuItem(g_GlobalSettings.hMenu, IDM_OPT_ONTOP, MF_UNCHECKED);

	if ( g_UserSettings.EnableChat )
		CheckMenuItem(g_GlobalSettings.hMenu, IDM_OPT_CHAT, MF_CHECKED);
	else if ( !g_UserSettings.EnableChat )
		CheckMenuItem(g_GlobalSettings.hMenu, IDM_OPT_CHAT, MF_UNCHECKED);

	if ( g_GlobalSettings.AFK_EXECUTE )
		CheckMenuItem(g_GlobalSettings.hMenu, IDM_OPT_AFK, MF_CHECKED);
	else if ( !g_GlobalSettings.AFK_EXECUTE )
		CheckMenuItem(g_GlobalSettings.hMenu, IDM_OPT_AFK, MF_UNCHECKED);

	if ( g_UserSettings.EnableAdv )
		CheckMenuItem(g_GlobalSettings.hMenu, IDM_OPT_ADV, MF_CHECKED);
	else if ( !g_UserSettings.EnableAdv )
		CheckMenuItem(g_GlobalSettings.hMenu, IDM_OPT_ADV, MF_UNCHECKED);
}

int MenuCheckState(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch ( msg )
	{
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDM_OPT_ONTOP:
				{
					if (g_UserSettings.OnTop) // if on
					{
						g_UserSettings.OnTop = 0; // turn it off
						SetWindowState();
						SetMenuState();
					}
					else // if off
					{
						g_UserSettings.OnTop = 1; // turn it on
						SetWindowState();
						SetMenuState();
					}
					break;
				} //case IDM_OPT_ONTOP:
				case IDM_OPT_CHAT:
				{
					if ( g_UserSettings.EnableChat ) // if on
					{
						g_UserSettings.EnableChat = 0; // turn it off
						SetWindowState();
						SetMenuState();
					}
					else // if off
					{
						g_UserSettings.EnableChat = 1; // turn it on
						SetWindowState();
						SetMenuState();
					}
					break;
				} // case IDM_OPT_CHAT:
				case IDM_OPT_AFK:
				{
					if ( g_GlobalSettings.AFK_EXECUTE ) // if on
					{
						g_GlobalSettings.AFK_EXECUTE = 0; // turn it off
						SetMenuState();
					}
					else // if off
					{
						g_GlobalSettings.AFK_EXECUTE = 1; // turn it on
						SetMenuState();
						g_GlobalSettings.hIdleThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&IdleProc, NULL, 0, &g_GlobalSettings.dwIdleThreadId);
					}
					break;
				} // case IDM_OPT_AFK:
				case IDM_OPT_ADV:
				{
					if ( g_UserSettings.EnableAdv ) // if on
					{
						g_UserSettings.EnableAdv = 0; // turn it off
						SetWindowState();
						SetMenuState();
					}
					else // if off
					{
						g_UserSettings.EnableAdv = 1; // turn it on
						SetWindowState();
						SetMenuState();
					}
					break;
				} // case IDM_OPT_ADV:
				case IDM_OPT_RST:
				{
					remove(GSIEGE_DAT);
					memset(&g_UserSettings, 0, sizeof(USERSETTINGS));
					g_UserSettings.CatMax = CATMAXRNG;
					g_UserSettings.TrebMax = TREBMAXRNG;
					g_UserSettings.Offset = RANDOFFSET;
					g_UserSettings.RamNoMastRecharge = RAMNOMASTER;
					g_UserSettings.RamMastRecharge = RAMMASTER;
					g_UserSettings.CatRecharge = CATTREB;
					g_UserSettings.TrebRecharge = CATTREB;
					g_UserSettings.MajorVersion = GSIEGE_MAJOR_VERSION;
					g_UserSettings.MinorVersion = GSIEGE_MINOR_VERSION;
					SaveSettings();
					if (g_GlobalSettings.dwThreadId)
						g_GlobalSettings.BOT_EXECUTE = 0;
					PostQuitMessage(0);
					break;
				} // case IDM_OPT_RST:
				case IDM_OPT_HELP:
				{
					char buffer[2048];

					memset(buffer, 0, sizeof(buffer));
					_snprintf(buffer, sizeof(buffer)-1,
						"%s\nCoded by: pushbyte\n\n" \
						"1. Set whether you have Flame Ram Mastery III or greater.\n" \
						"OR\n" \
						"1. Set range (in seconds) for Catapult or Trebuchet, as desired, using the slider bar.\n" \
						"\n2. Press Start button.\n" \
						"\n\nAdvanced:\n" \
						"-----------------------" \
						"\nOffset is the maximum amount of time to wait randomly between skill usage. " \
						"This feature is suggested for bypassing some simple detections.\n(Default: 5 nanoseconds)\n" \
						"\nMinimum and Maximum values for Catapult and Trebuchet determine the valid range of values to fire within. " \
						"This permits you to tweak the range settings from default.\n(Default: Catapult Max %lu, Trebuchet Max %lu)\n" \
						"\nRecharge is the time to wait inbetween skill usage for each piece of siege.\n" \
						"(Default: Ram Mastery %lu, No Ram Mastery %lu, Catapult/Trebuchet %lu)",
						GSIEGE_VERSION_STRING, CATMAXRNG, TREBMAXRNG, RAMMASTER, RAMNOMASTER, CATTREB);
					MessageBox(NULL, buffer, "Help", MB_OK);
				}
				default:
					return 0;
			} //switch ( LOWORD(wParam) )
		default:
			return 0;
	} //switch ( msg )
	return 0;
}

#include "includes.h"

#define SKPROCESS	   3 // Amount of buffer entries to process for SendKeysQue()

// Procedure to control Flame Ram bot functionality
DWORD BotRam()
{
	HWND hGuildWars;
	uint8_t k, n;
	DWORD dwSleep, dwDuration = 0;

	n = 3; // Set max blast tick count
	while ( g_GlobalSettings.BOT_EXECUTE )
	{
		// Locate and set Guild Wars 2 as active
		hGuildWars = FindWindow(FINDCLASS, NULL);
		if ( hGuildWars != NULL )
		{
			// Use Flame Blast(2) skill every 4th tick
			if ( n >= 3 )
			{
				n = 0; // Reset tick count
				dwDuration += SendKeys(hGuildWars, '2', RandOffset(0, MS_ENABLED));
			}

			// Pause between skills
			dwSleep = RandOffset(g_UserSettings.Offset, MS_ENABLED);
			dwDuration += dwSleep;
			Sleep(dwSleep);

			// Use Ram(1) skill
			dwDuration += SendKeys(hGuildWars, '1', RandOffset(0, MS_ENABLED));

			// Sleep one iteration and increment tick count
			dwSleep = RandOffset((g_UserSettings.RamMastery ? g_UserSettings.RamMastRecharge : g_UserSettings.RamNoMastRecharge), MS_ENABLED);
			dwDuration += dwSleep;
			n++;
			// Send SKPROCESS lines of text from SendQue
			for ( k = 0; k < SKPROCESS; k++ )
			{
				int nRet;
				if ( (nRet = SendKeysQue()) >= 0 )
				{
					dwDuration += nRet;
					dwSleep -= nRet;
					if ( dwSleep > 0x00FFFFFF ) // rolled over check
						dwSleep = 0;
				}
			}
			// Wait for iteration
			Sleep(dwSleep);
		}
	}
	return dwDuration;
}

// Procedure to control Catapult and Trebuchet bot functionality
DWORD BotCT(int nType)
{
	HWND hGuildWars;
	uint8_t k, n = 0;
	DWORD dwSleep, dwRange, dwDuration = 0;

	while ( g_GlobalSettings.BOT_EXECUTE )
	{
		// Locate and set Guild Wars 2 as active
		hGuildWars = FindWindow(FINDCLASS, NULL);
		if ( hGuildWars != NULL )
		{
			// Set range of siege
			if ( nType == BOT_TREBUCHET )
				dwRange = (((DWORD)g_UserSettings.TrebRange) * 10);
			else if ( nType == BOT_CATAPULT )
				dwRange = (((DWORD)g_UserSettings.CatRange) * 10);
			else
			{
				g_GlobalSettings.BOT_EXECUTE = 0;
				return dwDuration;
			}

			// Press Fire Boulder(2)/Fire(2) skill
			dwDuration += SendKeys(hGuildWars, '2', RandOffset(dwRange, MS_ENABLED));

			// Sleep one iteration and increment tick count
			if ( nType == BOT_TREBUCHET )
				dwSleep = RandOffset(g_UserSettings.TrebRecharge, MS_ENABLED);
			else if ( nType == BOT_CATAPULT )
				dwSleep = RandOffset(g_UserSettings.CatRecharge, MS_ENABLED);
			dwDuration += dwSleep;
			n++;

			// Send SKPROCESS lines of text from SendQue
			for ( k = 0; k < SKPROCESS; k++ )
			{
				int nRet;
				if ( (nRet = SendKeysQue()) >= 0 )
				{
					dwDuration += nRet;
					dwSleep -= nRet;
					if ( dwSleep > 0x00FFFFFF ) // rolled over check
						dwSleep = 0;
				}
			}

			// Wait for iteration
			Sleep(dwSleep);
		}
	}
	return dwDuration;
}

// Thread procedure for controlling the AFK
DWORD IdleProc(LPVOID lpParams)
{
	HWND hGuildWars;

	// Stop bot from running
	while ( g_GlobalSettings.AFK_EXECUTE )
	{
		// Locate and set Guild Wars 2 as active
		hGuildWars = FindWindow(FINDCLASS, NULL);
		if ( hGuildWars != NULL )
		{
			uint16_t nCount;
			char *seed = "1ad";

			// Send idle key from seed string to client
			SendKeys(hGuildWars, seed[(rand() % sizeof(seed))], RandOffset(1, MS_ENABLED));
			for ( nCount = 0; nCount < IDLEINTERVAL; nCount++ )
			{
				if ( g_GlobalSettings.AFK_EXECUTE )
					Sleep((rand() % 5) + 1000);
				else
					return 0;
			}

			// Send SKPROCESS lines of text from SendQue
			for ( nCount = 0; nCount < SKPROCESS; nCount++ )
				SendKeysQue();
		}
	}
	return 0;
}

// Thread procedure for controlling the bot
DWORD TimerProc(LPVOID lpParams)
{
	LPTIMERPROCPARAMS ptpp = (LPTIMERPROCPARAMS)lpParams;
	DWORD dwDuration;
	float fDuration;
	char buffer[32];

	memset(buffer, 0, sizeof(buffer));

	// Execute procedure for Flame Ram with Mastery II or less
	if ( ptpp->nType == BOT_RAM )
	{
		dwDuration = BotRam();
		fDuration = (((float)dwDuration) / 1000);
		_snprintf(buffer, sizeof(buffer)-1, "Run Time: %.2fs", fDuration);
		SendMessage(g_GlobalSettings.hRamDurationBox, WM_SETTEXT, 0, (LPARAM)buffer);
		SetStartStop(SHOWSTART);
		return dwDuration;
	}

	// Execute procedure for Catapult and Trebuchet
	else if ( (ptpp->nType == BOT_CATAPULT) || (ptpp->nType == BOT_TREBUCHET) )
	{
		dwDuration = BotCT(ptpp->nType);
		fDuration = (((float)dwDuration) / 1000);
		_snprintf(buffer, sizeof(buffer)-1, "Run Time: %.2fs", fDuration);
		if ( ptpp->nType == BOT_CATAPULT )
			SendMessage(g_GlobalSettings.hCatDurationBox, WM_SETTEXT,0,(LPARAM)buffer);
		else
			SendMessage(g_GlobalSettings.hTrebDurationBox, WM_SETTEXT,0,(LPARAM)buffer);
		SetStartStop(SHOWSTART);
		return dwDuration;
	}
	return 0;
}
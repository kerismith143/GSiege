#include "includes.h"

void ModifyEdits(HWND hWnd)
{
	DWORD dwOffsetLength;
	char *buffer;
	char buffer32[32];

	dwOffsetLength = SendMessage(hWnd, WM_GETTEXTLENGTH, 0, 0);
	buffer = (char *)malloc(dwOffsetLength + 1);
	SendMessage(hWnd, WM_GETTEXT, (WPARAM)(dwOffsetLength + 1), (LPARAM)buffer);

	if ( hWnd == g_GlobalSettings.hOffset )
		g_UserSettings.Offset = atoi(buffer);
	else if ( hWnd == g_GlobalSettings.hCatMinEdit )
		g_UserSettings.CatMin = atoi(buffer);
	else if ( hWnd == g_GlobalSettings.hCatMaxEdit )
		g_UserSettings.CatMax = atoi(buffer);
	else if ( hWnd == g_GlobalSettings.hTrebMinEdit )
		g_UserSettings.TrebMin = atoi(buffer);
	else if ( hWnd == g_GlobalSettings.hTrebMaxEdit )
		g_UserSettings.TrebMax = atoi(buffer);
	else if ( hWnd == g_GlobalSettings.hCatRecharge )
		g_UserSettings.CatRecharge = atoi(buffer);
	else if ( hWnd == g_GlobalSettings.hTrebRecharge )
		g_UserSettings.TrebRecharge = atoi(buffer);
	else if ( hWnd == g_GlobalSettings.hRamRecharge )
	{
		if ( g_UserSettings.RamMastery )
			g_UserSettings.RamMastRecharge = atoi(buffer);
		else
			g_UserSettings.RamNoMastRecharge = atoi(buffer);
	}

	SetScrollRange(g_GlobalSettings.hCatSliderBar, SB_CTL, g_UserSettings.CatMin, g_UserSettings.CatMax, FALSE);
	if ( g_UserSettings.CatRange < g_UserSettings.CatMin )
		g_UserSettings.CatRange = g_UserSettings.CatMin;
	else if ( g_UserSettings.CatRange > g_UserSettings.CatMax )
		g_UserSettings.CatRange = g_UserSettings.CatMax;
	SetScrollPos(g_GlobalSettings.hCatSliderBar, SB_CTL, g_UserSettings.CatRange, TRUE);
	memset(buffer32, 0, sizeof(buffer32));
	_snprintf(buffer32, sizeof(buffer32)-1, "Range: %.2f", (((float)(g_UserSettings.CatRange - g_UserSettings.CatMin) / (float)(g_UserSettings.CatMax - g_UserSettings.CatMin)) * 100) );
	SendMessage(g_GlobalSettings.hCatSliderValue, WM_SETTEXT, 0, (LPARAM)buffer32);

	SetScrollRange(g_GlobalSettings.hTrebSliderBar, SB_CTL, g_UserSettings.TrebMin, g_UserSettings.TrebMax, FALSE);
	if ( g_UserSettings.TrebRange < g_UserSettings.TrebMin )
		g_UserSettings.TrebRange = g_UserSettings.TrebMin;
	else if ( g_UserSettings.TrebRange > g_UserSettings.TrebMax )
		g_UserSettings.TrebRange = g_UserSettings.TrebMax;
	SetScrollPos(g_GlobalSettings.hTrebSliderBar, SB_CTL, g_UserSettings.TrebRange, TRUE);
	memset(buffer32, 0, sizeof(buffer32));
	_snprintf(buffer32, sizeof(buffer32)-1, "Range: %.2f", (((float)(g_UserSettings.TrebRange - g_UserSettings.TrebMin) / (float)(g_UserSettings.TrebMax - g_UserSettings.TrebMin)) * 100) );
	SendMessage(g_GlobalSettings.hTrebSliderValue, WM_SETTEXT, 0, (LPARAM)buffer32);

	free(buffer);
}

// Load position and setting information from disk
void LoadSettings()
{
	FILE *fp;

	// Search for saved settings information
	memset(&g_UserSettings, 0, sizeof(g_UserSettings));
	g_UserSettings.CatMax = CATMAXRNG;
	g_UserSettings.TrebMax = TREBMAXRNG;
	g_UserSettings.Offset = RANDOFFSET;
	g_UserSettings.RamNoMastRecharge = RAMNOMASTER;
	g_UserSettings.RamMastRecharge = RAMMASTER;
	g_UserSettings.CatRecharge = CATTREB;
	g_UserSettings.TrebRecharge = CATTREB;
	g_UserSettings.MajorVersion = GSIEGE_MAJOR_VERSION;
	g_UserSettings.MinorVersion = GSIEGE_MINOR_VERSION;

	if ( (fp = fopen(GSIEGE_DAT, "rb")) == NULL )
		return;
	fread(&g_UserSettings, sizeof(g_UserSettings), 1, fp);
	fclose(fp);
	if ( (g_UserSettings.MajorVersion != GSIEGE_MAJOR_VERSION) &&
		 (g_UserSettings.MinorVersion != GSIEGE_MINOR_VERSION) )
	{
		memset(&g_UserSettings, 0, sizeof(g_UserSettings));
		g_UserSettings.MajorVersion = GSIEGE_MAJOR_VERSION;
		g_UserSettings.MinorVersion = GSIEGE_MINOR_VERSION;
		remove(GSIEGE_DAT);
	}
}

// Save position and setting information to disk
void SaveSettings()
{
	FILE *fp;

	// Save settings information to disk
	if ( (fp = fopen(GSIEGE_DAT, "wb")) != NULL )
	{
		fwrite(&g_UserSettings, sizeof(g_UserSettings), 1, fp);
		fclose(fp);
	}
}

// Generate a randomly incremented number based on the original value
DWORD RandOffset(DWORD dwOrigValue, int nMilliseconds)
{
	DWORD dwNewValue;
	int nMultiplier = 1; // MS_DISABLED

	if ( nMilliseconds ) nMultiplier = 100;
	srand((unsigned int)time(NULL));
	dwNewValue = dwOrigValue;
	if ( g_UserSettings.Offset )
	{
		dwNewValue += (((rand() % g_UserSettings.Offset)) * nMultiplier); // Variance by 5 units
	}
	return dwNewValue;
}
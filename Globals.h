#ifndef __GSIEGE_GLOBALS_H__
#define __GSIEGE_GLOBALS_H__

// Guild Wars 2 main window class name
#define FINDCLASS	"ArenaNet_Dx_Window_Class"
#define GSIEGE_MAJOR_VERSION	1
#define GSIEGE_MINOR_VERSION	5
#define GSIEGE_VERSION_STRING	"GSiege v1.5 (beta)"

#define SLEEPMIN	50

#define RAMMASTER	3750 // Wait time + offset for Flame Ram with Mastery III or higher
#define RAMNOMASTER	5750 // Wait time + offset for Flame Ram with Mastery II or lower
#define CATTREB		5750 // Wait time + offset for Catapult and Trebuchet

// Global Flame Ram handles

// Trebuchet global window handles

// Global Catapult tab handles

int g_nSBy;

#define MAXTABS			3 // Maximum number of tabs to display
#define GSIEGE_DAT		"GSiege.dat" // Name of settings file
#define MAXTEXT			100 // Maximum number of characters for chat with NULL terminator
#define MAXRNG			4
#define RANDOFFSET		5
#define MAINWIDTH		300
#define MAINHEIGHT		210
#define IDLEINTERVAL	180 // seconds between skill uses

typedef struct tagGLOBALSETTINGS
{
	HINSTANCE hInstance;
	HINSTANCE hPrevInstance;
	uint8_t nCmdShow;
	HFONT hFont;
	HFONT hBoldFont;
	HWND hMainWindow;
	HWND hSiegeTab[MAXTABS];
	uint8_t BOT_EXECUTE;
	uint8_t AFK_EXECUTE;
	DWORD dwThreadId;
	HANDLE hThread;
	DWORD dwIdleThreadId;
	HANDLE hIdleThread;
	char szMenuName;
	HMENU hMenu;

	HWND hChatWindow;
	HWND hChatButton;
	HWND hRandomEdit;

	HWND hRamStartButton;
	HWND hRamDurationBox;
	HWND hRamRecharge;
	HWND hRamMasteryCheckbox;

	HWND hTrebStartButton;
	HWND hTrebDurationBox;
	HWND hTrebSliderBar;
	HWND hTrebSliderValue;
	HWND hTrebMinEdit;
	HWND hTrebMaxEdit;
	HWND hTrebRecharge;

	HWND hCatStartButton;
	HWND hCatDurationBox;
	HWND hCatSliderBar;
	HWND hCatSliderValue;
	HWND hCatMinEdit;
	HWND hCatMaxEdit;
	HWND hCatRecharge;

	HWND hOffset;
	HWND hOffsetStatic;

	HWND SelectedTab;
} GLOBALSETTINGS, *PGLOBALSETTINGS, *LPGLOBALSETTINGS;
GLOBALSETTINGS g_GlobalSettings;

#pragma pack(push, 1)
typedef struct tagUSERSETTINGS
{
	uint8_t MajorVersion;
	uint8_t MinorVersion;
	uint32_t MainX;
	uint32_t MainY;
	int MainWidth;
	int MainHeight;
	int32_t ClientWidth;
	int32_t ClientHeight;
	unsigned char RamMastery;
	uint16_t RamMastRecharge;
	uint16_t RamNoMastRecharge;
	int CatRange;
	int CatMin;
	int CatMax;
	uint16_t CatRecharge;
	int TrebRange;
	int TrebMin;
	int TrebMax;
	uint16_t TrebRecharge;
	int Offset;
	uint8_t OnTop;
	uint8_t EnableChat;
	uint8_t EnableAdv;
} USERSETTINGS, *PUSERSETTINGS, *LPUSERSETTINGS;
#pragma pack(pop)
USERSETTINGS g_UserSettings;


// Structure to pass to thread initialisation
typedef struct tagTIMERPROCPARAMS
{
	int		nType; // Type of thread to initialise
} TIMERPROCPARAMS, *PTIMERPROCPARAMS, *LPTIMERPROCPARAMS;
TIMERPROCPARAMS g_tpp; // Global thread initialisation type

DWORD RandOffset(DWORD dwValue, int nUseMilliseconds); // Vary dwValue by 100-500 milliseconds randomly, or 1 to 5 seconds randomly
void SaveSettings();
void LoadSettings();
void ModifyEdits(HWND hWnd);

#endif //__GSIEGE_GLOBALS_H__

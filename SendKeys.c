#include "includes.h"

#define VKSC_UP		0x01 // Virtual Key Scan Code setting for toggling last known down state
#define VKSC_SHIFT	0x01 // Virtual Key Scan Code for SHIFT key

// Build LPARAM parameter for use in PostMessage with WM_KEYDOWN and WM_KEYUP messages
UINT32 CreateLParam(USHORT nRepeatCount, CHAR vkScanCode, BOOL bDownBefore, BOOL bState)
{
	UINT32 nValue = 0;

	nValue = nRepeatCount | (vkScanCode << 16); // Execution count and key scan code from VkKeyScan API
	if ( bDownBefore ) nValue |= (VKSC_UP << 30);; // Last known state of key
	if ( bState ) nValue |= (VKSC_UP << 31); // Last known state of key
	return nValue;
}

// Remove all characters which would require SHIFT to be pressed, and make all uppercase characters into lowercase
char StripShift(char ch)
{
	if ( (ch >= 0x41) && (ch <= 0x5A) ) // Uppercase
		return (ch + 0x20); // Make lowercase
	if ( (ch >= 0x2C) && (ch <= 0x39) ) // Numbers and ,-./
		return ch;
	if ( (ch >= 0x61) && (ch <= 0x7A) ) // Lowercase
		return ch;
	switch ( ch )
	{
		case 0x0A: // VK_RETURN
		case 0x20: // VK_SPACE
		case 0x27: // '
		case 0x3B: // ;
		case 0x3D: // =
		case 0x5B: // [
		case 0x5C: // Backslash
		case 0x5D: // ]
		case 0x60: // `
			return ch;
		default:
			return 0;
	}
	return ch;
}

// Send keystrokes to provided window handle using PostMessage API
DWORD SendKeys(HWND hWnd, char ch, DWORD pause)
{
	short vkChar;
	short vkState;

	vkState = HIBYTE(VkKeyScan(ch));
	if ( vkState |= VKSC_SHIFT )
	{
		if ( (ch = StripShift(ch)) == 0 )
			return 0;
	}
	vkChar = LOBYTE(VkKeyScan(ch)); // Scan code for selected key

	PostMessage(hWnd, WM_KEYDOWN, (WPARAM)vkChar,
		(LPARAM)CreateLParam(0, MapVirtualKey(vkChar, MAPVK_VK_TO_VSC), FALSE, FALSE));
	Sleep(pause); // Required to process keystrokes properly
	PostMessage(hWnd, WM_KEYUP, (WPARAM)vkChar,
		(LPARAM)CreateLParam(1, MapVirtualKey(vkChar, MAPVK_VK_TO_VSC), TRUE, TRUE));

	return pause;
}

// Send strings to provided window handle using PostMessage API
DWORD SendKeysEx(HWND hWnd, char *text, size_t len, DWORD pause)
{
	DWORD dwDuration = 0;
	size_t n;

	for ( n = 0; n < len; n++ )
	{
		dwDuration += SendKeys(hWnd, text[n], pause);
	}

	return dwDuration;
}
#ifndef __GSIEGE_SENDKEYS_H__
#define __GSIEGE_SENDKEYS_H__

// VK_RETURN 0x0D

DWORD SendKeys(HWND hWnd, char ch, DWORD pause);
DWORD SendKeysEx(HWND hWnd, char *text, size_t len, DWORD pause);

#endif //__GSIEGE_SENDKEYS_H__
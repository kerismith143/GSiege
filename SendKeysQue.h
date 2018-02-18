#ifndef __GSIEGE_SENDKEYSQUE_H__TY7H4JFD938KUZ92XCN48H87J__
#define __GSIEGE_SENDKEYSQUE_H__TY7H4JFD938KUZ92XCN48H87J__
#include <windows.h>

#define SKCHARMAX		99
#define SKDELIMMAX		SKCHARMAX  + 2	// Leading and trailing \n
#define SKTOTALMAX		SKDELIMMAX + 1	// NULL termination.

#define SK_SUCCESS		 1
#define SKE_UNKNOWN		 0
#define SKE_MUSTINIT	-1
#define SKE_VALLIMIT	-2
#define SKE_NOTFOUND	-3

struct send_que
{
	HWND hWnd;
	char Text[SKCHARMAX+3]; // Handle leading and trailing 0x0A
	struct send_que *NextEntry;
};

struct send_que *lpSendQueHead;
struct send_que *lpSendQueCurrent;

struct send_que *SKQueAddElement(HWND hWnd, char *text);
int SKQueRemoveElement(HWND hWnd, char *text);
int SKQueRemoveFirstElement();
int SKQueRemoveLastElement();

int SendKeysQueAdd(HWND hWnd, char *text);
DWORD SendKeysQue();
void SKQueInit();

#endif //__GSIEGE_SENDKEYSQUE_H__TY7H4JFD938KUZ92XCN48H87J__

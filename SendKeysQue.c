#include <stdio.h>
#include <windows.h>
#include "Globals.h"
#include "SendKeys.h"
#include "SendKeysQue.h"

// Build initial list elements
struct send_que *SKQueInitialise(HWND hWnd, char *text)
{
	struct send_que *lpSendQuePtr;
	size_t len;

	len = strlen(text);
	if ( (len < 1) || (len > (SKDELIMMAX)) )
		return NULL;

	if ( (lpSendQuePtr = (struct send_que *)malloc(sizeof(struct send_que))) != NULL )
	{
		memset(lpSendQuePtr, 0, sizeof(struct send_que));
		lpSendQuePtr->hWnd = hWnd;
		memcpy(lpSendQuePtr->Text, text, len);
		lpSendQueHead = lpSendQueCurrent = lpSendQuePtr;
	}
	return lpSendQuePtr;
}

// Add a new element to linked list
struct send_que *SKQueAddElement(HWND hWnd, char *text)
{
	struct send_que *lpSendQuePtr = NULL;
	size_t len;

	len = strlen(text);
	if ( (len < 1) || (len > (SKDELIMMAX)) )
		return NULL;

	if ( lpSendQueHead == NULL )
	{
		lpSendQuePtr = SKQueInitialise(hWnd, text);
		return lpSendQuePtr;
	}

	lpSendQuePtr = (struct send_que *)malloc(sizeof(struct send_que));
	if ( lpSendQuePtr == NULL )
		return NULL;

	memset(lpSendQuePtr, 0, sizeof(struct send_que));
	lpSendQuePtr->hWnd = hWnd;
	memcpy(lpSendQuePtr->Text, text, len);
	lpSendQueCurrent->NextEntry = lpSendQuePtr;
	lpSendQueCurrent = lpSendQuePtr;

	return lpSendQuePtr;
}

// Find a specific element in linked list
struct send_que *SKQueFindElement(HWND hWnd, char *text, struct send_que **lpSendQuePrev)
{
	struct send_que *lpSendQuePtr = NULL;
	struct send_que *lpSendQueTemp = NULL;
	unsigned char bEntryFound = 0;

	lpSendQuePtr = lpSendQueHead;
	while ( lpSendQuePtr != NULL )
	{
		if ( (lpSendQuePtr->hWnd == hWnd) || (!strcmp(lpSendQuePtr->Text, text)) )
		{
			bEntryFound++;
			break;
		}
		lpSendQueTemp = lpSendQuePtr;
		lpSendQuePtr = lpSendQuePtr->NextEntry;
	}

	if ( bEntryFound )
	{
		if ( lpSendQuePrev )
			*lpSendQuePrev = lpSendQueTemp;
		return lpSendQuePtr;
	}
	return NULL;
}

// Remove a specific element from linked list
int SKQueRemoveElement(HWND hWnd, char *text)
{
	struct send_que *lpSendQuePrev = NULL;
	struct send_que *lpSendQueDel = NULL;
	size_t len;

	len = strlen(text);
	if ( (len < 1) || (len > (SKCHARMAX + 2)) )
		return SKE_VALLIMIT;

	lpSendQueDel = SKQueFindElement(hWnd, text, &lpSendQuePrev);
	
	if ( lpSendQueDel == NULL )
		return SKE_NOTFOUND;

	if ( lpSendQuePrev != NULL )
		lpSendQuePrev->NextEntry = lpSendQueDel->NextEntry;

	if ( lpSendQueDel == lpSendQueCurrent )
		lpSendQueCurrent = lpSendQuePrev;
	else if ( lpSendQueDel == lpSendQueHead )
		lpSendQueHead = lpSendQueDel->NextEntry;

	free(lpSendQueDel);

	return SK_SUCCESS;
}

// Remove first element from linked list
int SKQueRemoveFirstElement()
{
	struct send_que *lpSendQueDel = NULL;

	lpSendQueDel = lpSendQueHead;

	if ( lpSendQueDel == NULL )
		return SKE_NOTFOUND;

	lpSendQueHead = lpSendQueDel->NextEntry;

	free(lpSendQueDel);

	return SK_SUCCESS;
}

// Remove last element from linked list
int SKQueRemoveLastElement()
{
	struct send_que *lpSendQueDel;
	struct send_que *lpSendQuePrev = NULL;

	lpSendQueDel = lpSendQueHead;

	if ( lpSendQueDel == NULL )
		return SKE_NOTFOUND;

	while ( lpSendQueDel != lpSendQueCurrent )
	{
		lpSendQuePrev = lpSendQueDel;
		lpSendQueDel = lpSendQueDel->NextEntry;
	}

	lpSendQuePrev->NextEntry = lpSendQueDel->NextEntry;
	lpSendQueCurrent = lpSendQuePrev;

	free(lpSendQueDel);

	return SK_SUCCESS;
}

// Set initial list values
void SKQueInit()
{
	lpSendQueHead = NULL;
	lpSendQueCurrent = NULL;
}

// Add a new element to linked list (export)
int SendKeysQueAdd(HWND hWnd, char *text)
{
	if ( (SKQueAddElement(hWnd, text)) == NULL )
		return SKE_UNKNOWN;
	return SK_SUCCESS;
}

// Process elements from linked list LIFO
DWORD SendKeysQue()
{
	HWND hWnd = 0;
	char text[SKTOTALMAX]; // Handle leading and trailing 0x0A
	size_t len;
	int nRet;
	DWORD dwDuration;

	if ( lpSendQueHead == NULL )
		return ((DWORD)SKE_MUSTINIT);

	len = strlen(lpSendQueHead->Text);

	if ( (len < 1) || (len > (SKDELIMMAX)) )
		return ((DWORD)SKE_VALLIMIT);

	memset(text, 0, sizeof(text));
	hWnd = lpSendQueHead->hWnd;
	memcpy(text, lpSendQueHead->Text, len);

	if ( (nRet = SKQueRemoveFirstElement()) != SK_SUCCESS )
		return ((DWORD)nRet);

	dwDuration = SendKeysEx(hWnd, text, len, SLEEPMIN);
	return dwDuration;
}
#include <stdint.h>
#include "SignalQue.h"

// Linked list structure
typedef struct tagSignalQue
{
	uint8_t Signal;
	LPSIGNALQUE Next;
} SIGNALQUE, *PSIGNALQUE, *LPSIGNALQUE;
LPSIGNALQUE lpSignalQueHead; // Linked list head pointer
LPSIGNALQUE lpSignalQueTail; // Linked list tail pointer

// Initialise SignalQue
uint8_t SignalQueInit()
{
	lpSignalQueHead = NULL;
	lpSignalQueTail = NULL;
	return 0;
}

// Add SIGNAL to end of que
uint8_t SignalQueSIG(uint8_t SIG)
{
	return 0;
}

// Inject INTERRUPT to head of que
uint8_t SignalQueINT(uint8_t INT)
{
	return 0;
}

// Remove matching signals from queue
uint8_t SignalQueRemove()
{
	return 0;
}

// Empty the que
uint8_t SignalQueClearAll()
{
	return 0;
}

// Get next que item
uint8_t SignalQueNext()
{
	return 0;
}

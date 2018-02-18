#include <stdint.h>
#include <stdlib.h>
#include "SignalQue.h"

// Linked list structure
typedef struct s_signal_que
{
	uint8_t Signal;
	struct s_signal_que *Next;
} SIGNALQUE, *PSIGNALQUE, *LPSIGNALQUE;
LPSIGNALQUE lpSignalQueHead; // Linked list head pointer

// Initialise SignalQue
void SignalQueInit()
{
	LPSIGNALQUE lpSignalQuePtr;

	lpSignalQuePtr = (LPSIGNALQUE)malloc(sizeof(SIGNALQUE));
	lpSignalQueHead = lpSignalQuePtr;
}

// Add SIGNAL to end of que
void SignalQueSIG(uint8_t SIG)
{
}

// Inject INTERRUPT to head of que
void SignalQueINT(uint8_t INT)
{
	LPSIGNALQUE lpSignalQuePtr;

	lpSignalQuePtr = (LPSIGNALQUE)malloc(sizeof(SIGNALQUE)); // Allocate memory for list item
	lpSignalQuePtr->Next = lpSignalQueHead; // Save current head state as next entry
	lpSignalQuePtr->Signal = INT; // Set list item signal
	lpSignalQueHead = lpSignalQuePtr; // Update signal que state
}

// Remove matching signals from queue
void SignalQueRemove(uint8_t SignalMask)
{
}

// Empty the que
void SignalQueClearAll()
{
	LPSIGNALQUE lpSignalQuePtr;

	while ( lpSignalQueHead->Next != NULL )
	{
		lpSignalQuePtr = lpSignalQueHead->Next; // Save next index address
		free(lpSignalQueHead); // Free head index memory
		lpSignalQueHead = lpSignalQuePtr; // Move head index ptr to next index
	}
}

// Get next que item
uint8_t SignalQueNext()
{
	uint8_t Signal;
	LPSIGNALQUE lpSignalQuePtr;

	Signal = lpSignalQueHead->Signal; // Save signal for return
	lpSignalQuePtr = lpSignalQueHead->Next; // Save next index address
	free(lpSignalQueHead); // Free head index memory
	lpSignalQueHead = lpSignalQuePtr; // Move head index ptr to next index

	return Signal;
}

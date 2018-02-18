#ifndef __GSIEGE_SIGNALQue_H__CMCN439HFXCM934MHFQ92384H9C__
#define __GSIEGE_SIGNALQue_H__CMCN439HFXCM934MHFQ92384H9C__
#include <stdint.h>
/*
#define SIGNAL_START		0x01
#define SIGNAL_STOP			0x02
#define SIGNAL_CHAT			0x04
#define SIGNAL_FIRE			0x08
#define INTERRUPT_EJECT		0x10
#define SIGNAL_AFKSTART		0x20
#define SIGNAL_AFKSTOP		0x40
#define INTERRUPT_EXIT		0x80
*/

uint8_t SignalQueInit(); // Initialise signal Que for processing
uint8_t SignalQueSIG(); // Add SIGNAL to Que
uint8_t SignalQueINT(); // Add INTERRUPT to Que

#endif //__GSIEGE_SIGNALQue_H__CMCN439HFXCM934MHFQ92384H9C__

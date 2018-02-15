#ifndef __GSIEGE_BOT_H__MVC958YVM35CUMC3UT034UT0C234U__
#define __GSIEGE_BOT_H__MVC958YVM35CUMC3UT034UT0C234U__

// Bot definitions for thread initialisation
#define BOT_RAM				0x01 // Set thread for Flame Ram Mastery II or less
//#define	BOT_RAM_MASTER		0x02 // Set thread for Flame Ram Mastery III or higher
#define BOT_CATAPULT		0x04 // Set thread for Catapult
#define BOT_TREBUCHET		0x08 // Set thread for Trebuchet

DWORD TimerProc(LPVOID lpParams); // Thread procedure handler
DWORD IdleProc(LPVOID lpParams); // Thread idle procedure handler

#endif //__GSIEGE_BOT_H__MVC958YVM35CUMC3UT034UT0C234U__

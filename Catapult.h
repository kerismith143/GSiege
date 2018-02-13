#ifndef __GSIEGE_CATAPULT_HEADER__
#define __GSIEGE_CATAPULT_HEADER__

// Catapult precision range levels for Scrollbar
#define CATMAXRNG	380

int CatapultFormBuilder(); // Build Catapult tab
int CatapultShowElements(); // Show Catapult tab elements
int CatapultHideElements(); // Hide Catapult tab elements
void CatapultToggleElements(); // Toggle Catapult tab elements between show and hide
void CatapultCheckState(HWND hWnd); // Handle Catapult WM_COMMAND messages

#endif //__GSIEGE_CATAPULT_HEADER__
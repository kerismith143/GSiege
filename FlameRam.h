#ifndef __GSIEGE_FLAMERAM_HEADER__
#define __GSIEGE_FLAMERAM_HEADER__

int FlameRamFormBuilder(); // Build Flame Ram tab
int FlameRamHideElements(); // Hide Flame Ram tab elements
int FlameRamShowElements(); // Show Flame Ram tab elements
void FlameRamToggleElements(); // Toggle Flame Ram tab elements between show and hide
void FlameRamCheckState(HWND hWnd); // Handle Flame Ram WM_COMMAND messages

#endif //__GSIEGE_FLAMERAM_HEADER__
#ifndef __GSIEGE_TREBUCHET_HEADER__
#define __GSIEGE_TREBUCHET_HEADER__

// Range definitions for Trebuchet
#define TREBMAXRNG	810

int TrebuchetFormBuilder(); // Create and register trebuchet elements
int TrebuchetShowElements(); // Show all trebuchet elements
int TrebuchetHideElements(); // Hide all trebuchet elements
void TrebuchetToggleElements(); // Toggle showing and hiding of tebuchet elements
void TrebuchetCheckState(HWND hWnd); // Process messages from main window procedure

#endif //__GSIEGE_TREBUCHET_HEADER__
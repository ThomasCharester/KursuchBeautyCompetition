#include <windows.h>

#pragma comment (lib, "winmm.lib")

void startPlaying() {
	PlaySound(TEXT("Default_Music_Menu_1.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

}
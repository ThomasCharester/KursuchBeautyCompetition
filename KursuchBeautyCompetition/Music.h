#include <windows.h>
#include <string>
#include <random>

#pragma comment (lib, "winmm.lib")
class MusicPlayer {
private:
	string trackNames[4]{"Default_Music_Menu_1.wav","Default_Music_Menu_2.wav","Default_Music_Menu_3.wav","Default_Music_Menu_4.wav"};
public:
	void startPlaying() {
		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> dist4(0, 4); // distribution in range [1, 6]
		string temp1 = trackNames[dist4(rng)];
		wstring temp2 = wstring(temp1.begin(), temp1.end());
		PlaySound(temp2.c_str(), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

	}
};
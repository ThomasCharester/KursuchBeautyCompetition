#pragma once
#ifndef MUSIC
#define MUSIC

#include <windows.h>
#include <string>
#include <random>

#pragma comment (lib, "winmm.lib")

#define SOUNDCOUNT 21

#ifdef SOUNDCOUNT

using namespace std;

//MusicPlayer::playSound();

enum sounds : int {
	Default,
	Intro,
	Access,
	Denied,
	Granted,
	Error,
	Yes,
	No,
	Warning,
	Button_Press1,
	Button_Press2,
	Button_Failure,
	Button_Restricted,
	Add,
	Exit1,
	Exit2,
	Loading,
	Menu1,
	Menu2,
	Menu3,
	Menu4
};

class MusicPlayer {
private:
	static string trackNames[SOUNDCOUNT];
public:
	void startPlaying() {
		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> dist4(16, SOUNDCOUNT);
		string temp1 = trackNames[dist4(rng)];
		wstring temp2 = wstring(temp1.begin(), temp1.end());
		PlaySound(temp2.c_str(), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

	}
	static void playSound(sounds sound,bool sync = false) {
		string temp1 = trackNames[sound];
		wstring temp2 = wstring(temp1.begin(), temp1.end());
		if(sync) PlaySound(temp2.c_str(), NULL, SND_FILENAME | SND_SYNC);
		else PlaySound(temp2.c_str(), NULL, SND_FILENAME | SND_ASYNC);
	}
	static void playMusic() {
		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> dist4(17, SOUNDCOUNT);
		string temp1 = trackNames[dist4(rng)];
		wstring temp2 = wstring(temp1.begin(), temp1.end());
		PlaySound(temp2.c_str(), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	}
};

string MusicPlayer::trackNames[SOUNDCOUNT] = { "Default.wav","Music_Intro.wav","SND_Access.wav","SND_Denied","SND_Granted.wav","SND_Error.wav","SND_Yes.wav","SND_No","SND_Warning.wav","SND_Button_Press1.wav","SND_Button_Press2.wav","SND_Button_Failure.wav","SND_Button_Restricted.wav","SND_Element_Add.wav","SND_Exit1.wav","SND_Exit2.wav","SND_Loading.wav","Default_Music_Menu_1.wav","Default_Music_Menu_2.wav","Default_Music_Menu_3.wav","Default_Music_Menu_4.wav"};


#endif // SOUNDCOUNT

#endif // !MUSIC
#pragma once

class soundManager
{
public:
	soundManager();
	~soundManager();

	void preLoadSound();
	void playSfx(const int sfxEnum);
	void playNpcSound();
private:
	unsigned int prevNpcSoundID = 0;
};
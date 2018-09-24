#pragma once

namespace gameMetaData
{
	enum sfxName;
}

class soundManager
{
public:
	soundManager();
	~soundManager();

	void preLoadSound();
	void playSfx(const gameMetaData::sfxName sfxEnum);
	void playNpcSound();
private:
	unsigned int prevNpcSoundID = 0;
};
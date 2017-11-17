#ifndef __SoundManager_H__
#define __SoundManager_H__

enum SoundGroups
{
	SoundGroup_Music = 0,
	SoundGroup_Effects = 1, 
};

enum Sounds
{
	Sound_Music = 0,
	Sound_Effect_Jump = 1,
	Sound_Effect_Land = 2,
	Sound_Effect_Walk = 3,
	Sound_Effect_Shoot = 4,
	Sound_Effect_Hit = 5,
	Sound_Effect_Select = 6,
	Sound_Effect_Coin = 7,
	Sound_Effect_Explosion = 8,
	

};

class SoundManager
{
private:
	Mix_Chunk* m_WalkCue[4];
public:
	const int m_audio_rate = 22050;
	const Uint16 m_audio_format = MIX_DEFAULT_FORMAT;
	const int m_audio_channels = 2;
	const int m_audio_buffers = 512;
	SoundManager();
	~SoundManager();
	void initialize();
	int playSound(Sounds soundToPlay);
	void stopSound(int channelused);
	void MuteSound();
	void UnMuteSound();
	void MuteMusic(int channel);
	void UnMuteMusic(int channel);
};

#endif //__SoundManager_H__

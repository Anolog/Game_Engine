#include "pch.h"
#include "SoundManager.h"


SoundManager::SoundManager()
{
	//walk sounds
	{
		m_WalkCue[0] = Mix_LoadWAV("Data/Sounds/Sound_Walk1.wav");
		m_WalkCue[1] = Mix_LoadWAV("Data/Sounds/Sound_Walk2.wav");
		m_WalkCue[2] = Mix_LoadWAV("Data/Sounds/Sound_Walk3.wav");
		m_WalkCue[3] = Mix_LoadWAV("Data/Sounds/Sound_Walk4.wav");
	}
}

SoundManager::~SoundManager()
{

}

void SoundManager::initialize()
{
	//initialize SDL audio
	SDL_Init(SDL_INIT_AUDIO);
	
	//initialize SDL mixer
	if (Mix_OpenAudio(m_audio_rate, m_audio_format, m_audio_channels, m_audio_buffers))
	{
		assert(true);
	}
	//get the number of audio channels availableee
	int numchannels = Mix_GroupCount(-1);

	//Dedicate channel 0 for music and the rest for sfx
	Mix_GroupChannel(0, SoundGroup_Music);
	Mix_GroupChannels(1, numchannels - 1, SoundGroup_Effects);

}

int SoundManager::playSound(Sounds soundToPlay)
{
	//load the soundToPlay

	Mix_Chunk* pSound = nullptr;
	if (soundToPlay == Sound_Music)
	{
		pSound = Mix_LoadWAV("Data/Sounds/Sound_Music2.wav");
	}
	else if (soundToPlay == Sound_Effect_Jump)
	{
		pSound = Mix_LoadWAV("Data/Sounds/Sound_Jump1.wav");
	}
	else if (soundToPlay == Sound_Effect_Land)
	{
		pSound = Mix_LoadWAV("Data/Sounds/Sound_Land.wav");
	}
	else if (soundToPlay == Sound_Effect_Walk)
	{
		int randm = rand() % 4;

		pSound = m_WalkCue[randm];
	}

	else if (soundToPlay == Sound_Effect_Shoot)
	{
		pSound = Mix_LoadWAV("Data/Sounds/Sound_Shoot.wav");
	}
	else if (soundToPlay == Sound_Effect_Hit)
	{
		pSound = Mix_LoadWAV("Data/Sounds/Sound_Hit1.wav");
	}
	else if (soundToPlay == Sound_Effect_Select)
	{
		pSound = Mix_LoadWAV("Data/Sounds/Sound_Select.wav");
	}
	else if (soundToPlay == Sound_Effect_Coin)
	{
		pSound = Mix_LoadWAV("Data/Sounds/Sound_Coin.wav");
	}
	else if (soundToPlay == Sound_Effect_Explosion)
	{
		pSound = Mix_LoadWAV("Data/Sounds/Sound_Exlposion.wav");
	}

	//find a free channel, use the last one used if they're all taken
	int channel = Mix_GroupAvailable(SoundGroup_Effects);
	if (channel == -1)
		channel = Mix_GroupOldest(SoundGroup_Effects);

	//play the sound
	int channelused = Mix_PlayChannel(channel, pSound, 0);
	
	return channelused;
}

void SoundManager::stopSound(int channelused)
{
	//stop the sound in the specific channel
	Mix_HaltChannel(channelused);
}

void SoundManager::MuteSound()
{
	//continue playing the sound in the specific channel
	Mix_Volume(-1, 0);
}
void SoundManager::UnMuteSound()
{
	//continue playing the sound in the specific channel
	Mix_Volume(-1, 155);
}
void SoundManager::MuteMusic(int channel)
{
	//continue playing the sound in the specific channel
	Mix_Volume(channel, 0);
}
void SoundManager::UnMuteMusic(int channel)
{
	//continue playing the sound in the specific channel
	Mix_Volume(channel, 155);
}
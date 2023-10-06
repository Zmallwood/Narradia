//////////////////////////////////////////////////////////////////////
#include "Audio.hpp"
#include "Engine/Assets/AudioBank.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    Audio::Audio()
    /*//////////*/
    {
        if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
            return;
        AudioBank::Get().LoadAudioFiles();
    }

    void
    Audio::Cleanup()
    /*/////////////////*/
    {
        AudioBank::Get().Cleanup();
    }

    void
    Audio::PlaySound(std::string_view soundNameHash, int channel)
    /*//////////////////////////////////////////////////////////////*/
    {
        if (muted)
            return;
        if (Mix_Playing(0) == 0 || channel != 0)
            Mix_PlayChannel(channel, AudioBank::Get().GetSound(Hash(soundNameHash)), 0);
        Mix_Volume(channel, MIX_MAX_VOLUME / 4);
    }

    void
    Audio::PlayMusic(std::string_view musicNameHash)
    /*/////////////////////////////////////////////////*/
    {
        if (Mix_PlayingMusic() == 0)
            Mix_PlayMusic(AudioBank::Get().GetMusic(Hash(musicNameHash)), -1);
    }

    void
    Audio::StopPlaySound()
    /*///////////////////////*/
    {
        Mix_HaltChannel(0);
    }

    void
    Audio::Mute()
    /*//////////////*/
    {
        Mix_PauseMusic();
        Mix_Volume(-1, 0);
        muted = true;
    }

    void
    Audio::Unmute()
    /*////////////////*/
    {
        Mix_ResumeMusic();
        Mix_Volume(-1, MIX_MAX_VOLUME / 4);
        muted = false;
    }
}
//////////////////////////////////////////////////////////////////////
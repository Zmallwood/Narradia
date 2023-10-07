//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Audio : public Singleton<Audio>
    /*/////////////////////////////////*/
    {
      public:
        Audio();
        void Cleanup();
        void PlaySound(std::string_view soundNameHash, int = 0);
        void PlayMusic(std::string_view musicNameHash);
        void StopPlaySound();
        void Mute();
        void Unmute();

      private:
        bool muted = false;
    };
}
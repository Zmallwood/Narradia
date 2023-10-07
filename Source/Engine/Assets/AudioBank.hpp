#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class AudioBank : public Singleton<AudioBank>
    /*/////////////////////////////////////////*/
    {
      public:
        AudioBank();
        void LoadAudioFiles();
        void Cleanup();
        Mix_Chunk *GetSound(int soundNameHash);
        Mix_Music *GetMusic(int musicNameHash);

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
}
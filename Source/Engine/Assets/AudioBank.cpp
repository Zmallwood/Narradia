//////////////////////////////////////////////////////////////////////
#include "AudioBank.hpp"
#include "FileUtilities.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class AudioBank::Pimpl
    /*//////////////////*/
    {
      public:
        const std::string_view relMusicAudioPath = "Resources/Audio/Music/";
        const std::string_view relSoundsAudioPath = "Resources/Audio/Sounds/";
        std::map<int, Mix_Chunk *> soundFiles;
        std::map<int, Mix_Music *> musicFiles;
    };

    AudioBank::AudioBank()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/
    {
    }

    void AudioBank::LoadAudioFiles()
    /*////////////////////////////*/
    {
        auto absAllSoundsAudioPath = std::string(SDL_GetBasePath()) + p->relSoundsAudioPath.data();
        using iterator = std::filesystem::recursive_directory_iterator;
        for (const auto &imageFileEntry : iterator(absAllSoundsAudioPath))
        /****************************************************************/
        {
            auto absFilePath = imageFileEntry.path().string();
            if (FileUtilities::GetFileExtension(absFilePath) != "wav" &&
                FileUtilities::GetFileExtension(absFilePath) != "mp3")
                continue;
            auto sound = Mix_LoadWAV((absFilePath).c_str());
            if (sound == nullptr)
            /*******************/
            {
                std::cout << "Error loading sound file." << std::endl;
                return;
            }
            p->soundFiles.insert({FileUtilities::GetFileNameHash(absFilePath), sound});
        }
        auto absAllMusicAudioPath = std::string(SDL_GetBasePath()) + p->relMusicAudioPath.data();
        using iterator = std::filesystem::recursive_directory_iterator;
        for (const auto &imageFileEntry : iterator(absAllMusicAudioPath))
        /***************************************************************/
        {
            auto absFilePath = imageFileEntry.path().string();
            if (FileUtilities::GetFileExtension(absFilePath) != "wav" &&
                FileUtilities::GetFileExtension(absFilePath) != "mp3")
                continue;
            auto music = Mix_LoadMUS((absFilePath).c_str());
            if (music == nullptr)
            /*******************/
            {
                std::cout << "Error loading music file." << std::endl;
                return;
            }
            p->musicFiles.insert({FileUtilities::GetFileNameHash(absFilePath), music});
        }
    }

    void AudioBank::Cleanup()
    /*/////////////////////*/
    {
        for (auto entry : p->soundFiles)
            Mix_FreeChunk(entry.second);
        for (auto entry : p->musicFiles)
            Mix_FreeMusic(entry.second);
        Mix_CloseAudio();
    }

    Mix_Chunk *AudioBank::GetSound(int soundNameHash)
    /*/////////////////////////////////////////////*/
    {
        return p->soundFiles.at(soundNameHash);
    }

    Mix_Music *AudioBank::GetMusic(int musicNameHash)
    /*/////////////////////////////////////////////*/
    {
        return p->musicFiles.at(musicNameHash);
    }
}
//////////////////////////////////////////////////////////////////////
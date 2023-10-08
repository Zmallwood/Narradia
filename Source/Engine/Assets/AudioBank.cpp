#include "AudioBank.hpp"
#include "FileUtilities.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class AudioBank::Pimpl {
      public:
        const std::string_view kRelMusicAudioPath = "Resources/Audio/Music/";
        const std::string_view kRelSoundsAudioPath = "Resources/Audio/Sounds/";
        std::map<int, Mix_Chunk *> sound_files_;
        std::map<int, Mix_Music *> music_files_;
    };

    AudioBank::AudioBank()
        : p(std::make_shared<Pimpl>()) {
    }

    void AudioBank::LoadAudioFiles() {
        auto abs_all_sounds_audio_path =
            std::string(SDL_GetBasePath()) + p->kRelSoundsAudioPath.data();
        using iterator = std::filesystem::recursive_directory_iterator;
        for (const auto &audio_file_entry : iterator(abs_all_sounds_audio_path)) {
            auto abs_file_path = audio_file_entry.path().string();
            if (FileUtilities::GetFileExtension(abs_file_path) != "wav" &&
                FileUtilities::GetFileExtension(abs_file_path) != "mp3")
                continue;
            auto sound = Mix_LoadWAV((abs_file_path).c_str());
            if (sound == nullptr) {
                std::cout << "Error loading sound file." << std::endl;
                return;
            }
            p->sound_files_.insert({FileUtilities::GetFileNameHash(abs_file_path), sound});
        }
        auto abs_all_music_audio_path =
            std::string(SDL_GetBasePath()) + p->kRelMusicAudioPath.data();
        using iterator = std::filesystem::recursive_directory_iterator;
        for (const auto &audio_file_entry : iterator(abs_all_music_audio_path)) {
            auto abs_file_path = audio_file_entry.path().string();
            if (FileUtilities::GetFileExtension(abs_file_path) != "wav" &&
                FileUtilities::GetFileExtension(abs_file_path) != "mp3")
                continue;
            auto music = Mix_LoadMUS((abs_file_path).c_str());
            if (music == nullptr) {
                std::cout << "Error loading music file." << std::endl;
                return;
            }
            p->music_files_.insert({FileUtilities::GetFileNameHash(abs_file_path), music});
        }
    }

    void AudioBank::Cleanup() {
        for (auto entry : p->sound_files_)
            Mix_FreeChunk(entry.second);
        for (auto entry : p->music_files_)
            Mix_FreeMusic(entry.second);
        Mix_CloseAudio();
    }

    Mix_Chunk *AudioBank::GetSound(int soundNameHash) {
        return p->sound_files_.at(soundNameHash);
    }

    Mix_Music *AudioBank::GetMusic(int musicNameHash) {
        return p->music_files_.at(musicNameHash);
    }
}
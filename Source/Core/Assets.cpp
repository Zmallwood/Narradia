//////////////////////////////////////////////////////////////////////
#include "Assets.hpp"
#include "ModelCreation.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class ImageBank::Pimpl
    /*//////////////////*/ {
      public:
        GLuint LoadSingleImage(const std::string_view &absFilePath) const;

        const std::string_view relImagesPath = "Resources/Images/";
        std::map<const int, ImageEntry> images;
    }; // Class

    std::map<const int, ImageEntry> &ImageBank::GetImages()
    /*///////////////////////////////////////////////////*/ { return p->images; } // Function

    ImageBank::ImageBank()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/ {} // Class

    GLuint ImageBank::GetImage(int imageNameHash) const
    /*///////////////////////////////////////////////*/ {
        if (p->images.count(imageNameHash)) return p->images.at(imageNameHash).textureId;
        return 0;
    } // Function

    GLuint ImageBank::GetImage(const std::string_view &imageName) const
    /*///////////////////////////////////////////////////////////////*/ {
        return GetImage(Hash(imageName));
    } // Function

    void ImageBank::GetBlankTextImage(const std::string_view &uniqueNameId)
    /*///////////////////////////////////////////////////////////////////*/ {
        ImageEntry imageEntry;
        glGenTextures(1, &imageEntry.textureId);
        p->images.insert({Hash(uniqueNameId), imageEntry});
    } // Function

    void ImageBank::LoadImages()
    /*////////////////////////*/ {
        using iterator = std::filesystem::recursive_directory_iterator;
        auto absAllImagesPath = std::string(SDL_GetBasePath()) + p->relImagesPath.data();
        for (const auto &imageFileEntry : iterator(absAllImagesPath))
        /***********************************************************/ {
            auto absFilePath = imageFileEntry.path().string();
            if (FileUtilities::GetFileExtension(absFilePath) != "png") continue;
            ImageEntry imageEntry;
            imageEntry.textureId = p->LoadSingleImage(absFilePath);
            imageEntry.fileName = FileUtilities::GetFileNameNoExt(absFilePath);
            p->images.insert({FileUtilities::GetFileNameHash(absFilePath), imageEntry});
        }
    } // Function

    void ImageBank::Cleanup() const
    /*///////////////////////////*/ {
        for (const auto &image : p->images)
            glDeleteTextures(1, &image.second.textureId);
    } // Function

    GLuint ImageBank::Pimpl::LoadSingleImage(const std::string_view &absFilePath) const
    /*///////////////////////////////////////////////////////////////////////////////*/ {
        auto surface = IMG_Load(absFilePath.data());
        glEnable(GL_TEXTURE_2D);
        GLuint textureId;
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        if (surface->format->BytesPerPixel == 4)
            glTexImage2D(
                GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                surface->pixels);
        else
            glTexImage2D(
                GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE,
                surface->pixels);
        SDL_FreeSurface(surface);
        return textureId;
    } // Function

    class ModelBank::Pimpl
    /*//////////////////*/ {
      public:
        std::shared_ptr<Model> LoadSingleModel(const std::string_view &pathStr);

        const std::string_view relModelsPath = "Resources/Models/";
        std::map<int, std::shared_ptr<const Model>> models;
    }; // Class

    ModelBank::ModelBank()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/ {} // Function

    auto ModelBank::GetModel(int modelNameHash) const -> const Model *
    /*//////////////////////////////////////////////////////////////*/ {
        return p->models.at(modelNameHash).get();
    } // Function

    auto ModelBank::GetAllModels() const -> std::map<int, std::shared_ptr<const Model>>
    /*///////////////////////////////////////////////////////////////////////////////*/ {
        return p->models;
    } // Function

    void ModelBank::LoadModels()
    /*////////////////////////*/ {
        using iterator = std ::filesystem::recursive_directory_iterator;
        auto absAllModelsPath = std::string(SDL_GetBasePath()) + p->relModelsPath.data();
        for (const auto &filePath : iterator(absAllModelsPath))
        /*****************************************************/ {
            auto pathStr = filePath.path().string();
            auto extensionPos = pathStr.find_last_of('.') + 1;
            auto fileExtension = pathStr.substr(extensionPos);
            if (fileExtension != "dae") continue;
            auto loadedModel = p->LoadSingleModel(pathStr);
            auto imageNameStart = pathStr.find_last_of('/') + 1;
            auto imageNameWithExt = pathStr.substr(imageNameStart);
            auto imageNameExtPos = imageNameWithExt.find_last_of('.');
            auto fileNameNoExt = imageNameWithExt.substr(0, imageNameExtPos);
            auto imageNameHash = Hash(fileNameNoExt);
            p->models.insert({imageNameHash, loadedModel});
        }
    } // Function

    std::shared_ptr<Model> ModelBank::Pimpl::LoadSingleModel(const std::string_view &pathStr)
    /*/////////////////////////////////////////////////////////////////////////////////////*/ {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(pathStr.data(), 0);
        return ModelCreator::Get().CreateModel(scene);
    } // Function

    class AudioBank::Pimpl
    /*//////////////////*/ {
      public:
        const std::string_view relMusicAudioPath = "Resources/Audio/Music/";
        const std::string_view relSoundsAudioPath = "Resources/Audio/Sounds/";
        std::map<int, Mix_Chunk *> soundFiles;
        std::map<int, Mix_Music *> musicFiles;
    }; // Class

    AudioBank::AudioBank()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/ {} // Function

    void AudioBank::LoadAudioFiles()
    /*////////////////////////////*/ {
        auto absAllSoundsAudioPath = std::string(SDL_GetBasePath()) + p->relSoundsAudioPath.data();
        using iterator = std::filesystem::recursive_directory_iterator;
        for (const auto &imageFileEntry : iterator(absAllSoundsAudioPath))
        /****************************************************************/ {
            auto absFilePath = imageFileEntry.path().string();
            if (FileUtilities::GetFileExtension(absFilePath) != "wav" &&
                FileUtilities::GetFileExtension(absFilePath) != "mp3")
                continue;
            auto sound = Mix_LoadWAV((absFilePath).c_str());
            if (sound == nullptr)
            /*******************/ {
                std::cout << "Error loading sound file." << std::endl;
                return;
            }
            p->soundFiles.insert({FileUtilities::GetFileNameHash(absFilePath), sound});
        }
        auto absAllMusicAudioPath = std::string(SDL_GetBasePath()) + p->relMusicAudioPath.data();
        using iterator = std::filesystem::recursive_directory_iterator;
        for (const auto &imageFileEntry : iterator(absAllMusicAudioPath))
        /***************************************************************/ {
            auto absFilePath = imageFileEntry.path().string();
            if (FileUtilities::GetFileExtension(absFilePath) != "wav" &&
                FileUtilities::GetFileExtension(absFilePath) != "mp3")
                continue;
            auto music = Mix_LoadMUS((absFilePath).c_str());
            if (music == nullptr)
            /*******************/ {
                std::cout << "Error loading music file." << std::endl;
                return;
            }
            p->musicFiles.insert({FileUtilities::GetFileNameHash(absFilePath), music});
        }
    } // Function

    void AudioBank::Cleanup()
    /*/////////////////////*/ {
        for (auto entry : p->soundFiles)
            Mix_FreeChunk(entry.second);
        for (auto entry : p->musicFiles)
            Mix_FreeMusic(entry.second);
        Mix_CloseAudio();
    } // Function

    Mix_Chunk *AudioBank::GetSound(int soundNameHash)
    /*/////////////////////////////////////////////*/ {
        return p->soundFiles.at(soundNameHash);
    } // Function

    Mix_Music *AudioBank::GetMusic(int musicNameHash)
    /*/////////////////////////////////////////////*/ {
        return p->musicFiles.at(musicNameHash);
    } // Function

    const std::string_view FileUtilities::GetFileExtension(const std::string_view &absFilePath)
    /*///////////////////////////////////////////////////////////////////////////////////////*/ {
        auto extPos = absFilePath.find_last_of('.') + 1;
        auto fileExt = absFilePath.substr(extPos);
        return fileExt;
    } // Function

    const std::string_view FileUtilities::GetFileNameNoExt(const std::string_view &absFilePath)
    /*///////////////////////////////////////////////////////////////////////////////////////*/ {
        auto imgNameStart = absFilePath.find_last_of('/') + 1;
        auto imgNameWithExt = absFilePath.substr(imgNameStart);
        auto imgNameExtPos = imgNameWithExt.find_last_of('.');
        auto fileNameNoExt = imgNameWithExt.substr(0, imgNameExtPos);
        return fileNameNoExt;
    } // Function

    const int FileUtilities::GetFileNameHash(const std::string_view &absFilePath)
    /*/////////////////////////////////////////////////////////////////////////*/ {
        auto fileNameNoExt = GetFileNameNoExt(absFilePath);
        auto imgNameHash = Hash(fileNameNoExt);
        return imgNameHash;
    } // Function
}
//////////////////////////////////////////////////////////////////////
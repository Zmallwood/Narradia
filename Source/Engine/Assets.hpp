//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    struct ImageEntry
    /*/////////////*/
    {
        std::string fileName;
        GLuint textureId;
    };

    class ImageBank : public Singleton<ImageBank>
    /*/////////////////////////////////////////*/
    {
      public:
        ImageBank();
        GLuint GetImage(int) const;
        GLuint GetImage(const std::string_view &) const;
        void GetBlankTextImage(const std::string_view &);
        void LoadImages();
        void Cleanup() const;
        std::map<const int, ImageEntry> &GetImages();

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };

    class ModelBank : public Singleton<ModelBank>
    /*/////////////////////////////////////////*/
    {
      public:
        ModelBank();
        auto GetModel(int) const -> const Model *;
        auto GetAllModels() const -> std::map<int, std::shared_ptr<const Model>>;
        void LoadModels();

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };

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

    namespace FileUtilities
    /*///////////////////*/
    {
        const std::string_view GetFileExtension(const std::string_view &absFilePath);
        const std::string_view GetFileNameNoExt(const std::string_view &absFilePath);
        const int GetFileNameHash(const std::string_view &absFilePath);
    }
}
//////////////////////////////////////////////////////////////////////
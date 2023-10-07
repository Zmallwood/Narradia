//////////////////////////////////////////////////////////////////////
#include "ImageBank.hpp"
#include "FileUtilities.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class ImageBank::Pimpl
    /*//////////////////*/
    {
      public:
        GLuint LoadSingleImage(const std::string_view &absFilePath) const;

        const std::string_view relImagesPath = "Resources/Images/";
        std::map<const int, ImageEntry> images;
    };

    std::map<const int, ImageEntry> &
    ImageBank::GetImages()
    /*/////////////////////////////*/
    {
        return p->images;
    }

    ImageBank::ImageBank()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/
    {
    }

    GLuint
    ImageBank::GetImage(int imageNameHash) const
    /*////////////////////////////////////////*/
    {
        if (p->images.count(imageNameHash))
            return p->images.at(imageNameHash).textureId;
        return 0;
    }

    GLuint
    ImageBank::GetImage(const std::string_view &imageName) const
    /*////////////////////////////////////////////////////////*/
    {
        return GetImage(Hash(imageName));
    }

    void
    ImageBank::GetBlankTextImage(const std::string_view &uniqueNameId)
    /*//////////////////////////////////////////////////////////////*/
    {
        ImageEntry imageEntry;
        glGenTextures(1, &imageEntry.textureId);
        p->images.insert({Hash(uniqueNameId), imageEntry});
    }

    void
    ImageBank::LoadImages()
    /*///////////////////*/
    {
        using iterator = std::filesystem::recursive_directory_iterator;
        auto absAllImagesPath = std::string(SDL_GetBasePath()) + p->relImagesPath.data();
        for (const auto &imageFileEntry : iterator(absAllImagesPath))
        /***********************************************************/
        {
            auto absFilePath = imageFileEntry.path().string();
            if (FileUtilities::GetFileExtension(absFilePath) != "png")
                continue;
            ImageEntry imageEntry;
            imageEntry.textureId = p->LoadSingleImage(absFilePath);
            imageEntry.fileName = FileUtilities::GetFileNameNoExt(absFilePath);
            p->images.insert({FileUtilities::GetFileNameHash(absFilePath), imageEntry});
        }
    }

    void
    ImageBank::Cleanup() const
    /*//////////////////////*/
    {
        for (const auto &image : p->images)
            glDeleteTextures(1, &image.second.textureId);
    }

    GLuint
    ImageBank::Pimpl::LoadSingleImage(const std::string_view &absFilePath) const
    /*////////////////////////////////////////////////////////////////////////*/
    {
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
    }
}
//////////////////////////////////////////////////////////////////////
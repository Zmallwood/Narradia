#include "ImageBank.hpp"
#include "FileUtilities.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class ImageBank::Pimpl {
      public:
        GLuint LoadSingleImage(const std::string_view &absFilePath) const;

        const std::string_view kRelImagesPath = "Resources/Images/";
        std::map<const int, ImageEntry> images_;
    };

    std::map<const int, ImageEntry> &ImageBank::GetImages() {
        return p->images_;
    }

    ImageBank::ImageBank()
        : p(std::make_shared<Pimpl>()) {
    }

    GLuint ImageBank::GetImage(int imageNameHash) const {
        if (p->images_.count(imageNameHash))
            return p->images_.at(imageNameHash).textureId;
        return 0;
    }

    GLuint ImageBank::GetImage(const std::string_view &imageName) const {
        return GetImage(Hash(imageName));
    }

    void ImageBank::GetBlankTextImage(const std::string_view &uniqueNameId) {
        ImageEntry image_entry;
        glGenTextures(1, &image_entry.textureId);
        p->images_.insert({Hash(uniqueNameId), image_entry});
    }

    void ImageBank::LoadImages() {
        using iterator = std::filesystem::recursive_directory_iterator;
        auto abs_all_images_path = std::string(SDL_GetBasePath()) + p->kRelImagesPath.data();
        for (const auto &image_file_entry : iterator(abs_all_images_path)) {
            auto abs_file_path = image_file_entry.path().string();
            if (FileUtilities::GetFileExtension(abs_file_path) != "png")
                continue;
            ImageEntry image_entry;
            image_entry.textureId = p->LoadSingleImage(abs_file_path);
            image_entry.fileName = FileUtilities::GetFileNameNoExt(abs_file_path);
            p->images_.insert({FileUtilities::GetFileNameHash(abs_file_path), image_entry});
        }
    }

    void ImageBank::Cleanup() const {
        for (const auto &image : p->images_)
            glDeleteTextures(1, &image.second.textureId);
    }

    GLuint ImageBank::Pimpl::LoadSingleImage(const std::string_view &absFilePath) const {
        auto surface = IMG_Load(absFilePath.data());
        glEnable(GL_TEXTURE_2D);
        GLuint texture_id;
        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);
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
        return texture_id;
    }
}
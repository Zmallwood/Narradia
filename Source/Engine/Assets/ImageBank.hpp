//////////////////////////////////////////////////////////////////////
#pragma once
#include "ImageEntry.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
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
}
//////////////////////////////////////////////////////////////////////
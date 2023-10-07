#include "FileUtilities.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    const std::string_view
    FileUtilities::GetFileExtension(const std::string_view &absFilePath)
    /*////////////////////////////////////////////////////////////////*/
    {
        auto extPos = absFilePath.find_last_of('.') + 1;
        auto fileExt = absFilePath.substr(extPos);
        return fileExt;
    }

    const std::string_view
    FileUtilities::GetFileNameNoExt(const std::string_view &absFilePath)
    /*////////////////////////////////////////////////////////////////*/
    {
        auto imgNameStart = absFilePath.find_last_of('/') + 1;
        auto imgNameWithExt = absFilePath.substr(imgNameStart);
        auto imgNameExtPos = imgNameWithExt.find_last_of('.');
        auto fileNameNoExt = imgNameWithExt.substr(0, imgNameExtPos);
        return fileNameNoExt;
    }

    const int
    FileUtilities::GetFileNameHash(const std::string_view &absFilePath)
    /*///////////////////////////////////////////////////////////////*/
    {
        auto fileNameNoExt = GetFileNameNoExt(absFilePath);
        auto imgNameHash = Hash(fileNameNoExt);
        return imgNameHash;
    }
}
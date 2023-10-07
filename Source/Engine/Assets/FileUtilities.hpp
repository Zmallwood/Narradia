//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    namespace FileUtilities
    /*///////////////////*/
    {
        const std::string_view GetFileExtension(const std::string_view &absFilePath);
        const std::string_view GetFileNameNoExt(const std::string_view &absFilePath);
        const int GetFileNameHash(const std::string_view &absFilePath);
    }
}
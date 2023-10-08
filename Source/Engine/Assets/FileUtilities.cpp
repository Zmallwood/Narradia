#include "FileUtilities.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    const std::string_view FileUtilities::GetFileExtension(const std::string_view &absFilePath) {
        auto ext_pos = absFilePath.find_last_of('.') + 1;
        auto file_ext = absFilePath.substr(ext_pos);
        return file_ext;
    }

    const std::string_view FileUtilities::GetFileNameNoExt(const std::string_view &absFilePath) {
        auto img_name_start = absFilePath.find_last_of('/') + 1;
        auto image_name_with_ext = absFilePath.substr(img_name_start);
        auto img_name_ext_pos = image_name_with_ext.find_last_of('.');
        auto file_name_no_ext = image_name_with_ext.substr(0, img_name_ext_pos);
        return file_name_no_ext;
    }

    const int FileUtilities::GetFileNameHash(const std::string_view &absFilePath) {
        auto file_name_no_ext = GetFileNameNoExt(absFilePath);
        auto img_name_hash = Hash(file_name_no_ext);
        return img_name_hash;
    }
}
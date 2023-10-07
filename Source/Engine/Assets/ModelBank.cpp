//////////////////////////////////////////////////////////////////////
#include "ModelBank.hpp"
#include "ModelCreation/ModelCreator.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class ModelBank::Pimpl
    /*//////////////////*/
    {
      public:
        std::shared_ptr<Model> LoadSingleModel(const std::string_view &pathStr);

        const std::string_view relModelsPath = "Resources/Models/";
        std::map<int, std::shared_ptr<const Model>> models;
    };

    ModelBank::ModelBank()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/
    {
    }

    auto
    ModelBank::GetModel(int modelNameHash) const -> const Model *
    /*/////////////////////////////////////////////////////////*/
    {
        return p->models.at(modelNameHash).get();
    }

    auto
    ModelBank::GetAllModels() const -> std::map<int, std::shared_ptr<const Model>>
    /*//////////////////////////////////////////////////////////////////////////*/
    {
        return p->models;
    }

    void
    ModelBank::LoadModels()
    /*///////////////////*/
    {
        using iterator = std ::filesystem::recursive_directory_iterator;
        auto absAllModelsPath = std::string(SDL_GetBasePath()) + p->relModelsPath.data();
        for (const auto &filePath : iterator(absAllModelsPath))
        /*****************************************************/
        {
            auto pathStr = filePath.path().string();
            auto extensionPos = pathStr.find_last_of('.') + 1;
            auto fileExtension = pathStr.substr(extensionPos);
            if (fileExtension != "dae")
                continue;
            auto loadedModel = p->LoadSingleModel(pathStr);
            auto imageNameStart = pathStr.find_last_of('/') + 1;
            auto imageNameWithExt = pathStr.substr(imageNameStart);
            auto imageNameExtPos = imageNameWithExt.find_last_of('.');
            auto fileNameNoExt = imageNameWithExt.substr(0, imageNameExtPos);
            auto imageNameHash = Hash(fileNameNoExt);
            p->models.insert({imageNameHash, loadedModel});
        }
    }

    std::shared_ptr<Model>
    ModelBank::Pimpl::LoadSingleModel(const std::string_view &pathStr)
    /*//////////////////////////////////////////////////////////////*/
    {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(pathStr.data(), 0);
        return ModelCreator::Get()->CreateModel(scene);
    }
}
//////////////////////////////////////////////////////////////////////
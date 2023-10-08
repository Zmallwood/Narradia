#include "ModelBank.hpp"
#include "ModelCreation/ModelCreator.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class ModelBank::Pimpl {
      public:
        std::shared_ptr<Model> LoadSingleModel(const std::string_view &pathStr);

        const std::string_view kRelModelsPath = "Resources/Models/";
        std::map<int, std::shared_ptr<const Model>> models_;
    };

    ModelBank::ModelBank()
        : p(std::make_shared<Pimpl>()) {
    }

    auto ModelBank::GetModel(int modelNameHash) const -> const Model * {
        return p->models_.at(modelNameHash).get();
    }

    auto ModelBank::GetAllModels() const -> std::map<int, std::shared_ptr<const Model>> {
        return p->models_;
    }

    void ModelBank::LoadModels() {
        using iterator = std ::filesystem::recursive_directory_iterator;
        auto abs_all_models_path = std::string(SDL_GetBasePath()) + p->kRelModelsPath.data();
        for (const auto &file_path : iterator(abs_all_models_path)) {
            auto path_str = file_path.path().string();
            auto extension_pos = path_str.find_last_of('.') + 1;
            auto file_extension = path_str.substr(extension_pos);
            if (file_extension != "dae")
                continue;
            auto loaded_model = p->LoadSingleModel(path_str);
            auto image_name_start = path_str.find_last_of('/') + 1;
            auto image_name_with_ext = path_str.substr(image_name_start);
            auto image_name_ext_pos = image_name_with_ext.find_last_of('.');
            auto file_name_no_ext = image_name_with_ext.substr(0, image_name_ext_pos);
            auto image_name_hash = Hash(file_name_no_ext);
            p->models_.insert({image_name_hash, loaded_model});
        }
    }

    std::shared_ptr<Model> ModelBank::Pimpl::LoadSingleModel(const std::string_view &pathStr) {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(pathStr.data(), 0);
        return ModelCreator::Get()->CreateModel(scene);
    }
}
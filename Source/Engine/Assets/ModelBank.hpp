#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Model;

    class ModelBank : public Singleton<ModelBank> {
      public:
        ModelBank();
        auto GetModel(int) const -> const Model *;
        auto GetAllModels() const -> std::map<int, std::shared_ptr<const Model>>;
        void LoadModels();

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
}
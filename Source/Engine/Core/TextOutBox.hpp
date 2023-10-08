#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class TextOutBox : public Singleton<TextOutBox> {
      public:
        TextOutBox();
        void Update();
        void Render() const;
        void Print(std::string_view, Color = Colors::wheat);

      private:
        int GetMaxNumLines() const;
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
}
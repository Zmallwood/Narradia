#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class ShaderProgram {
      public:
        ShaderProgram();
        bool Create(const GLchar *, const GLchar *);
        void Cleanup() const;
        GLuint GetProgramId();

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
}
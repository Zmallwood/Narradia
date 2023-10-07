//////////////////////////////////////////////////////////////////////
#include "ShaderProgram.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class ShaderProgram::Pimpl
    /*//////////////////////*/
    {
      public:
        GLuint InitVertexShader(const GLchar *);
        GLuint InitFragShader(const GLchar *);

        GLuint vertexShader = 0;
        GLuint fragShader = 0;
        GLuint programId;
    };

    ShaderProgram::ShaderProgram()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/
    {
    }

    bool
    ShaderProgram::Create(const GLchar *vertexShaderSrc, const GLchar *fragShaderSrc)
    /*/////////////////////////////////////////////////////////////////////////////*/
    {
        auto success = true;
        p->programId = glCreateProgram();
        auto vertexShaderCompiled = p->InitVertexShader(vertexShaderSrc);
        if (vertexShaderCompiled != GL_TRUE)
        /**********************************/
        {
            SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_ERROR, "Shader error", "Unable to compile vertex shader.", nullptr);
            printf("Unable to compile vertex shader %d!\n", p->vertexShader);
            success = false;
        }
        else
        /**/
        {
            glAttachShader(p->programId, p->vertexShader);
            auto fragShaderCompiled = p->InitFragShader(fragShaderSrc);
            if (fragShaderCompiled != GL_TRUE)
            /********************************/
            {
                SDL_ShowSimpleMessageBox(
                    SDL_MESSAGEBOX_ERROR, "Shader error", "Unable to compile fragment shader.",
                    nullptr);
                printf("Unable to compile fragment shader %d!\n", p->fragShader);
                success = false;
            }
            else
            /**/
            {
                glAttachShader(p->programId, p->fragShader);
                glLinkProgram(p->programId);
                GLint program_success = GL_TRUE;
                glGetProgramiv(p->programId, GL_LINK_STATUS, &program_success);
                if (program_success != GL_TRUE)
                /*****************************/
                {
                    SDL_ShowSimpleMessageBox(
                        SDL_MESSAGEBOX_ERROR, "Shader error", "Error linking shader program.",
                        nullptr);
                    printf("Error linking program %d!\n", p->programId);
                    success = false;
                }
            }
        }
        glDeleteShader(p->vertexShader);
        glDeleteShader(p->fragShader);
        return success;
    }

    void
    ShaderProgram::Cleanup() const
    /*//////////////////////////*/
    {
        glDeleteProgram(p->programId);
    }

    GLuint
    ShaderProgram::GetProgramId()
    /*/////////////////////////*/
    {
        return p->programId;
    }

    GLuint
    ShaderProgram::Pimpl::InitVertexShader(const GLchar *vertexShaderSource)
    /*////////////////////////////////////////////////////////////////////*/
    {
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        GLint vertexShaderCompiled = GL_FALSE;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexShaderCompiled);
        return vertexShaderCompiled;
    }

    GLuint
    ShaderProgram::Pimpl::InitFragShader(const GLchar *fragmentShaderSource)
    /*////////////////////////////////////////////////////////////////////*/
    {
        fragShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragShader);
        GLint fragShaderCompiled = GL_FALSE;
        glGetShaderiv(fragShader, GL_COMPILE_STATUS, &fragShaderCompiled);
        return fragShaderCompiled;
    }
}
//////////////////////////////////////////////////////////////////////
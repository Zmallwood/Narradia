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

        GLuint vertex_shader_ = 0;
        GLuint fragment_shader_ = 0;
        GLuint program_id_ = 0;
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
        p->program_id_ = glCreateProgram();
        auto vertex_shader_compiled = p->InitVertexShader(vertexShaderSrc);
        if (vertex_shader_compiled != GL_TRUE)
        /**********************************/
        {
            SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_ERROR, "Shader error", "Unable to compile vertex shader.", nullptr);
            printf("Unable to compile vertex shader %d!\n", p->vertex_shader_);
            success = false;
        }
        else
        /**/
        {
            glAttachShader(p->program_id_, p->vertex_shader_);
            auto fragment_shader_compiled = p->InitFragShader(fragShaderSrc);
            if (fragment_shader_compiled != GL_TRUE)
            /********************************/
            {
                SDL_ShowSimpleMessageBox(
                    SDL_MESSAGEBOX_ERROR, "Shader error", "Unable to compile fragment shader.",
                    nullptr);
                printf("Unable to compile fragment shader %d!\n", p->fragment_shader_);
                success = false;
            }
            else
            /**/
            {
                glAttachShader(p->program_id_, p->fragment_shader_);
                glLinkProgram(p->program_id_);
                GLint program_success = GL_TRUE;
                glGetProgramiv(p->program_id_, GL_LINK_STATUS, &program_success);
                if (program_success != GL_TRUE)
                /*****************************/
                {
                    SDL_ShowSimpleMessageBox(
                        SDL_MESSAGEBOX_ERROR, "Shader error", "Error linking shader program.",
                        nullptr);
                    printf("Error linking program %d!\n", p->program_id_);
                    success = false;
                }
            }
        }
        glDeleteShader(p->vertex_shader_);
        glDeleteShader(p->fragment_shader_);
        return success;
    }

    void
    ShaderProgram::Cleanup() const
    /*//////////////////////////*/
    {
        glDeleteProgram(p->program_id_);
    }

    GLuint
    ShaderProgram::GetProgramId()
    /*/////////////////////////*/
    {
        return p->program_id_;
    }

    GLuint
    ShaderProgram::Pimpl::InitVertexShader(const GLchar *vertexShaderSource)
    /*////////////////////////////////////////////////////////////////////*/
    {
        vertex_shader_ = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader_, 1, &vertexShaderSource, NULL);
        glCompileShader(vertex_shader_);
        GLint vertex_shader_compiled = GL_FALSE;
        glGetShaderiv(vertex_shader_, GL_COMPILE_STATUS, &vertex_shader_compiled);
        return vertex_shader_compiled;
    }

    GLuint
    ShaderProgram::Pimpl::InitFragShader(const GLchar *fragmentShaderSource)
    /*////////////////////////////////////////////////////////////////////*/
    {
        fragment_shader_ = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader_, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragment_shader_);
        GLint fragment_shader_compiled = GL_FALSE;
        glGetShaderiv(fragment_shader_, GL_COMPILE_STATUS, &fragment_shader_compiled);
        return fragment_shader_compiled;
    }
}
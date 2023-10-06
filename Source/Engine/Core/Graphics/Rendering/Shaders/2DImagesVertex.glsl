R"(
    #version 330 core
    layout (location = 0) in vec2 in_Position;
    layout (location = 1) in vec4 in_Color;
    layout (location = 2) in vec2 in_TexCoord;
    out vec4 ex_Color;
    out vec2 ex_TexCoord;
    void main() {
        gl_Position = vec4(in_Position.x, in_Position.y, 0.0, 1.0);
        ex_Color = in_Color;
        ex_TexCoord = in_TexCoord;
    }
)"
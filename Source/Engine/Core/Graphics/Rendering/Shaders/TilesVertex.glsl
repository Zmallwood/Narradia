R"(
    #version 330 core
    layout (location = 0) in vec3 in_Position;
    layout (location = 1) in vec4 in_Color;
    layout (location = 2) in vec2 in_TexCoord;
    layout (location = 3) in vec3  in_Normal;
    out vec4 ex_Color;
    out vec2 ex_TexCoord;
    out vec3  ex_Normal;
    out vec3 FragPos;
    uniform mat4 projection;
    uniform mat4 view;
    uniform mat4 model;
    void main() {
        gl_Position = projection * view * vec4(in_Position, 1.0);
        ex_Color = in_Color;
        ex_TexCoord = in_TexCoord;
        ex_Normal = in_Normal;
        FragPos = vec3(model * vec4(in_Position.x, in_Position.y, in_Position.z, 1.0));
    }
)"
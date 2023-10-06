R"(
    #version 330 core
    layout (location = 0) in vec2 in_Position;
    layout (location = 1) in vec2 in_TexCoord;
    out vec2 ex_TexCoord;
    uniform mat4 projection;
    uniform mat4 view;
    uniform vec3 particleCenterWorldspace;
    uniform vec2 billboardSize;
    void main() {
        gl_Position = projection * view * vec4(particleCenterWorldspace, 1.0);
        gl_Position /= gl_Position.w;
        gl_Position.xy += in_Position * billboardSize;
        ex_TexCoord = in_TexCoord;
    }
)"
R"(
    #version 330 core
    precision mediump float;
    in vec2 ex_TexCoord;
    uniform sampler2D TexUnit;
    out vec4 fragColor;
    void main() {
        if (texture(TexUnit, ex_TexCoord).a == 0.0)
            discard;
        fragColor = texture(TexUnit, ex_TexCoord).rgba;
    }
)"
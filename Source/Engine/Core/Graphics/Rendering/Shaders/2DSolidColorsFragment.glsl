R"(
    #version 330 core
    precision mediump float;
    in vec4 ex_Color;
    out vec4 fragColor;
    void main() {
        fragColor = ex_Color;
    }
)"
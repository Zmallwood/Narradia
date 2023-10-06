R"(
    #version 330 core
    precision mediump float;
    vec3 light_direction = vec3(4.5, -3.0, -4.5);
    vec3 light_ambient = vec3(0.8, 0.8, 0.8);
    vec3 light_color = vec3(1.2, 0.7, 0.9);
    in vec4 ex_Color;
    in vec2 ex_TexCoord;
    in vec3 ex_Normal;
    in vec3 FragPos;
    uniform sampler2D TexUnit;
    uniform float mAlpha;
    uniform vec3 viewPos;
    uniform vec3 fogColor;
    out vec4 fragColor;
    float nearPlane = 0.1;
    float farPlane = 65.0;
    float getFogFactor(float d, float nearPlane, float farPlane)
    {
        float FogMax = 1.0f * farPlane;
        float FogMin = 0.5f * farPlane;
        if (d>=FogMax) return 1.0f;
        if (d<=FogMin) return 0.0f;
        return 1.0f - (FogMax - d) / (FogMax - FogMin);
    }
    float getFogFactorAlpha(float d, float nearPlane, float farPlane)
    {
        float FogMax = 1.0f * farPlane;
        float FogMin = 0.7f * farPlane;
        if (d>=FogMax) return 1.0f;
        if (d<=FogMin) return 0.0f;
        return 1.0f - (FogMax - d) / (FogMax - FogMin);
    }
    void main() {
        vec4 color = vec4(ex_Color.rgb, texture(TexUnit, ex_TexCoord).a);
        if (color.a == 0.0)
            discard;
        vec3 mAmbient = light_ambient * texture(TexUnit, ex_TexCoord).rgb;
        vec3 norm = normalize(ex_Normal);
        vec3 lightDir = normalize(-light_direction);
        float diff = 3.5*max(dot(norm, lightDir), 0.15);
        vec3 mDiffuse = light_color * diff * texture(TexUnit, ex_TexCoord).rgb;
        vec3 result = mAmbient + mDiffuse;
        vec4 result2 = vec4(result.r*color.r, result.g*color.g, result.b*color.b, color.a);
        fragColor = result2;
        float d = distance(viewPos, FragPos);
        float fogFactor = getFogFactor(d, nearPlane, farPlane);
        float alpha = getFogFactorAlpha(d, nearPlane, farPlane);
        fragColor = mix(fragColor, vec4(fogColor, 1.0f), fogFactor);
        fragColor.a = 1-alpha;
    }
)"
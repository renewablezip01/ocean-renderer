#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 fColor;
out vec2 fTexCoord;
uniform int uTime;
uniform mat4 uTranslate;

void main()
{
    fColor = aColor;
    fTexCoord = aTexCoord;
    gl_Position = vec4(aPos, 1.0);
}

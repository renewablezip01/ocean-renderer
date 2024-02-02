#version 330 core
out vec4 FragColor;

in vec4 fColor;
in vec2 fTexCoord;

uniform sampler2D ourContainerTexture;
uniform sampler2D ourHappyFaceTexture;
uniform int uTime;

void main()
{
    vec4 happy = texture(ourHappyFaceTexture, fTexCoord * vec2(-1, 1));
    vec4 container = texture(ourContainerTexture, fTexCoord);
    FragColor = mix(container, happy, 0.2f);
} 

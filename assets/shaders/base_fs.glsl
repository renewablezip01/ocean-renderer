#version 330 core
out vec4 FragColor;

in vec4 fColor;
uniform int uTime;

void main()
{
    FragColor = fColor;
} 

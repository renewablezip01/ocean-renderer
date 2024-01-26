#version 330 core
out vec4 FragColor;

in vec4 fColor;

uniform vec4 ourColor;

void main()
{
    // FragColor = vec4(fColor);
    FragColor = ourColor;
} 

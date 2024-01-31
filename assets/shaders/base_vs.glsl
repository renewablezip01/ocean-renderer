#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

out vec4 fColor;
uniform int uTime;

void main()
{
    fColor = aColor;

    float t = uTime / 1000.0f;
    vec2 position = aPos.xy;

    // position.x = (aPos.x - 1.5) + mod(t, 3.0f);
    // position.y = (aPos.y - 1.5) + mod(t, 3.0f);

    position.x = aPos.x * cos(t) -  aPos.y * sin(t);
    position.y = aPos.x * sin(t) +  aPos.y * cos(t);

    gl_Position = vec4(position, 0.0, 1.0);
}

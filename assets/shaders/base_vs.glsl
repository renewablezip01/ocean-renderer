#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 fColor;
out vec2 fTexCoord;
uniform int uTime;

uniform mat4 uModel; // From localspace to world space by modifying every vertex
uniform mat4 uView; // From worldspace to viewspace by modifying what is to be shown on the screen (camera) 
uniform mat4 uProjection; // From viewspace to clipspace by normalizing all vectors within NDC range (-1, 1) and discarding everything outside (OpenGL)



void main()
{
    fColor = aColor;
    fTexCoord = aTexCoord;
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
}

#version 330 core

in vec3 vertex;
in vec3 color;

uniform mat4 TG;
uniform mat4 Proj;
uniform mat4 View;
uniform float Green;
uniform float RickVerd;


out vec3 fcolor;

void main() {
    if (Green == 1) fcolor = color*vec3(0,1,0);
    else fcolor = color;
    if (RickVerd == 1) fcolor = vec3(0, 0.7, 0);
    gl_Position = Proj * View * TG * vec4 (vertex, 1.0);
}

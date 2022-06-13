#version 450 core

uniform vec3 uColor;
layout (location=0) out vec4 fFragClr;

void main () {

fFragClr = vec4(uColor, 1.0);
}

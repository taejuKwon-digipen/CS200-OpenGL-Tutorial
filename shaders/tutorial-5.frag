#version 450 core

uniform int uColor;
uniform int model;
uniform int size;
layout(location=0) in vec3 vClrCoord;
layout (location=0) out vec4 fFragClr;

layout(location=1) in vec2 vTexCoord;
uniform sampler2D uTex2d;

void main () {

fFragClr = vec4(vClrCoord, 1.0);

fFragClr = texture(uTex2d, vTexCoord);
  
}
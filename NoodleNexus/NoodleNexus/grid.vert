#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 viewProjectionMatrix;

void main() {
    gl_Position = viewProjectionMatrix * vec4(aPos, 1.0);
}
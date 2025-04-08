#version 410
layout(location = 0) in vec3 vPos;
uniform mat4 matModel;
uniform mat4 matView;
uniform mat4 matProjection;

void main() {
    gl_Position = matProjection * matView * matModel * vec4(vPos, 1.0);
}
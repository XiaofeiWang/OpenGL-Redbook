#version 430 core

in vec2 tex_coord;

layout (location = 0) out vec4 fragColor;

uniform sampler2D gSampler;

void main(void)
{
    fragColor = texture(gSampler, tex_coord); //��ȡ����ʹ��sampler����������
}
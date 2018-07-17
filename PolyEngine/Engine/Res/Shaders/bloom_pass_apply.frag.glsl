#version 330 core

in vec2 vUV;

uniform sampler2D uImage;
uniform sampler2D uBloom;

out vec4 oColor;

void main()
{
    vec3 image = texture(uImage, vUV).rgb;
    vec3 bloom = texture(uBloom, vUV).rgb;
    oColor = vec4(image + bloom, 1.0);
}
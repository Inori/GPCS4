#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform texture2D texImage;
layout(binding = 2) uniform sampler texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main() 
{
    outColor = texture(sampler2D(texImage, texSampler), fragTexCoord) * vec4(fragColor, 1.0);
}
#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 fragTexCoord;
layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) out vec4 outColor;

void main() {
	outColor = texture(texSampler, fragTexCoord);
    //outColor = vec4(fragTexCoord, 0.0, 1.0);
    //outColor = vec4(0.5, 0.5, 0.0, 1.0);
}
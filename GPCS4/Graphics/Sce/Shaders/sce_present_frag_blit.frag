#version 450


layout(binding = 0) uniform sampler2D s_image;

layout(location = 0) in  vec2 i_coord;
layout(location = 0) out vec4 o_color;

layout(push_constant)
uniform present_info_t {
  ivec2 src_offset;
  uvec2 src_extent;
};

void main() {
  vec2 coord = vec2(src_offset) + vec2(src_extent) * i_coord;
  o_color = textureLod(s_image, coord, 0.0f);
}
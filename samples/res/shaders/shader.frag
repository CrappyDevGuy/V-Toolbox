#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 outColor;

layout(set = 1, binding = 1) uniform MATERIAL {
	vec4 ambient;
	vec4 diffuse;
} material;


void main(void)
{
  
  outColor = vec4(material.diffuse.xyz, 1.0f);
}

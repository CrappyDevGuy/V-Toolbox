#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormals;

layout(set = 0, binding = 0) uniform VIEW_MATRIX {
	mat4 viewMatrix;
	mat4 projMatrix;
} viewMatrix;

layout(set = 1, binding = 0) uniform MODEL_MATRIX {
	mat4 transformMatrix;
} modelMatrix;

layout(location = 4) out to_frag
{
	vec3 normals;
	vec3 vertexWorldPos;
} vs_out;

void main() {
	vec4 modelPosition = modelMatrix.transformMatrix * vec4(inPosition, 1.0);

	vs_out.normals				= (modelMatrix.transformMatrix * vec4(inNormals, 0.0f)).xyz;
  vs_out.vertexWorldPos = modelPosition.xyz;
  
	gl_Position = viewMatrix.projMatrix * viewMatrix.viewMatrix * modelPosition;
}

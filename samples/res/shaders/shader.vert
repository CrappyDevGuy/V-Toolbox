#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormals;
layout (location = 2) in vec2 inTexCoords;

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
	vec3 cameraVector;
	vec2 textureCoords;
} vs_out;

void main() {
	vec4 modelPosition = modelMatrix.transformMatrix * vec4(inPosition, 1.0);

	vs_out.normals				= (modelMatrix.transformMatrix * vec4(inNormals, 0.0f)).xyz;
  vs_out.vertexWorldPos = modelPosition.xyz;
  vs_out.cameraVector   = vec3(inverse(viewMatrix.viewMatrix) * vec4(0.0f, 0.0f, 0.0f, 1.0f)).xyz - modelPosition.xyz;
  vs_out.textureCoords  = inTexCoords;
  
	gl_Position = viewMatrix.projMatrix * viewMatrix.viewMatrix * modelPosition;
}

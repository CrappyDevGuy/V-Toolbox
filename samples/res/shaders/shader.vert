#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 inPosition;

layout(set = 0, binding = 0) uniform VIEW_MATRIX {
	mat4 viewMatrix;
	mat4 projMatrix;
} viewMatrix;

layout(set = 1, binding = 0) uniform MODEL_MATRIX {
	mat4 transformMatrix;
} modelMatrix;

void main() {
	vec4 modelPosition = modelMatrix.transformMatrix * vec4(inPosition, 1.0);
	  
	gl_Position = viewMatrix.projMatrix * viewMatrix.viewMatrix * modelPosition;
}

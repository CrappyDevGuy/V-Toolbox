#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 outColor;

layout(set = 1, binding = 1) uniform MATERIAL {
	vec4 ambient;
	vec4 diffuse;
} material;

layout(location = 4) in to_frag
{
	vec3 normals;
	vec3 vertexWorldPos;
} fs_in;

struct MaterialData
{
	vec3 ambient;
	vec3 diffuse;
};

vec3 calcPointLight(MaterialData c_mat, vec3 c_normal, vec3 c_vertexWorldPos)
{
	vec3 lightPos 		 = vec3(-2.0, 4.0, -2.0);
	vec3 toLightVector = normalize(lightPos - c_vertexWorldPos);

	float diffuseColor = max(dot(c_normal, toLightVector), 0.0);
	vec3 ambient = c_mat.ambient*c_mat.diffuse;
	vec3 diffuse = diffuseColor * c_mat.diffuse;
	return (ambient+diffuse);
}

void main(void)
{
	MaterialData mat = {
		material.ambient.xyz,
		material.diffuse.xyz
	};

	vec3 color = vec3(0.0f);
  color = calcPointLight(mat, fs_in.normals, fs_in.vertexWorldPos);
  outColor = vec4(color, 1.0f);
}

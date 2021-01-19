#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 outColor;

struct LightData
{
	vec4 position;
	vec4 diffuse;
	vec4 specular;
};

struct MaterialData
{
	vec4  ambient;
	vec4  diffuse;
	float textureFactor;
};

//_Per Mesh_//
layout(set = 1, binding = 1) uniform MATERIAL { MaterialData inMaterial; };
layout(set = 1, binding = 2) uniform sampler2D texSampler;

//_General_//
layout(set = 0, binding = 1) uniform LIGHT 		{ LightData 	 inLight;    };

layout(location = 4) in to_frag
{
	vec3 normals;
	vec3 vertexWorldPos;
	vec3 cameraVector;
	vec2 textureCoords;
} fs_in;

vec3 calcPointLight(MaterialData c_mat, LightData c_light, vec3 c_normal, vec3 c_vertexWorldPos, vec3 c_cameraVector)
{
	vec3 toLightVector = normalize(c_light.position.xyz - c_vertexWorldPos);

	float diffuseColor = max(dot(c_normal, toLightVector), 0.0f);

	vec3 reflectDirection = reflect(-toLightVector, c_normal);
	vec3 viewDirection = normalize(c_cameraVector);
	float spec = pow(max(dot(viewDirection, reflectDirection), 0.0f), 32);
	
	vec3 textureColor = texture(texSampler, fs_in.textureCoords).xyz;
	vec3 diffColor = mix(c_mat.diffuse.xyz, textureColor, c_mat.textureFactor);

	vec3 ambient  = c_mat.ambient.xyz*diffColor;
	vec3 diffuse  = c_light.diffuse.xyz * vec3(diffuseColor*diffColor);
	vec3 specular = c_light.specular.xyz * vec3(0.5f*spec);
	
	return (ambient+diffuse+specular);
}

void main(void)
{
	vec3 color = vec3(0.0f);
  color = calcPointLight(inMaterial, inLight, fs_in.normals, fs_in.vertexWorldPos, fs_in.cameraVector);
  outColor = vec4(color, 1.0f);
}

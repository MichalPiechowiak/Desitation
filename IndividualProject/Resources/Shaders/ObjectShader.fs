#version 330 core

in vec2 TexCoord;
in vec3 Normal; 
in vec3 Vertex;

uniform sampler2D texture_diffuse1;

uniform vec4		lightPosition1;
uniform vec4		lightPosition2;
uniform vec4		lightPosition3;
uniform vec4		lightPosition4;

uniform vec4		lightAmbient;
uniform vec4		lightDiffuse;
uniform vec3		lightAttenuation;

uniform vec4		matAmbient;
uniform vec4		matDiffuse;
uniform vec4        matSpecularColour;
uniform float       matSpecularExponent;

uniform vec3 eyePos;

out vec4 FragColour;

vec4 calculateLight(vec4 lightPos);

void main()
{    
	vec4 light1 = calculateLight(lightPosition1);
	vec4 light2 = calculateLight(lightPosition2);
	vec4 light3 = calculateLight(lightPosition3);
	vec4 light4 = calculateLight(lightPosition4);

	FragColour = light1 + light2 + light3 + light4;
}
vec4 calculateLight(vec4 lightPos)
{
	//Attenuation (drop-off light over distance)
	float d = length(lightPos.xyz - Vertex);
	float att = 1.0 / (lightAttenuation.x + lightAttenuation.y * d + lightAttenuation.z * (d * d));

	//Ambient light value
	vec4 texColour = texture(texture_diffuse1, TexCoord);
	vec4 ambient = lightAmbient * matAmbient * texColour * att;

	//Diffuse light value
	vec3 N = normalize(Normal);	
	vec3 L = normalize(lightPos.xyz - Vertex);
	float lambertTerm = clamp(dot(N, L), 0.0, 1.0);
	vec4 diffuse = lightDiffuse * matDiffuse * lambertTerm * texColour * att;

	//Specular light value
	vec3 E = normalize(eyePos - Vertex);
	vec3 R = reflect(-L, N);
	float specularIntensity = pow(max(dot(E, R), 0.0), matSpecularExponent);
	vec4 specular = matSpecularColour * specularIntensity * texColour * att;

	//Final colour is the combinatin of all components
	return ambient + diffuse + specular;
}
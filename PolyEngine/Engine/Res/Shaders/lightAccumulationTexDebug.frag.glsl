#version 430 core

#ifndef MAX_DIRLIGHT_COUNT
#define MAX_DIRLIGHT_COUNT 8
#endif

in VERTEX_OUT{
	vec3 positionInWorld;
	vec2 uv;
	vec3 normalInModel;
	mat3 tangentFromWorld;
	vec3 viewPositionInTangent;
	vec3 fragmentPositionInTangent;
} fragment_in;


struct DirectionalLight
{
	vec4 ColorIntensity;
	vec4 Direction;
};

struct Light
{
	vec4 Position;
	vec4 Color;
	vec4 RangeIntensity;
};

struct VisibleIndex
{
	int Index;
};

layout(std430, binding = 0) readonly buffer LightBuffer {
	Light data[];
} bLightBuffer;

layout(std430, binding = 1) readonly buffer VisibleLightIndicesBuffer {
	VisibleIndex data[];
} bVisibleLightIndicesBuffer;

struct Material
{
	vec4 Emissive;
	vec4 Albedo;
	float Roughness;
	float Metallic;
	float OpacityMaskThreshold;
};

const uint MAX_NUM_LIGHTS = 1024;

uniform sampler2D uBrdfLUT;
uniform samplerCube uIrradianceMap;
uniform samplerCube uPrefilterMap;

uniform sampler2D uEmissiveMap;
uniform sampler2D uAlbedoMap;
uniform sampler2D uRoughnessMap;
uniform sampler2D uMetallicMap;
uniform sampler2D uNormalMap;
uniform sampler2D uAmbientOcclusionMap;

uniform float uTime;
uniform Material uMaterial;

uniform DirectionalLight uDirectionalLight[8];
uniform int uDirectionalLightCount;

uniform int uLightCount;
uniform int uWorkGroupsX;
uniform int uWorkGroupsY;

layout(location = 0) out vec4 oColor;
layout(location = 1) out vec4 oNormal;

const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
	float a = roughness*roughness;
	float a2 = a*a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH*NdotH;

	float nom = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + 1.0);
	float k = (r*r) / 8.0;

	float nom = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
	return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

void main()
{
	mat3 WorldFromTangent = inverse(fragment_in.tangentFromWorld);

	// Debug texture input
	// oColor.rgb = emissive.rgb; // missing
	// oColor.rgb = albedo.rgb; // ok
	// oColor.rgb = normal.rgb * 0.5 + 0.5; // ok
	// oColor.rgb = (WorldFromTangent * normal.rgb) * 0.5 + 0.5;
	// oColor.rgb = vec3(roughness); // ok
	// oColor.rgb = vec3(metallic); // ok
	// oColor.rgb = vec3(ao); // ok
	// oColor.rgb = fragment_in.normalInModel * 0.5 + 0.5;
	
	vec3 normal = fragment_in.tangentFromWorld * fragment_in.normalInModel;

	vec3 normalInModel = fragment_in.normalInModel * 0.5 + 0.5;
	vec3 brdf = vec3(texture(uBrdfLUT, fragment_in.uv).rg, 0.0);
	vec3 irr = texture(uIrradianceMap, fragment_in.normalInModel).rgb;
	vec3 pref = texture(uPrefilterMap, fragment_in.normalInModel).rgb;
	vec3 em = 0.5*texture(uEmissiveMap, fragment_in.uv).rgb;
	vec3 al = texture(uAlbedoMap, fragment_in.uv).rgb;
	vec3 rg = texture(uRoughnessMap, fragment_in.uv).rrr;
	vec3 mt = texture(uMetallicMap, fragment_in.uv).rrr;
	vec3 nn = normalize(texture(uNormalMap, fragment_in.uv).rgb * 2.0 - 1.0);
	vec3 aa = texture(uAmbientOcclusionMap, fragment_in.uv).rrr;
	oColor.rgb = mix(brdf, normalInModel, smoothstep(-0.5, 0.5, sin(4.0 * uTime)));
	
	int t = int(floor(10.0 * fract(0.1*uTime)));
	switch (t)
	{
		case 0:
			oColor.rgb = brdf;
			break;
		case 1:
			oColor.rgb = irr;
			break;
		case 2:
			oColor.rgb = pref;
			break;
		case 3:
			oColor.rgb = em;
			break;
		case 4:
			oColor.rgb = al;
			break;
		case 5:
			oColor.rgb = rg;
			break;
		case 6:
			oColor.rgb = mt;
			break;
		case 7:
			oColor.rgb = nn;
			break;
		case 9:
			oColor.rgb = aa;
			break;
		case 8:
			oColor.rgb = normalInModel;
			break;
		default:
			oColor.rgb = normalInModel;
			break;
	}

	oNormal.rgb = (WorldFromTangent * normal) * 0.5 + 0.5;
}

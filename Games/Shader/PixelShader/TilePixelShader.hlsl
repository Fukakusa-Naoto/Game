#include "../../Library/Shader/Header/Primitive3D.hlsli"

float4 main(PS_INPUT input) : SV_TARGET
{
	float alpha = 1 - (abs(dot(normalize(float3(0.0f, -14.0f, -9.0f)), input.normal)));
	input.color.a = alpha;
	return input.color;
}
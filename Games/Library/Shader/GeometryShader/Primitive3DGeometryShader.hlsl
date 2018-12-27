#include "../Header/Primitive3D.hlsli"

[maxvertexcount(3)]
void main(
	triangle PS_INPUT input[3],
	inout TriangleStream< PS_INPUT > output
)
{
	PS_INPUT element;

	// 法線ベクトルの計算
    float3 faceEdge = input[0].position.xyz / input[0].position.w;
    float3 faceEdgeA = (input[1].position.xyz / input[1].position.w) - faceEdge;
    float3 faceEdgeB = (input[2].position.xyz / input[2].position.w) - faceEdge;
    element.normal = normalize(cross(faceEdgeA, faceEdgeB));


	for (uint i = 0; i < 3; i++)
	{

		element.position = input[i].position;

		element.position = mul(element.position, projection);

		element.color = input[i].color;

		output.Append(element);
	}
	output.RestartStrip();
}

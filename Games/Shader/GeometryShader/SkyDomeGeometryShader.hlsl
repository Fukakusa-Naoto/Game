#include "../Header/SkyDomeHeader.hlsli"

static const int vnum = 4;

static const float4 offset_array[vnum] =
{
    float4(-0.5f, -0.5f, 0, 0), // ����
	float4(-0.5f, +0.5f, 0, 0), // ����
	float4(+0.5f, -0.5f, 0, 0), // �E��
	float4(+0.5f, +0.5f, 0, 0), // �E��

};

[maxvertexcount(vnum)]
void main(
	point PS_INPUT input[1],
	inout TriangleStream<PS_INPUT> output
)
{
    for (int i = 0; i < vnum; i++)
    {
        PS_INPUT element;

        element.Pos = input[0].Pos + offset_array[i] * 1.0f;

        element.Pos = mul(element.Pos, world);
        element.Pos = mul(element.Pos, view);
        element.Pos = mul(element.Pos, proj);

        element.Tex = float2(offset_array[i].x, -offset_array[i].y) + float2(0.5f, 0.5f);
        output.Append(element);
    }
    output.RestartStrip();
}
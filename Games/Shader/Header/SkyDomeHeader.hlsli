struct VS_INPUT
{
    float3 Pos : POSITION;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD;
};


cbuffer ConstBuffer : register(b0)
{
    matrix world;
    matrix view;
    matrix proj;
    float4 time;
};
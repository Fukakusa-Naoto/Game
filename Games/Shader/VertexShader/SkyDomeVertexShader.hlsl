#include "../Header/SkyDomeHeader.hlsli"

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;

    output.Pos = float4(input.Pos, 1);
    output.Tex = float2(0, 0);

    return output;
}
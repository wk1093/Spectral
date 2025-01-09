struct VS_Input
{
    float3 pos : POS;
    float3 color : COL;
};

struct VS_Output
{
    float4 position : SV_POSITION;
    float4 color : COLR;
};

VS_Output main(VS_Input input)
{
    VS_Output output;
    output.position = float4(input.pos, 1.0f);
    output.color = float4(input.color, 1.0f);

    return output;
}
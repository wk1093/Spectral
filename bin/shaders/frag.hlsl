struct VS_Output
{
    float4 position : SV_POSITION;
    float4 color : COLR;
};

float4 main(VS_Output input) : SV_TARGET
{
    return input.color;
}
struct SPSL_VS_Input {
 float3 pos:C_0e;
 float3 normal:C_1e;
 float3 color:C_2e;
};
struct SPSL_VS_Output {
float4 position : SV_POSITION; float3 obj_color:C_obj_colore;
 float3 vertex_pos:C_vertex_pose;
 float3 normal_out:C_normal_oute;
};
cbuffer SPSL_Uniforms {
};










SPSL_VS_Output main(SPSL_VS_Input input) { SPSL_VS_Output output; 
    output.position = float4(input.pos, 1.0);
    output.vertex_pos = input.pos;
    output.obj_color = input.color;
    output.normal_out = input.normal;
 return output; }

struct SPSL_FS_Input {
float4 position : SV_POSITION; float3 obj_color:C_obj_colore;
 float3 vertex_pos:C_vertex_pose;
 float3 normal_out:C_normal_oute;
};
cbuffer SPSL_Uniforms {
 float3 uColorMult;
 float uTime;
};







float4 main(SPSL_FS_Input input) : SV_TARGET { float4 spsl_fragcolor; 
    float3 lightPos = float3(0.0, 5.0, 1.0);
    float3 lightDir = normalize(lightPos - input.vertex_pos);
    float3 normal = normalize(input.normal_out);
    float lightIntensity = max(dot(normal, lightDir), 0.0);
    float3 lightColor = float3(1.0, 1.0, 1.0);
    float3 color = input.obj_color;
    spsl_fragcolor = float4(color, 1.0);
 return spsl_fragcolor; }
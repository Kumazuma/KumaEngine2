cbuffer ObjectCBuffer : register(b0)
{
	float4 maskcolor4;
}
cbuffer ObjectCBuffer2 : register(b2)
{
	float4 maskcolor;
	float4 maskcolor1;
	float4 maskcolor2;
}
struct PSInput
{
	float4 vPosition:SV_POSITION;
	float2 vTex:TEXCOORD0;
};
struct PSOutput
{
	float4 vTarget0:SV_TARGET0;
	float4 vTarget1:SV_TARGET1;
};
Texture2D gTexture:register(t0);
SamplerState gSampleType;
PSOutput ps_main(PSInput input)
{
	PSOutput output;
	output.vTarget0 = gTexture.Sample(gSampleType, input.vTex);
	output.vTarget1 = float4(0, 0, 0, 0);
	output.vTarget1.r = maskcolor.r * output.vTarget0.a;
	output.vTarget1.g = (1.f - maskcolor.g) * output.vTarget0.a;
	return output;
}
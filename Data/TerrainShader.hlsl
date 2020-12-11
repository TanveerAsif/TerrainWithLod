cbuffer MatrixBuffer: register(b0)
{
	matrix worldMat;
	matrix viewMat;
	matrix projMat;
};

struct VertexInput
{
	float3 Pos : POSITION;
	float2 tex  : TEXCOORD0;
	float3 norm : NORMAL;
};



struct PixelInput
{
	float4 position : SV_POSITION;
	float2 tex  : TEXCOORD0;
	float3 normal : NORMAL;	

	float4 worldPos : TEXCOORD1;
};


////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////       VS : Vertex Shader                //////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
PixelInput MyVertexShader(VertexInput input)
{
    PixelInput output;
    
	output.position = float4(input.Pos, 1.0f);
	output.position = mul(output.position, worldMat);
	output.worldPos = output.position;
	output.position = mul(output.position, viewMat);
	output.position = mul(output.position, projMat);
	
	
	output.normal = input.norm;
	output.tex = input.tex;
    
    return output;
};

////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////       PS : Pixel Shader                //////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
Texture2D		RockTexture : register(t0);
Texture2D		IceTexture : register(t1);
SamplerState textureSampler;

float4 MyPixelShader(PixelInput input) : SV_TARGET
{
	float4 rockColor = RockTexture.Sample(textureSampler, input.tex);
	float4 iceColor = IceTexture.Sample(textureSampler, input.tex);
	//if(input.worldPos.y > 5.0)
	//float4 FinalColor = rockColor + iceColor * input.worldPos.y * 0.25;
	float scalarFactor = input.worldPos.y * 0.25;
	//iceColor = float4(0, 1, 0, 1.0);
	float4 FinalColor = lerp(rockColor, iceColor, scalarFactor);
	return FinalColor;

	//return  float4(0, 1, 0, 1.0);
};
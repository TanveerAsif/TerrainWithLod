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
};


////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////       VS : Vertex Shader                //////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
PixelInput MyVertexShader(VertexInput input)
{
    PixelInput output;
    
	output.position = float4(input.Pos, 1.0f);
	output.position = mul(output.position, worldMat);
	output.position = mul(output.position, viewMat);
	output.position = mul(output.position, projMat);
	
	
	output.normal = input.norm;
	output.tex = input.tex;
    
    return output;
};

////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////       PS : Pixel Shader                //////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
Texture2D		myTexture;
SamplerState textureSampler;
float4 MyPixelShader(PixelInput input) : SV_TARGET
{
	float4 texColor = myTexture.Sample(textureSampler, input.tex);
	//return float4(input.normal.x, 0, 0, 1.0);
	return texColor;
};
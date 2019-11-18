//=============================================================================
//	LogoPixelShader.hlsl
//
//	pixel shader file of logo shader.
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "LogoShaderData.hlsli"

Texture2D renderTexture : register(t0);
SamplerState renderTextureSamplerState : register(s0);

PSOutput main(PSInput _input){
	PSOutput output = (PSOutput)0;

	float4 textureColor = renderTexture.Sample(renderTextureSamplerState, _input.texcoord);
	output.color = textureColor * colorRate;

	return output;
}

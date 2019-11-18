//=============================================================================
//	OutputScreenPixelShader.hlsl
//
//	pixel shader file of output screen shader.
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "OutputScreenShaderData.hlsli"

Texture2D renderTexture : register(t0);
SamplerState renderTextureSamplerState : register(s0);

PSOutput main(PSInput _input){
	PSOutput output = (PSOutput)0;

	output.color = renderTexture.Sample(renderTextureSamplerState, _input.texcoord);

	return output;
}

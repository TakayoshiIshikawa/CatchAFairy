//=============================================================================
//	DirectionalLightPixelShader.hlsl
//
//	pixel shader file of directional light shader.
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "DirectionalLightShaderData.hlsli"

Texture2D diffuseTexture : register(t0);
SamplerState renderTextureSamplerState : register(s0);

PSOutput main(PSInput _input){
	PSOutput output = (PSOutput)0;

	float4 diffuse = diffuseTexture.Sample(renderTextureSamplerState, _input.texcoord);
	clip(diffuse.a - 0.99f);

//	output.normal = 0.5f*_input.normal + 0.5f;
//	output.normal.w = 1.0f;

	output.position = _input._pos;

	return output;
}

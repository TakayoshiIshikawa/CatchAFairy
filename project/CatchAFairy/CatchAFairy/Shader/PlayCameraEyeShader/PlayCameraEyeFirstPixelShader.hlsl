//=============================================================================
//	PlayCameraEyeFirstPixelShader.hlsl
//
//	pixel shader file of play camera eye first shader.
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "PlayCameraEyeFirstShaderData.hlsli"

Texture2D diffuseTexture : register(t0);
Texture2D normalTexture : register(t1);
SamplerState renderTextureSamplerState : register(s0);

PSOutput main(PSInput _input){
	PSOutput output = (PSOutput)0;

	output.diffuse = diffuseTexture.Sample(renderTextureSamplerState, _input.texcoord);
	clip(output.diffuse.a - 0.9f);

	float3 nl = 2.0f*normalTexture.Sample(renderTextureSamplerState, _input.texcoord).xyz - float3(1.0f, 1.0f, 1.0f);
	output.normal.xyz = nl.x*_input.tangent.xyz + nl.y*_input.binormal.xyz + nl.z*_input.normal.xyz;
	output.normal.w = 1.0f;

	output.position = _input._pos;

	return output;
}

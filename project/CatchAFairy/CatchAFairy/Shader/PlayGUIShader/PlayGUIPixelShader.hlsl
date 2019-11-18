//=============================================================================
//	PlayGUIPixelShader.hlsl
//
//	pixel shader file of play scene graphic user interface shader.
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "PlayGUIShaderData.hlsli"

Texture2D shapeRenderTexture : register(t0);
Texture2D viewRenderTexture : register(t1);
SamplerState renderTextureSamplerState : register(s0);

PSOutput main(PSInput _input){
	clip(_input.color.a - 0.000001f);

	PSOutput output = (PSOutput)0;

	float shape = shapeRenderTexture.Sample(renderTextureSamplerState, _input.shapeTexcoord.xy).r;
	float4 viewColor = viewRenderTexture.Sample(renderTextureSamplerState, _input.viewTexcoord.xy);
	float3 rgb = viewColor.rgb * _input.color.rgb;
	output.color = float4(rgb, viewColor.a*_input.color.a*shape);

	return output;
}

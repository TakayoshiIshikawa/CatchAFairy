//=============================================================================
//	DirectionalLightVertexShader.hlsl
//
//	vertex shader file of directional light shader.
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "DirectionalLightShaderData.hlsli"

VSOutput main(VSInput _input){
	VSOutput output = (VSOutput)0;

	output.position = float4(_input.position, 1.0f);
	output.normal	= float4(_input.normal, 0.0f);
	output.texcoord = _input.texcoord;

	return output;
}

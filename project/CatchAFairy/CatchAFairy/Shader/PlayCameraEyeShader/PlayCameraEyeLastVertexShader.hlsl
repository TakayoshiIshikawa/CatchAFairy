//=============================================================================
//	PlayCameraEyeLastVertexShader.hlsl
//
//	vertex shader file of play camera eye last shader.
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "PlayCameraEyeLastShaderData.hlsli"

VSOutput main(VSInput _input){
	VSOutput output = (VSOutput)0;

	output.position = float4(_input.position, 0.0f, 1.0f);
	output.texcoord = _input.texcoord;

	return output;
}

//=============================================================================
//	CameraEyeReflectionLightVertexShader.hlsl
//
//	vertex shader file of camera eye reflection light shader.
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "CameraEyeReflectionLightShaderData.hlsli"

VSOutput main(VSInput _input){
	VSOutput output = (VSOutput)0;

	output.color = _input.color;
	output.lightViewPos = float4(_input.position, 1.0f);
	output.radiusAndLumen = _input.radiusAndLumen;

	return output;
}

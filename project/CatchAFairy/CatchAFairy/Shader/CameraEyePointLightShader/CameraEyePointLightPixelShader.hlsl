//=============================================================================
//	CameraEyePointLightPixelShader.hlsl
//
//	pixel shader file of camera eye point light shader.
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "CameraEyePointLightShaderData.hlsli"

Texture2D positionTexture : register(t0);
SamplerState renderTextureSamplerState : register(s0);

PSOutput main(PSInput _input){
	clip(_input.color.a - 0.001f);
	clip(_input.radiusAndLumen.y - 0.001f);

	PSOutput output = (PSOutput)0;

	float3 delta = _input.lightViewPos.xyz - _input.viewPos.xyz;

	float d = max(0.0f, length(delta)-_input.radiusAndLumen.x) + 1.0f;
	float lux = 1.0f / (d*d);
	float texRate = 1.0f;
	{
		const float DIST = 0.5f;
		float r = 1.0f - (1.0f/DIST)*max(0.0f, min(DIST, length(_input.texcoord2)-1.0f+DIST));
		texRate = r;
	}
	float alphaRate = lux * texRate;

	output.pointColor.rgb = _input.color.rgb;
	output.pointColor.a = alphaRate * _input.color.a;

	return output;
}

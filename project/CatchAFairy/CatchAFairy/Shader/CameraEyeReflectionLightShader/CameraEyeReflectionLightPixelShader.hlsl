//=============================================================================
//	CameraEyeReflectionLightPixelShader.hlsl
//
//	pixel shader file of camera eye reflection light shader.
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "CameraEyeReflectionLightShaderData.hlsli"

Texture2D positionTexture : register(t0);
Texture2D normalTexture : register(t1);
SamplerState renderTextureSamplerState : register(s0);

PSOutput main(PSInput _input){
	clip(_input.color.a - 0.001f);
	clip(_input.radiusAndLumen.y - 0.03f);

	float4 projectionPosition = positionTexture.Sample(renderTextureSamplerState, _input.texcoord);
	clip(projectionPosition.w - 0.1f);

	float3 normal = normalize(normalTexture.Sample(renderTextureSamplerState, _input.texcoord).xyz);

	float4 viewPosition = mul(projectionPosition, invertProjectionMatrix);
	viewPosition /= viewPosition.w;

	float3 deltaPosToLight = _input.lightViewPos.xyz - viewPosition.xyz;
	float lx = _input.radiusAndLumen.y / (max(0.0f, length(deltaPosToLight)-_input.radiusAndLumen.x) + 1.0f);
	clip(lx - 0.01f);

	float3 directionPosToLight = normalize(deltaPosToLight.xyz);
	float directReflectionRate = max(0.0f, dot(directionPosToLight, normal));

	float3 directionViewPos = normalize(viewPosition.xyz);
	float3 reflectionVector = reflect(directionViewPos, normal);
	float diffuseReflectionRate = max(0.0f, dot(directionPosToLight, reflectionVector));

	float alphaRate = lx*(directReflectionRate+diffuseReflectionRate);

	PSOutput output = (PSOutput)0;
	output.reflectionColor.rgb = _input.color.rgb;
	output.reflectionColor.a = alphaRate * _input.color.a;

	return output;
}

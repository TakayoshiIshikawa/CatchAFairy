//=============================================================================
//	CameraEyePointLightShaderData.hlsli
//
//	header file of camera eye point light shader.
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================

struct VSInput{
	float3 position			: SV_POSITION0;
	float4 color			: COLOR0;
	float2 radiusAndLumen	: TEXCOORD0;
};

struct VSOutput{
	float4 position			: SV_POSITION0;
	float4 color			: COLOR0;
	float2 texcoord			: TEXCOORD0;
	float4 viewPos			: TEXCOORD1;
	float4 lightViewPos		: TEXCOORD2;
	float2 radiusAndLumen	: TEXCOORD3;
	float2 texcoord2		: TEXCOORD4;
};

typedef VSOutput PSInput;

struct PSOutput{
	float4 pointColor : SV_TARGET0;
};


cbuffer ConstantBuffer : register(b0){
	float4x4 worldViewProjectionMatrix;
	float4x4 worldViewMatrix;
	float4x4 projectionMatrix;
	float4x4 invertProjectionMatrix;
};

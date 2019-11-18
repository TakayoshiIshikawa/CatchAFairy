//=============================================================================
//	PlayCameraEyeFirstShaderData.hlsli
//
//	header file of play camera eye first shader.
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================

struct VSInput{
	float3 position : SV_POSITION0;
	float3 normal	: NORMAL0;
	float2 texcoord : TEXCOORD0;
};

struct VSOutput{
	float4 position : SV_POSITION0;
	float4 normal	: NORMAL0;
	float2 texcoord : TEXCOORD0;
	float4 _pos		: TEXCOORD1;
	float4 tangent	: TEXCOORD2;
	float4 binormal	: TEXCOORD3;
};

typedef VSOutput PSInput;

struct PSOutput{
	float4 diffuse	: SV_TARGET0;
	float4 normal	: SV_TARGET1;
	float4 position	: SV_TARGET2;
};


cbuffer ConstantBuffer : register(b0){
	float4x4 modelToProjectionMatrix;
};

//=============================================================================
//	OutputScreenGeometryShader.hlsl
//
//	geometry shader file of output screen shader.
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "OutputScreenShaderData.hlsli"

[maxvertexcount(3)]
void main(
	triangle VSOutput _input[3],
	inout TriangleStream<PSInput> _output
){
	for(int i=0; i<3; ++i){
		PSInput element = (PSInput)0;

		element.position = _input[i].position;
		element.texcoord = _input[i].texcoord;

		_output.Append(element);
	}
	_output.RestartStrip();
}

#pragma once

class Shader
{
public:
	Shader();
	virtual ~Shader();
};

class VertexShader : public Shader
{
public:
	VertexShader();
	virtual ~VertexShader();

	virtual void Execute(const VS_Input* input, VS_Output*  output) = 0;
};

class FragmentShader : public Shader
{
public:
	FragmentShader();
	virtual ~FragmentShader();

	virtual bool Execute(const VS_Output* input, PS_Output* output, float* pDepthIO) = 0;
};
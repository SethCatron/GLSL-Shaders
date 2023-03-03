#pragma once
#include <GL/glew.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>

class Program
{
public:
	
	Program();
	~Program();
	void SetShadersFileName(char *vFileName, char *sFileName);
	void CheckShaderCompileStatus(GLuint shader);
	void Init();
	std::string ReadShader(const char *name);
	void SendAttributeData(std::vector<float> &buffer, const char* name);
	void SendUniformData(int a, const char* name);
	void SendUniformData(float a, const char* name);
	void SendUniformData(glm::vec3 input, const char* name);
	void SendUniformData(glm::mat4 &mat, const char* name);
	void Bind();
	void Unbind();
	// float maxer(float zero, float dot1, float dot2);
	// float Gouraud(float ka, float kd, float ks, float s, float diffuse, float specular);
	// void assignMaterials();
	// void assignLights();


private:
	GLint programID;
	char *vertexShaderFileName, *fragmentShaderFileName;
};


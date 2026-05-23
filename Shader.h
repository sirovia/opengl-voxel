#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <string_view>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	unsigned int id;

	Shader(std::string_view vertexPath, std::string_view fragmentPath);

	void use() const;
	void setBool(std::string_view name, bool val) const;
	void setInt(std::string_view name, int val) const;
	void setFloat(std::string_view name, float val) const;
	void setMat4(std::string_view name, glm::mat4 val) const;

private:
	int loc(std::string_view name) const;

	static std::string readFile(std::string_view path);

	static void checkErrors(unsigned int target, std::string_view type);

	static unsigned int compile(GLenum type, const char* src);

};


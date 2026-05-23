#include "Shader.h"

Shader::Shader(std::string_view vertexPath, std::string_view fragmentPath)
{
	std::string vertSrc = readFile(vertexPath);
	std::string fragSrc = readFile(fragmentPath);

	const char* vCode = vertSrc.c_str();
	const char* fCode = fragSrc.c_str();

	unsigned int vert = compile(GL_VERTEX_SHADER, vCode);
	unsigned int frag = compile(GL_FRAGMENT_SHADER, fCode);

	id = glCreateProgram();
	glAttachShader(id, vert);
	glAttachShader(id, frag);
	glLinkProgram(id);
	checkErrors(id, "PROGRAM");

	glDeleteShader(vert);
	glDeleteShader(frag);
}

void Shader::use() const
{
	glUseProgram(id);
}

void Shader::setBool(std::string_view name, bool val) const
{
	glUniform1i(loc(name), val);
}

void Shader::setInt(std::string_view name, int val) const
{
	glUniform1i(loc(name), val);
}

void Shader::setFloat(std::string_view name, float val) const
{
	glUniform1f(loc(name), val);
}

void Shader::setMat4(std::string_view name, glm::mat4 val) const
{
	glUniformMatrix4fv(loc(name), 1, GL_FALSE, glm::value_ptr(val));
}


int Shader::loc(std::string_view name) const
{
	return glGetUniformLocation(id, name.data());
}

std::string Shader::readFile(std::string_view path)
{
	std::ifstream file(path.data());
	if (!file) {
		std::cerr << "File not found: " << path;
		return "";
	}
	std::ostringstream ss;
	ss << file.rdbuf();
	return ss.str();
}

void Shader::checkErrors(unsigned int target, std::string_view type)
{
	int success;
	char infoLog[1024];
	if (type == "PROGRAM") {
		glGetProgramiv(target, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(target, 1024, nullptr, infoLog);
			std::cerr << "PROGRAM ERROR\n" << infoLog << '\n';
		}
	}
	else {
		glGetShaderiv(target, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(target, 1024, nullptr, infoLog);
			std::cerr << "SHADER ERROR\n" << infoLog << '\n';
		}
	}
}

unsigned int Shader::compile(GLenum type, const char* src)
{
	unsigned int shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);
	checkErrors(shader, type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT");
	return shader;
}

#include "shader.hpp"
#include "core/localFile.hpp"
#include <stdio.h>

using namespace core;

ShaderProgram::ShaderProgram(const std::string &vertexPath,
                             const std::string &fragPath) {
  // Get the shader sources from disc
  std::string vSource = LocalFile::ReadFileData(vertexPath);
  std::string fSource = LocalFile::ReadFileData(fragPath);
  const char *vertexSource = vSource.c_str();
  const char *fragSource = fSource.c_str();

  // Create shaders and compile them
  this->vertexId = glCreateShader(GL_VERTEX_SHADER);
  this->fragId = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(this->vertexId, 1, &vertexSource, NULL);
  glShaderSource(this->fragId, 1, &fragSource, NULL);
  this->compileShader(this->vertexId);
  this->compileShader(this->fragId);

  // Create and attach our shaders to the program
  this->programId = glCreateProgram();
  glAttachShader(this->programId, this->vertexId);
  glAttachShader(this->programId, this->fragId);

  // Link our shaders to the active program
  this->linkProgram();

  // Delete unneeded shaders
  glDeleteShader(this->vertexId);
  glDeleteShader(this->vertexId);
}

void ShaderProgram::use() { glUseProgram(this->programId); }

void ShaderProgram::setUniform(const std::string &uniform, int data) {
  GLuint location = this->getUniformLocation(uniform);
  glUniform1i(location, data);
}

void ShaderProgram::setUniform(const std::string &uniform, float data) {
  GLuint location = this->getUniformLocation(uniform);
  glUniform1f(location, data);
}

void ShaderProgram::setUniform(const std::string &uniform, glm::vec3 data) {
  GLuint location = this->getUniformLocation(uniform);
  glUniform3f(location, data.x, data.y, data.z);
}
void ShaderProgram::setUniform(const std::string &uniform, glm::vec4 data) {
  GLuint location = this->getUniformLocation(uniform);
  glUniform4f(location, data.x, data.y, data.z, data.w);
}

int ShaderProgram::getVertexId() { return this->vertexId; }
int ShaderProgram::getFragId() { return this->fragId; }
int ShaderProgram::getProgramId() { return this->programId; }

void ShaderProgram::compileShader(GLuint shader) {
  constexpr int buffer_size = 512;
  glCompileShader(shader);
  int success;
  char log[buffer_size];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (success) // TODO: Handle compile errors properly
    return;
  glGetShaderInfoLog(shader, buffer_size, NULL, log);
  printf("Failed to compile shader %d with log:\n %s\n", shader, log);
}

void ShaderProgram::linkProgram() {
  constexpr int buffer_size = 512;
  glLinkProgram(this->programId);
  int success;
  char log[buffer_size];
  glGetProgramiv(this->programId, GL_LINK_STATUS, &success);
  if (success)
    return;
  glGetProgramInfoLog(this->programId, buffer_size, NULL, log);
  printf("Failed to link program %d with log:\n %s\n", this->programId, log);
}

GLuint ShaderProgram::getUniformLocation(const std::string &uniform) {
  return glGetUniformLocation(this->programId, uniform.c_str());
}

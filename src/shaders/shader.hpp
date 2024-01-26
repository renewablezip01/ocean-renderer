#pragma once

class ShaderProgram {
public:
  ShaderProgram(const std::string &vertexPath, const std::string &fragPath);

public:
  void use();

  void setUniform(const std::string &name, int data);
  void setUniform(const std::string &name, float data);
  void setUniform(const std::string &name, glm::vec3 data);
  void setUniform(const std::string &name, glm::vec4 data);

  int getVertexId();
  int getFragId();
  int getProgramId();

private:
  void compileShader(GLuint shader);
  void linkProgram();
  GLuint getUniformLocation(const std::string &uniform);

private:
  int programId;
  int vertexId;
  int fragId;
};

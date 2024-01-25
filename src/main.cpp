#include "SDL_error.h"
#include "SDL_keycode.h"
#include "SDL_video.h"
#include "core/localFile.hpp"
#include <GLES/gl.h>
#include <GLES2/gl2.h>
#include <SDL.h>
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <stdio.h>
#include <string>

#define GP_ERR(exp, msg) assert(((void)msg, exp))

struct Vertex {
  glm::vec3 position;
  glm::vec4 color;
  Vertex(const glm::vec3 &position, const glm::vec4 &color)
      : position(position), color(color) {}
};

int main(int argc, char *argv[]) {
  int wnd_width = 800, wnd_height = 600;
  // Initialize SDL
  GP_ERR(SDL_Init(SDL_INIT_EVENTS) >= 0, "Failed to initialize SDL");

  int context_flags =
      SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG | SDL_GL_CONTEXT_DEBUG_FLAG;

  // Setup SDL window attributes for OpenGL
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, context_flags);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  // Create our window using SDL
  SDL_Window *main_window = SDL_CreateWindow(
      "Sandbox", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, wnd_width,
      wnd_height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

  GP_ERR(main_window != NULL, "Failed to create window");
  SDL_GLContext gl_context = SDL_GL_CreateContext(main_window);

  // Create our OpenGL context and make it current
  GP_ERR(gl_context != NULL, "Failed to create OpenGL context in SDL");
  SDL_GL_MakeCurrent(main_window, gl_context);

  GP_ERR(gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress),
         "Failed to initialize GLAD");

  // Enable alpha blending after loading glad
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  static const Vertex vertices[] = {
      Vertex(glm::vec3(0.5f, 0.5f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)),
      Vertex(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)),
      Vertex(glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)),
      Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)),
  };

  static const unsigned int indices[] = {
      // First triangle
      0, 1, 2,
      // Second triangle
      1, 3, 2
      // End
  };

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Create a VBO = vertex buffer object to store our vertices
  GLuint vertexbuffer;
  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  GLuint ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, sizeof(glm::vec3) / sizeof(float), GL_FLOAT,
                        GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, position));

  glVertexAttribPointer(1, sizeof(glm::vec4) / sizeof(float), GL_FLOAT,
                        GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, color));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);

  // Create our base vertex shader
  std::string vertexShaderSrc =
      core::LocalFile::ReadFileData("./src/shaders/base_vs.glsl");
  GLuint vertShaderId = glCreateShader(GL_VERTEX_SHADER);
  const char *vertexShaderRawSource = vertexShaderSrc.c_str();
  glShaderSource(vertShaderId, 1, &vertexShaderRawSource, NULL);
  glCompileShader(vertShaderId);

  // Check if the shader compilation succeeded
  int success;
  char infoLog[512];
  glGetShaderiv(vertShaderId, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertShaderId, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  // Create our base fragment shader
  std::string fragShaderSrc =
      core::LocalFile::ReadFileData("./src/shaders/base_fs.glsl");
  GLuint fragShaderId = glCreateShader(GL_FRAGMENT_SHADER);
  const char *fragShaderRawSource = fragShaderSrc.c_str();
  glShaderSource(fragShaderId, 1, &fragShaderRawSource, NULL);
  glCompileShader(fragShaderId);

  // Check if the shader compilation succeeded
  glGetShaderiv(vertShaderId, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertShaderId, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  GLuint shaderProgramId = glCreateProgram();
  glAttachShader(shaderProgramId, vertShaderId);
  glAttachShader(shaderProgramId, fragShaderId);
  glLinkProgram(shaderProgramId);
  glDeleteShader(vertShaderId);
  glDeleteShader(fragShaderId);

  glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgramId, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
  }
  glViewport(0, 0, wnd_width, wnd_height);
  SDL_Event event = {0};
  bool should_quit = false;
  while (!should_quit) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        should_quit = true;
        break;
      case SDL_WINDOWEVENT:
        if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
          SDL_GetWindowSize(main_window, &wnd_width, &wnd_height);
          glViewport(0, 0, wnd_width, wnd_height);
        }
        break;
      case SDL_MOUSEBUTTONDOWN:
        GLubyte pixel[4];
        glReadPixels(event.motion.x, wnd_height - event.motion.y, 1, 1, GL_RGBA,
                     GL_UNSIGNED_BYTE, pixel);
        printf("R: %u G: %u B: %u A: %u\n", pixel[0], pixel[1], pixel[2],
               pixel[3]);
        break;
      case SDL_KEYDOWN:

        switch (event.key.keysym.sym) {
        case SDLK_DOWN:
          glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
          break;
        case SDLK_UP:
          glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
          break;
        case SDLK_ESCAPE:
          should_quit = true;
          break;
        }
        break;
      }
    }

    // Clear the window viewport
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glUseProgram(shaderProgramId);
    glBindVertexArray(vao);

    // Draw the triangle !
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Swap the front with the back buffer
    SDL_GL_SwapWindow(main_window);
  }

  // Cleanup
  SDL_GL_DeleteContext(gl_context);
  SDL_DestroyWindow(main_window);
  SDL_Quit();
  return 0;
}

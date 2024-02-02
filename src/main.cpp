#include "SDL_keycode.h"
#include "SDL_timer.h"
#include "SDL_video.h"
#include "shaders/shader.hpp"
#include <SDL.h>
#include <cstddef>
#include <cstdio>
#include <ctime>
#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <stdio.h>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "core/stb_image.h"

#define GP_ERR(exp, msg) assert(((void)msg, exp))

struct Vertex {
  glm::vec3 position;
  glm::vec4 color;
  glm::vec2 texture_coords;

  Vertex(const glm::vec3 &position, const glm::vec4 &color,
         const glm::vec2 &texture_coords)
      : position(position), color(color), texture_coords(texture_coords) {}
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
      Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
             glm::vec2(0.0f, 0.0f)),
      Vertex(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
             glm::vec2(1.0f, 0.0f)),
      Vertex(glm::vec3(0.5f, 0.5f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
             glm::vec2(1.0f, 1.0f)),
      Vertex(glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
             glm::vec2(0.0f, 1.0f)),
  };

  static const unsigned int indices[] = {
      // First triangle
      0, 1, 2,
      // Second triangle
      3, 2, 0
      // End
  };

  stbi_set_flip_vertically_on_load(
      true); // Flip y-axis so that bottom becomes 0.0

  // Load our texture into the GPU
  GLuint textureContainer;
  {
    glGenTextures(1, &textureContainer);
    glBindTexture(GL_TEXTURE_2D, textureContainer);
    int texWidth, texHeight, nrChannels;
    unsigned char *data = stbi_load("./assets/textures/container.jpg",
                                    &texWidth, &texHeight, &nrChannels, 0);

    // (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei
    // height, GLint border, GLenum format, GLenum type, const void *pixels);
    if (data) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB,
                   GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
    } else {
      std::cout << "Failed to load texture"
                << "\n";
    }

    stbi_image_free(data);
  }

  GLuint textureHappy;
  {
    glGenTextures(1, &textureHappy);
    glBindTexture(GL_TEXTURE_2D, textureHappy);
    int texWidth, texHeight, nrChannels;
    unsigned char *data = stbi_load("./assets/textures/awesomeface.png",
                                    &texWidth, &texHeight, &nrChannels, 0);

    // (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei
    // height, GLint border, GLenum format, GLenum type, const void *pixels);
    if (data) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGBA,
                   GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
    } else {
      std::cout << "Failed to load texture"
                << "\n";
    }

    stbi_image_free(data);
  }

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

  glVertexAttribPointer(2, sizeof(glm::vec2) / sizeof(float), GL_FLOAT,
                        GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, texture_coords));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);

  // custom shaders
  auto program = ShaderProgram("./assets/shaders/base_vs.glsl",
                               "./assets/shaders/base_fs.glsl");

  glViewport(0, 0, wnd_width, wnd_height);
  SDL_Event event = {0};
  bool should_quit = false;
  // glm::vec2 mouse = glm::vec2(0.0f, 0.0f);

  program.use();
  program.setUniform("ourContainerTexture", 0);
  program.setUniform("ourHappyFaceTexture", 1);

  // Create our transform matrix
  SDL_GL_SetSwapInterval(0);

  long frames = 0;
  long currentTime = 0;
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

    unsigned int ticks = SDL_GetTicks();

    frames++;
    if ((ticks - currentTime) > 1000) {
      currentTime = ticks;
      std::cout << frames << " fps\n";
      frames = 0;
    }

    // Use our shader program
    program.use();
    program.setUniform("uTime", static_cast<int>(ticks));

    auto trans = glm::mat4(1.0f);
    trans = glm::scale(trans, glm::vec3(0.5f));
    auto anim = glm::radians(ticks / 10.0f);
    trans = glm::translate(trans,
                           glm::vec3(0.0f, glm::sin((anim) + 1.0f) / 2, 0.0f));
    trans = glm::rotate(trans, anim, glm::vec3(0.0f, 0.0f, 1.0f));
    program.setUniform("uTranslate", trans);

    // Clear the window viewport
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Bind our texture
    glActiveTexture(GL_TEXTURE0); // Set the texture unit to 0
    glBindTexture(GL_TEXTURE_2D, textureContainer);

    glActiveTexture(GL_TEXTURE1); // Set the texture unit to 1
    glBindTexture(GL_TEXTURE_2D, textureHappy);

    // Bind our VAO for the vertex data, element data, and vertex attributes
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

#include "SDL_keyboard.h"
#include "SDL_keycode.h"
#include "SDL_mouse.h"
#include "SDL_timer.h"
#include "SDL_video.h"
#include "core/camera/camera.hpp"
#include "shaders/shader.hpp"
#include <SDL.h>
#include <cstddef>
#include <cstdio>
#include <ctime>
#include <glad/glad.h>
#include <glm/common.hpp>
#include <glm/ext/matrix_clip_space.hpp>
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
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "core/stb_image.h"

#define GP_ERR(exp, msg) assert(((void)msg, exp))
using namespace core;

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
  //
  // static const Vertex vertices[] = {
  //     Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec4(0.0f, 1.0f,
  //     0.0f, 1.0f),
  //            glm::vec2(0.0f, 0.0f)),
  //     Vertex(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
  //            glm::vec2(1.0f, 0.0f)),
  //     Vertex(glm::vec3(0.5f, 0.5f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
  //            glm::vec2(1.0f, 1.0f)),
  //     Vertex(glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
  //            glm::vec2(0.0f, 1.0f)),
  // };

  static const Vertex vertices[] = {
      Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
             glm::vec2(0.0f, 0.0f)),
      Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
             glm::vec2(1.0f, 0.0f)),
      Vertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
             glm::vec2(1.0f, 1.0f)),
      Vertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
             glm::vec2(1.0f, 1.0f)),
      Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
             glm::vec2(0.0f, 1.0f)),
      Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
             glm::vec2(0.0f, 0.0f)),

      Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
             glm::vec2(0.0f, 0.0f)),
      Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
             glm::vec2(1.0f, 0.0f)),
      Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
             glm::vec2(1.0f, 1.0f)),
      Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
             glm::vec2(1.0f, 1.0f)),
      Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
             glm::vec2(0.0f, 1.0f)),
      Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
             glm::vec2(0.0f, 0.0f)),

      Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
             glm::vec2(1.0f, 0.0f)),
      Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
             glm::vec2(1.0f, 1.0f)),
      Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
             glm::vec2(0.0f, 1.0f)),
      Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
             glm::vec2(0.0f, 1.0f)),
      Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
             glm::vec2(0.0f, 0.0f)),
      Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
             glm::vec2(1.0f, 0.0f)),

      Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
             glm::vec2(1.0f, 0.0f)),
      Vertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
             glm::vec2(1.0f, 1.0f)),
      Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
             glm::vec2(0.0f, 1.0f)),
      Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
             glm::vec2(0.0f, 1.0f)),
      Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
             glm::vec2(0.0f, 0.0f)),
      Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
             glm::vec2(1.0f, 0.0f)),

      Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
             glm::vec2(0.0f, 1.0f)),
      Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
             glm::vec2(1.0f, 1.0f)),
      Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
             glm::vec2(1.0f, 0.0f)),
      Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
             glm::vec2(1.0f, 0.0f)),
      Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
             glm::vec2(0.0f, 0.0f)),
      Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
             glm::vec2(0.0f, 1.0f)),

      Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
             glm::vec2(0.0f, 1.0f)),
      Vertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
             glm::vec2(1.0f, 1.0f)),
      Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
             glm::vec2(1.0f, 0.0f)),
      Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
             glm::vec2(1.0f, 0.0f)),
      Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
             glm::vec2(0.0f, 0.0f)),
      Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
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

    // (GLenum target, GLint level, GLint internalformat, GLsizei width,
    // GLsizei height, GLint border, GLenum format, GLenum type, const void
    // *pixels);
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

    // (GLenum target, GLint level, GLint internalformat, GLsizei width,
    // GLsizei height, GLint border, GLenum format, GLenum type, const void
    // *pixels);
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

  // GLuint ebo;
  // glGenBuffers(1, &ebo);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
  //              GL_STATIC_DRAW);

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
  glEnable(GL_DEPTH_TEST);
  SDL_Event event = {0};
  bool should_quit = false;

  program.use();
  program.setUniform("ourContainerTexture", 0);
  program.setUniform("ourHappyFaceTexture", 1);

  // Set Vsync off
  SDL_GL_SetSwapInterval(0);

  uint32_t frames = 0;
  uint32_t lastTime = 0;
  uint32_t lastSecond = 0;

  const Uint8 *keystates = SDL_GetKeyboardState(NULL);
  bool focused = true;
  Camera camera = Camera();
  while (!should_quit) {
    uint32_t ticks = SDL_GetTicks();
    float deltaTime = ticks - lastTime;
    lastTime = ticks;
    frames++;
    if ((ticks - lastSecond) > 1000) {
      lastSecond = ticks;
      SDL_SetWindowTitle(
          main_window,
          std::string("Sandbox - " + std::to_string(frames) + " fps").c_str());
      frames = 0;
    }

    if (focused)
      camera.MoveByKeyboard(keystates, deltaTime);
    const auto &cameraTarget = camera.GetTarget();
    const auto &cameraPosition = camera.GetPosition();
    auto view = glm::lookAt(cameraPosition, cameraPosition + cameraTarget,
                            glm::vec3(0.0f, 1.0f, 0.0f));

    if (keystates[SDL_SCANCODE_SPACE]) {
      camera.SetPosition(cameraPosition +
                         glm::vec3(0.0f, 0.005f * deltaTime, 0.0f));
    }

    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        should_quit = true;
        break;
      case SDL_WINDOWEVENT:
        switch (event.window.event) {
        case SDL_WINDOWEVENT_FOCUS_GAINED: {
          SDL_SetWindowGrab(main_window, SDL_TRUE);
          SDL_SetRelativeMouseMode(SDL_TRUE);
          focused = true;
          break;
        }
        case SDL_WINDOWEVENT_FOCUS_LOST: {
          SDL_SetWindowGrab(main_window, SDL_FALSE);
          SDL_SetRelativeMouseMode(SDL_FALSE);
          focused = false;
          break;
        }
        case SDL_WINDOWEVENT_RESIZED: {
          SDL_GetWindowSize(main_window, &wnd_width, &wnd_height);
          glViewport(0, 0, wnd_width, wnd_height);
          break;
        }
        }
        break;
      case SDL_MOUSEMOTION:
        camera.Update(event.motion.xrel, event.motion.yrel);
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
        case SDLK_x:
          glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
          break;
        case SDLK_z:
          glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
          break;
        case SDLK_ESCAPE:
          should_quit = true;
          break;
        }
        break;
      }
    }

    // Use our shader program
    program.use();

    auto projection =
        glm::perspective(glm::radians(45.0f),
                         (float)wnd_width / (float)wnd_height, 0.1f, 100.0f);

    program.setUniform("uView", view);
    program.setUniform("uProjection", projection);
    program.setUniform("uTime", static_cast<int>(ticks));

    // Clear the window viewport
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Bind our texture
    glActiveTexture(GL_TEXTURE0); // Set the texture unit to 0
    glBindTexture(GL_TEXTURE_2D, textureContainer);

    glActiveTexture(GL_TEXTURE1); // Set the texture unit to 1
    glBindTexture(GL_TEXTURE_2D, textureHappy);

    // Bind our VAO for the vertex data, element data, and vertex attributes
    glBindVertexArray(vao);

    for (int x = 0; x < 30; x++) {
      for (int z = 0; z < 30; z++) {
        auto model = glm::translate(glm::mat4(1.0f), glm::vec3(x, 0, -z));
        if ((x + z) % 2 == 0)
          model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(
            model, glm::vec3(0.0f, (sin(ticks / 1000.0f + x)), 0.0f));
        program.setUniform("uModel", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
      }
    }

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

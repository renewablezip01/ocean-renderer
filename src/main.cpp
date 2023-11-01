#include <SDL.h>
#include <glad/glad.h>
#include <iomanip>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  int screen_width = 640, screen_height = 480;
  SDL_Window *main_window = nullptr;
  SDL_GLContext gl_context = nullptr;
  SDL_Event event = {0};
  bool should_quit = false;

  // TRY-CATCH TO FACILITATE COMMON CLEAN-UP CODE
  // INITIALIZE SDL:
  if (SDL_Init(SDL_INIT_EVENTS) < 0) {
    throw(std::string("Failed to initialize SDL: ") + SDL_GetError());
  }

  // CONFIGURE OPENGL ATTRIBUTES USING SDL:
  int context_flags = SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG;
#ifdef _DEBUG
  context_flags |= SDL_GL_CONTEXT_DEBUG_FLAG;
#endif
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, context_flags);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  // CREATE AND SDL WINDOW CONFIGURED FOR OPENGL:
  if (0 == (main_window = SDL_CreateWindow(
                "OpenGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                screen_width, screen_height,
                SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE))) {
    throw(std::string("Failed to create window: ") + SDL_GetError());
  }

  // CREATE THE OPENGL CONTEXT AND MAKE IT CURRENT:
  if (NULL == (gl_context = SDL_GL_CreateContext(main_window))) {
    throw(std::string("Failed to create OpenGL context"));
  } else
    SDL_GL_MakeCurrent(main_window, gl_context);

  // INITIALIZE GLAD:
  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
    throw(std::string("Failed to initialize GLAD"));
  }

  // GAME LOOP:
  static const GLfloat g_vertex_buffer_data[] = {
      -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
  };

  GLuint vertexbuffer;
  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data),
               g_vertex_buffer_data, GL_STATIC_DRAW);
  while (!should_quit) {
    // EVENT LOOP:
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        should_quit = true;
        break;
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
          should_quit = true;
          break;
        }
        break;
      }
    }

    // Clear the screen to BLACK:
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0, // attribute 0. No particular reason for 0, but
                             // must match the layout in the shader.
                          3, // size
                          GL_FLOAT, // type
                          GL_FALSE, // normalized?
                          0,        // stride
                          (void *)0 // array buffer offset
    );

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0,
                 3); // Starting from vertex 0; 3 vertices total -> 1 triangle

    glDisableVertexAttribArray(0);

    // PRESENT BACKBUFFER:
    SDL_GL_SwapWindow(main_window);
  }
  if (gl_context)
    SDL_GL_DeleteContext(gl_context);

  if (main_window)
    SDL_DestroyWindow(main_window);
  SDL_Quit();

  return 0;
}

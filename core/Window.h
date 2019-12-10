#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include <string>

class Window {
 public:
  Window(size_t width, size_t height, const std::string& title = "Pollution");

  ~Window();

  SDL_Window* getWindow();
  SDL_Renderer* getRenderer();
  void render();
  void resize(size_t w, size_t h);

  size_t getW() const;
  size_t getH() const;

 private:
  size_t width_;
  size_t height_;

  SDL_Window* sdl_window_;
  SDL_Renderer* sdl_renderer_;
};
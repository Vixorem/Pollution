#include "main.h"

#include <iostream>

#include "core/Window.h"
#include "models/water_pollution/WaterPollutionModel.h"
#include "widgets/Canvas.h"

bool saveScreenshotBMP(std::string filepath, SDL_Window* SDLWindow,
                       SDL_Renderer* SDLRenderer) {
  SDL_Surface* saveSurface = NULL;
  SDL_Surface* infoSurface = NULL;
  infoSurface = SDL_GetWindowSurface(SDLWindow);
  if (infoSurface == NULL) {
    std::cerr << "Failed to create info surface from window in "
                 "saveScreenshotBMP(string), SDL_GetError() - "
              << SDL_GetError() << "\n";
  } else {
    unsigned char* pixels =
        new (std::nothrow) unsigned char[infoSurface->w * infoSurface->h *
                                         infoSurface->format->BytesPerPixel];
    if (pixels == 0) {
      std::cerr
          << "Unable to allocate memory for screenshot pixel data buffer!\n";
      return false;
    } else {
      if (SDL_RenderReadPixels(
              SDLRenderer, &infoSurface->clip_rect, infoSurface->format->format,
              pixels,
              infoSurface->w * infoSurface->format->BytesPerPixel) != 0) {
        std::cerr << "Failed to read pixel data from SDL_Renderer object. "
                     "SDL_GetError() - "
                  << SDL_GetError() << "\n";
        delete[] pixels;
        return false;
      } else {
        saveSurface = SDL_CreateRGBSurfaceFrom(
            pixels, infoSurface->w, infoSurface->h,
            infoSurface->format->BitsPerPixel,
            infoSurface->w * infoSurface->format->BytesPerPixel,
            infoSurface->format->Rmask, infoSurface->format->Gmask,
            infoSurface->format->Bmask, infoSurface->format->Amask);
        if (saveSurface == NULL) {
          std::cerr << "Couldn't create SDL_Surface from renderer pixel data. "
                       "SDL_GetError() - "
                    << SDL_GetError() << "\n";
          delete[] pixels;
          return false;
        }
        SDL_SaveBMP(saveSurface, filepath.c_str());
        SDL_FreeSurface(saveSurface);
        saveSurface = NULL;
      }
      delete[] pixels;
    }
    SDL_FreeSurface(infoSurface);
    infoSurface = NULL;
  }
  return true;
}

int main(int argc, char* args[]) {
  Window win(800, 400);
  WaterPollutionModel model(win);
  model.initParticles();
  model.draw();
  bool quit = false;
  SDL_Event e;
  bool enter_pushed = false;

  win.render();
  unsigned long long cnt = 0;
  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
        quit = true;
      }
      if (e.key.keysym.sym == SDLK_RETURN) {
        enter_pushed = true;
      }
      if (e.key.keysym.sym == SDLK_BACKSPACE) {
        enter_pushed = false;
        model.initParticles();
        model.draw();
      }
      if (e.key.keysym.sym == SDLK_SPACE) {
        enter_pushed = false;
        model.update();
        model.draw();
      }
    }
    if (enter_pushed) {
      model.update();
      model.draw();
    }
    win.render();
    /*auto s = saveScreenshotBMP("C:\\Users\\Victor\\Desktop\\Screenshots\\" +
                                   std::to_string(cnt) + ".bmp ",
                               win.getWindow(), win.getRenderer());*/
  }
  return 0;
}
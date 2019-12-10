#include "main.h"

#include "core/Window.h"
#include "models/water_pollution/WaterPollutionModel.h"
#include "widgets/Canvas.h"

int main(int argc, char* args[]) {
  Window win(800, 400);
  WaterPollutionModel model(win);
  model.initParticles();
  model.draw();
  bool quit = false;
  SDL_Event e;
  bool enter_pushed = false;

  win.render();
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
  }
  return 0;
}
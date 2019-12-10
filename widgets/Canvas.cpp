#include "Canvas.h"

Canvas::Canvas(const Window& window, int width, int height, int x, int y)
    : window_(window), width_(width), height_(height), x_(x), y_(y) {
  setColor(255, 255, 255);
  cleanUp();
}

void Canvas::setColor(int r, int g, int b, int alpha) {
  SDL_SetRenderDrawColor(window_.getRenderer(), r, g, b, alpha);
}

void Canvas::plot(int x, int y) {
  if (x <= width_ && x >= x_ && y <= height_ && y >= y_) {
    // SDL_RenderDrawPoint(window_.getRenderer(), x, y);
    auto siz = 4;
    auto rect = SDL_Rect{x - siz / 2, y - siz / 2, siz, siz};
    SDL_RenderFillRect(window_.getRenderer(), &rect);
    // SDL_RenderPresent(window_.getRenderer());
  }
}

void Canvas::drawLine(int x1, int y1, int x2, int y2) {
  if (x1 <= width_ && x1 >= x_ && y1 <= height_ && y1 >= y_ && x2 <= width_ &&
      x2 >= x_ && y2 <= height_ && y2 >= y_) {
    SDL_RenderDrawLine(window_.getRenderer(), x1, y1, x2, y2);
    // SDL_RenderPresent(window_.getRenderer());
  }
}

void Canvas::cleanUp() {
  auto rect = SDL_Rect{x_, y_, width_, height_};
  setColor(0xFF, 0xFF, 0xFF);
  SDL_RenderFillRect(window_.getRenderer(), &rect);
}

int Canvas::w() const { return width_; }

int Canvas::h() const { return height_; }

#pragma once
#include <map>

#include "../core/Window.h"

class Canvas {
 public:
  Canvas(const Window& window, int width, int height, int x = 0, int y = 0);

  void setColor(int r, int g, int b, int alpha = 255);
  void plot(int x, int y, int siz = 4);
  void square(int x, int y, int siz);
  void drawLine(int x1, int y1, int x2, int y2);
  void cleanUp();

  int w() const;
  int h() const;

 private:
  int width_;
  int height_;
  int x_{};
  int y_{};
  Window window_;
};

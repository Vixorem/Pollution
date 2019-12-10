#pragma once
#include <map>
#include <random>
#include <vector>

#include "../../widgets/Canvas.h"

using t_phantoms = std::vector<std::pair<double, double>>;

struct Rectangle {
  double x;
  double y;
  double w;
  double h;
};

class WaterPollutionModel {
 public:
  WaterPollutionModel(const Window& win);

  void initParticles();
  void draw();
  void update();

 private:
  void drawParticles();
  void drawGradients();
  std::pair<int, int> toScreen(const Canvas& cnvs, double x, double y);
  void setGradientColors();
  // speed components
  double u(double x, double y);
  double v(double x, double y);
  double c(double y);

  // A piece of vector field we consider
  static constexpr Rectangle rect{0.25, 0.75, 0.5, 0.5};
  static constexpr size_t phantom_plot_size = 30;
  static constexpr size_t dirt_size = 200;
  static constexpr size_t water_size = 200;
  static constexpr double delta = 0.001;
  static constexpr int mesh_siz =
      50;  // it's supposed to be square so we've got the only one field for
            // dimensions
  std::vector<std::pair<double, double>> mesh_nodes;
  std::vector<double> mesh_mean_mass;
  std::random_device rd;
  std::mt19937 gen_{rd()};
  std::vector<t_phantoms> dirt;
  std::vector<t_phantoms> water;
  std::vector<double> dirt_mass_arr;
  std::vector<double> water_mass_arr;
  Canvas particle_canvas;
  Canvas gradient_canvas;
};

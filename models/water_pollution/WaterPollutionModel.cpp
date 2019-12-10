#include "WaterPollutionModel.h"

#include <cmath>

#define D (0.5)

WaterPollutionModel::WaterPollutionModel(const Window& win)
    : particle_canvas(Canvas(win, win.getW() / 2, win.getH())) {}

double WaterPollutionModel::u(double x, double y) {
  return -2 * M_PI * sin(M_PI * y) * cos(2 * M_PI * x);
}

double WaterPollutionModel::v(double x, double y) {
  return M_PI * sin(2 * M_PI * x) * cos(M_PI * y);
}

double WaterPollutionModel::c(double y) { return -tanh((0.5 - y) / D); }

void WaterPollutionModel::initParticles() {
  std::uniform_real_distribution<> ox(rect.x, rect.x + rect.w);
  std::uniform_real_distribution<> water_oy(rect.y, rect.y + rect.h / 2);
  std::uniform_real_distribution<> dirt_oy(rect.y + rect.h / 2,
                                           rect.y + rect.h);

  dirt.clear();
  mesh_nodes.clear();
  water.clear();
  dirt_mass_arr.clear();
  water_mass_arr.clear();

  //for (double i = rect.x; i <= rect.x + rect.w; i += mesh_space) {
  //  for (double j = rect.y; j <= rect.y + rect.h; j += mesh_space) {
  //    mesh_nodes.emplace_back(std::make_pair(i, j));
  //  }
  //}

  for (int i = 0; i < dirt_size; ++i) {
    std::vector<t_phantoms> v;
    t_phantoms ph;
    auto x = ox(gen_);
    auto y = dirt_oy(gen_);
    ph.emplace_back(std::make_pair(x, y));
    dirt.emplace_back(ph);
    dirt_mass_arr.emplace_back(c(y));
  }

  for (int i = 0; i < water_size; ++i) {
    std::vector<t_phantoms> v;
    t_phantoms ph;
    auto x = ox(gen_);
    auto y = water_oy(gen_);
    ph.emplace_back(std::make_pair(x, y));
    water.emplace_back(ph);
    water_mass_arr.emplace_back(c(y));
  }
}

void WaterPollutionModel::drawParticles() {
  particle_canvas.cleanUp();
  particle_canvas.setColor(230, 21, 21);
  for (auto& i : dirt) {
    for (auto& j : i) {
      auto screen_point =
          toScreen(particle_canvas, std::get<0>(j), std::get<1>(j));
      particle_canvas.plot(screen_point.first, screen_point.second);
    }
  }

  particle_canvas.setColor(20, 49, 179);
  for (auto& i : water) {
    for (auto& j : i) {
      auto screen_point =
          toScreen(particle_canvas, std::get<0>(j), std::get<1>(j));
      particle_canvas.plot(screen_point.first, screen_point.second);
    }
  }
}

std::pair<int, int> WaterPollutionModel::toScreen(const Canvas& cnvs, double x,
                                                  double y) {
  auto w = cnvs.w();
  auto h = cnvs.h();

  auto scale_x = w / rect.w;
  auto scale_y = h / rect.h;
  auto canv_height = particle_canvas.h();

  return {x * scale_x - rect.x * scale_x,
          canv_height - y * scale_y + rect.y * scale_y};
}

void WaterPollutionModel::update() {
  for (auto& i : dirt) {
    if (i.size() == phantom_plot_size) {
      i.erase(i.begin());
    }
    auto prev = i.back();
    auto last =
        std::make_pair(prev.first + u(prev.first, prev.second) * delta,
                       prev.second + v(prev.first, prev.second) * delta);
    i.emplace_back(last);
  }
  for (auto& i : water) {
    if (i.size() == phantom_plot_size) {
      i.erase(i.begin());
    }
    auto prev = i.back();
    auto last =
        std::make_pair(prev.first + u(prev.first, prev.second) * delta,
                       prev.second + v(prev.first, prev.second) * delta);
    i.emplace_back(last);
  }
}
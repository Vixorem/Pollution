#include "WaterPollutionModel.h"

#include <cmath>

#define D (0.5)

WaterPollutionModel::WaterPollutionModel(const Window& win)
    : particle_canvas(Canvas(win, win.getW() / 2, win.getH())),
      gradient_canvas(
          Canvas(win, win.getW() / 2, win.getH(), win.getW() / 2, 0)) {}

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
  mesh_nodes.resize(mesh_siz);
  mesh_mean_mass.resize(mesh_siz);
  for (auto& m : mesh_mean_mass) {
    m.resize(mesh_siz);
  }
  for (auto& m : mesh_nodes) {
    m.resize(mesh_siz);
  }
  water.clear();
  dirt_mass_arr.clear();
  water_mass_arr.clear();

  // TODO: set red if they are above the tanh and blue if opposed

  auto step = rect.w / static_cast<double>(mesh_siz);
  for (int i = 0; i < mesh_siz; ++i) {
    for (int j = 0; j < mesh_siz; ++j) {
      mesh_nodes[i][j] =
          std::make_pair(rect.x  + i * step, rect.y  + j * step);
    }
  }

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

void WaterPollutionModel::draw() {
  drawParticles();
  drawGradients();
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

void WaterPollutionModel::drawGradients() {
  auto length = gradient_canvas.w() / mesh_siz;
  for (int i = 0; i < mesh_siz; ++i) {
    for (int j = 0; j < mesh_siz; ++j) {
      auto m = mesh_mean_mass[i][j];
      gradient_canvas.setColor(255 * (100 - m) / 100, 0, 255 * m / 100);
      gradient_canvas.square(i * length, j * length, length);
    }
  }
}

std::pair<int, int> WaterPollutionModel::toScreen(const Canvas& cnvs, double x,
                                                  double y) {
  auto w = cnvs.w();
  auto h = cnvs.h();

  auto scale_x = w / rect.w;
  auto scale_y = h / rect.h;

  return {x * scale_x - rect.x * scale_x, h - y * scale_y + rect.y * scale_y};
}

void WaterPollutionModel::setGradientColors() {
  // Find a distantce to each particle

  auto total_siz = dirt_size + water_size;

  for (int i = 0; i < mesh_siz; ++i) {
    for (int j = 0; j < mesh_siz; ++j) {
      double sum = 0;
      auto node = mesh_nodes[i][j];
      for (int j = 0; j < dirt.size(); ++j) {
        sum += dirt_mass_arr[j] +
               pow(2, ((node.first - dirt[j].back().first) *
                           (node.first - dirt[j].back().first) +
                       (node.second - dirt[j].back().second) *
                           (node.second - dirt[j].back().second)));
      }
      for (int j = 0; j < water.size(); ++j) {
        sum -= water_mass_arr[j] +
               pow(2, ((node.first - water[j].back().first) *
                           (node.first - water[j].back().first) +
                       (node.second - water[j].back().second) *
                           (node.second - water[j].back().second)));
      }
      auto mean_dist = 1000 * sum / total_siz;
      mesh_mean_mass[i][j] = mean_dist;
    }
  }
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

  setGradientColors();
}

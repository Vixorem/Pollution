#include "WaterPollutionModel.h"

#include <cmath>

WaterPollutionModel::WaterPollutionModel(const Window& win)
    : particle_canvas(Canvas(win, win.getW() / 2, win.getH())),
    gradient_canvas(
        Canvas(win, win.getW() / 2, win.getH(), win.getW() / 2, 0)) {}

double WaterPollutionModel::v(double x, double y) {
    return -2 * M_PI * sin(M_PI * y) * cos(2 * M_PI * x);
}

double WaterPollutionModel::u(double x, double y) {
    return M_PI * sin(2 * M_PI * x) * cos(M_PI * y);
}

double WaterPollutionModel::c(double x) { return 0.5 -tanh((x - rect.w / 2) / 12); }



void WaterPollutionModel::initParticles() {
    std::uniform_real_distribution<> ox(rect.x, rect.x + rect.w);
    std::uniform_real_distribution<> oy(rect.y, rect.y + rect.h);

    particles.clear();
    mesh_nodes.resize(mesh_siz);
    mesh_mean_mass.resize(mesh_siz);
    for (auto& m : mesh_mean_mass) {
        m.resize(mesh_siz);
    }
    for (auto& m : mesh_nodes) {
        m.resize(mesh_siz);
    }
    mass_arr.clear();

    auto step = rect.w / static_cast<double>(mesh_siz);
    for (int i = 0; i < mesh_siz; ++i) {
        for (int j = 0; j < mesh_siz; ++j) {
            mesh_nodes[i][j] = std::make_pair(rect.x + i * step, rect.y + j * step);
        }
    }

    for (int i = 0; i < size; ++i) {
        std::vector<t_phantoms> v;
        t_phantoms ph;
        auto x = ox(gen_);
        auto y = oy(gen_);
        ph.emplace_back(std::make_pair(x, y));
        particles.emplace_back(ph);
        auto t = c(x);
        if (t <= y) {
            mass_arr.emplace_back(0);
        }
        else {
            mass_arr.emplace_back(100);
        }
        //mass_arr.emplace_back(t);
    }

}

void WaterPollutionModel::draw() {
    drawParticles();
    if (frame_cnt++ == UPDATE_FRAME) {
        drawGradients();
        frame_cnt = 0;
    }
}

void WaterPollutionModel::drawParticles() {
    particle_canvas.cleanUp();
    for (auto i = 0; i < particles.size(); ++i) {
        for (auto& j : particles[i]) {
            auto screen_point =
                toScreen(particle_canvas, std::get<0>(j), std::get<1>(j));
            auto m = mass_arr[i];
            //TODO: make it independent to m, just to use it as binary(0 or non-0)
            particle_canvas.setColor(255 * (100 - m) / 100, 0, 255 * m / 100);
            particle_canvas.plot(screen_point.first, screen_point.second);
        }
    }
}

void WaterPollutionModel::drawGradients() {
    auto length = gradient_canvas.w() / mesh_siz;
    for (int i = 0; i < mesh_siz; ++i) {
        for (int j = 0; j < mesh_siz; ++j) {
            auto m = mesh_mean_mass[i][j];
            gradient_canvas.setColor(255 * m, 0, 255 * (1 - m));
            gradient_canvas.square(i * length, (mesh_siz - j - 1) * length, length);
        }
    }
}

std::pair<int, int> WaterPollutionModel::toScreen(const Canvas& cnvs, double x,
    double y) {
    auto w = cnvs.w();
    auto h = cnvs.h();

    auto scale_x = w / rect.w;
    auto scale_y = h / rect.h;

    return { x * scale_x - rect.x * scale_x, h - y * scale_y + rect.y * scale_y };
    //return {h - y * scale_y + rect.y * scale_y,x * scale_x - rect.x * scale_x };
}

void WaterPollutionModel::setGradientColors() {
    for (int i = 0; i < mesh_siz; ++i) {
        for (int j = 0; j < mesh_siz; ++j) {
            double colsum1 = 0;
            double colsum2 = 0;
            auto node = mesh_nodes[i][j];
            for (int o = 0; o < particles.size(); ++o) {
                auto dist = ((node.first - particles[o].back().first) *
                    (node.first - particles[o].back().first) +
                    (node.second - particles[o].back().second) *
                    (node.second - particles[o].back().second));

                if (!mass_arr[o]) {
                    colsum1 += 5 / pow(3, 50 * dist);
                }
                else {
                    colsum2 += 5 / pow(3, 50 * dist);
                }

            }
            mesh_mean_mass[i][j] = colsum1 / (colsum1 + colsum2);
        }
    }
}

void WaterPollutionModel::update() {
    for (auto& i : particles) {
        if (i.size() == phantom_plot_size) {
            i.erase(i.begin());
        }
        auto prev = i.back();
        auto last =
            std::make_pair(prev.first + u(prev.first, prev.second) * delta,
                prev.second + v(prev.first, prev.second) * delta);
        i.emplace_back(last);
    }

    if (frame_cnt == UPDATE_FRAME) {
        setGradientColors();
    }
}

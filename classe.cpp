#include "header.hpp"
#include <iostream>
#include <fstream>
#include <cmath>
#include <gnuplot-iostream.h>

Simulation::Simulation(double x0, double y0, double A, double B, double C, double D, double deltat)
    : A(A), B(B), C(C), D(D), deltat(deltat) {
    e2_x = D / C;
    e2_y = A / B;
    x_rel = x0 / e2_x;
    y_rel = y0 / e2_y;
    x_values.push_back(x0);
    y_values.push_back(y0);
    H_values.push_back(calculateH(x0, y0));
    time_values.push_back(0.0);
}

void Simulation::evolve() {
    double new_x_rel = x_rel + (A - B * y_rel * e2_y) * x_rel * deltat;
    double new_y_rel = y_rel + (C * x_rel * e2_x - D) * y_rel * deltat;
    x_rel = new_x_rel > 0 ? new_x_rel : 1e-6;
    y_rel = new_y_rel > 0 ? new_y_rel : 1e-6;
}

void Simulation::runSimulation(double totalTime) {
    int steps = static_cast<int>(totalTime / deltat);
    for (int i = 0; i < steps; ++i) {
        evolve();
        double abs_x = x_rel * e2_x;
        double abs_y = y_rel * e2_y;
        x_values.push_back(abs_x);
        y_values.push_back(abs_y);
        H_values.push_back(calculateH(abs_x, abs_y));
        time_values.push_back((i + 1) * deltat);
    }
}

void Simulation::saveResults(const std::string& filename) const {
    std::ofstream file(filename);
    file << "time,x,y,H\n";
    for (size_t i = 0; i < x_values.size(); ++i) {
        file << time_values[i] << "," << x_values[i] << "," << y_values[i] << "," << H_values[i] << "\n";
    }
}

void Simulation::plotResultsWithGnuplot() const {
    Gnuplot gp;
    gp << "set title 'Prede e predatori in funzione del tempo'\n";
    gp << "set xlabel 'Tempo'\n";
    gp << "set ylabel 'Popolazione'\n";
    gp << "plot '-' using 1:2 with lines notitle, '-' using 1:2 with lines notitle\n";
    gp.send1d(boost::make_tuple(time_values, x_values));
    gp.send1d(boost::make_tuple(time_values, y_values));
}

void Simulation::plotHWithGnuplot() const {
    Gnuplot gp;
    gp << "set title 'H in funzione del tempo'\n";
    gp << "set xlabel 'Tempo'\n";
    gp << "set ylabel 'H'\n";
    gp << "plot '-' using 1:2 with lines title 'H'\n";
    gp.send1d(boost::make_tuple(time_values, H_values));
}

double Simulation::getX() const {
    return x_rel * e2_x;
}

double Simulation::getY() const {
    return y_rel * e2_y;
}

double Simulation::getH() const {
    return calculateH(getX(), getY());
}

double Simulation::getXAtTime(double time) const {
    int index = static_cast<int>(time / deltat);
    if (index >= 0 && index < x_values.size()) {
        return x_values[index];
    }
    else {
        return -1;
    }
}

double Simulation::getYAtTime(double time) const {
    int index = static_cast<int>(time / deltat);
    if (index >= 0 && index < y_values.size()) {
        return y_values[index];
    }
    else {
        return -1;
    }
}

double Simulation::getHAtTime(double time) const {
    int index = static_cast<int>(time / deltat);
    if (index >= 0 && index < H_values.size()) {
        return H_values[index];
    }
    else {
        return -1;
    }
}

double Simulation::calculateH(double x, double y) const {
    return -D * std::log(x) + C * x + B * y - A * std::log(y);
}

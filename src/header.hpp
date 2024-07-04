#ifndef HEADER_HPP
#define HEADER_HPP

#include <vector>
#include <string>

class Simulation {
public:
    Simulation(double x0, double y0, double A, double B, double C, double D, double deltat);

    void runSimulation(double totalTime);
    void saveResults(const std::string& filename) const;
    void plotResultsWithGnuplot() const;
    void plotHWithGnuplot() const;

    double getX() const;
    double getY() const;
    double getH() const;

    double getXAtTime(double time) const;
    double getYAtTime(double time) const;
    double getHAtTime(double time) const;

    const std::vector<double>& getXValues() const { return x_values; }
    const std::vector<double>& getYValues() const { return y_values; }
    const std::vector<double>& getHValues() const { return H_values; }

    double calculateH(double x, double y) const; // Move this to public

private:
    void evolve();

    double x_rel, y_rel, A, B, C, D, deltat;
    double e2_x, e2_y;
    std::vector<double> x_values, y_values, H_values, time_values;
};

#endif // HEADER_HPP

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "header.hpp"

TEST_CASE("Simulation calculates values correctly with 5% tolerance") {
    // Define initial parameters
    double x0 = 1200.0;  // Initial prey population
    double y0 = 1000.0;  // Initial predator population
    double A = 2.0;      // Prey growth rate
    double B = 0.02;     // Prey death rate per interaction with predators
    double C = 0.01;     // Predator growth rate per interaction with prey
    double D = 1.0;      // Natural death rate of predators
    double deltat = 0.001;
    double totalTime = 17.0;  // Total simulation time

    // Create simulation object
    Simulation sim(x0, y0, A, B, C, D, deltat);

    // Run the simulation
    sim.runSimulation(totalTime);

    // Get the calculated values
    const auto& x_values = sim.getXValues();
    const auto& y_values = sim.getYValues();
    const auto& H_values = sim.getHValues();

    // Test some specific values
    REQUIRE(x_values.size() > 0);
    REQUIRE(y_values.size() > 0);
    REQUIRE(H_values.size() > 0);

    // Test initial values
    CHECK(x_values[0] == doctest::Approx(x0));
    CHECK(y_values[0] == doctest::Approx(y0));
    CHECK(H_values[0] == doctest::Approx(sim.calculateH(x0, y0)));

    // Test final values with 5% tolerance
    CHECK(x_values.back() == doctest::Approx(2460).epsilon(0.05)); // Expected final value of x with 5% tolerance
    CHECK(y_values.back() == doctest::Approx(14.9).epsilon(0.05));  // Expected final value of y with 5% tolerance
    CHECK(H_values.back() == doctest::Approx(11.8).epsilon(0.05)); // Expected final value of H with 5% tolerance

    // Additional checks for specific values at index 1
    CHECK(x_values[1] == doctest::Approx(1178.4).epsilon(0.05)); // x at index 1
    CHECK(y_values[1] == doctest::Approx(1011).epsilon(0.05));   // y at index 1
    CHECK(H_values[1] == doctest::Approx(11.0947).epsilon(0.05)); // H at index 1
}

#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

class Simulation {
public:
    Simulation(double x0, double y0, double A, double B, double C, double D, double deltat)
        : A(A), B(B), C(C), D(D), deltat(deltat) {
        e2_x = D / C;
        e2_y = A / B;
        x_rel = x0 / e2_x;  // Initial relative value
        y_rel = y0 / e2_y;  // Initial relative value
        x_values.push_back(x0);  // Initialize the vector with initial absolute values
        y_values.push_back(y0);  // Initialize the vector with initial absolute values
        h_values.push_back(calculateH(x0, y0)); // Initialize the vector with the initial H value
        time_values.push_back(0.0);  // Initialize the vector with the initial time value
    }

    void evolve() {
        double new_x_rel = x_rel + (A - B * y_rel * e2_y) * x_rel * deltat;
        double new_y_rel = y_rel + (C * x_rel * e2_x - D) * y_rel * deltat;
        x_rel = new_x_rel > 0 ? new_x_rel : 1e-6;  // avoid non-positive numbers
        y_rel = new_y_rel > 0 ? new_y_rel : 1e-6;  // avoid non-positive numbers
    }

    void runSimulation(double totalTime) {
        int steps = static_cast<int>(totalTime / deltat);
        for (int i = 1; i <= steps; ++i) {
            evolve();
            double x = x_rel * e2_x;
            double y = y_rel * e2_y;
            double time = i * deltat;
            x_values.push_back(x);  // Convert to absolute value
            y_values.push_back(y);  // Convert to absolute value
            h_values.push_back(calculateH(x, y)); // Calculate and store H
            time_values.push_back(time);  // Store the current time
        }
    }

    void saveResults(const std::string& filename) const {
        std::ofstream file(filename);
        file << "time,x,y,H\n";
        for (size_t i = 0; i < x_values.size(); ++i) {
            file << time_values[i] << "," << x_values[i] << "," << y_values[i] << "," << h_values[i] << "\n";
        }
    }

    double getX() const {
        return x_rel * e2_x;  // Convert to absolute value
    }

    double getY() const {
        return y_rel * e2_y;  // Convert to absolute value
    }

    double getXAtTime(double time) const {
        int index = static_cast<int>(time / deltat);
        if (index >= 0 && index < x_values.size()) {
            return x_values[index];
        }
        else {
            return -1;  // error value
        }
    }

    double getYAtTime(double time) const {
        int index = static_cast<int>(time / deltat);
        if (index >= 0 && index < y_values.size()) {
            return y_values[index];
        }
        else {
            return -1;  // error value
        }
    }

    double getHAtTime(double time) const {
        int index = static_cast<int>(time / deltat);
        if (index >= 0 && index < h_values.size()) {
            return h_values[index];
        }
        else {
            return -1;  // error value
        }
    }

private:
    double x_rel, y_rel, A, B, C, D, deltat;
    double e2_x, e2_y;
    std::vector<double> x_values, y_values, h_values, time_values;

    double calculateH(double x, double y) const {
        return -D * std::log(x) + C * x + B * y - A * std::log(y);
    }
};

int main() {
    double x0 = 1200.0;  // Initial prey population
    double y0 = 1000.0;  // Initial predator population
    double A = 2.0;      // Prey growth rate
    double B = 0.02;     // Prey mortality rate per predator interaction
    double C = 0.01;     // Predator growth rate per prey interaction
    double D = 1.0;      // Natural predator mortality rate
    double deltat = 0.001;
    double totalTime;

    std::string input;
    while (true) {
        std::cout << "Vuoi inserire manualmente i valori di A, B, C, D, x0, y0? (default/no/si): ";
        std::cin >> input;

        if (input == "default" || input == "no") {
            if (input == "no") {
                std::cout << "Usando i valori predefiniti.\n";
            }
            break;
        }
        else if (input == "si") {
            std::cout << "Inserisci il valore di A: ";
            std::cin >> A;
            std::cout << "Inserisci il valore di B: ";
            std::cin >> B;
            std::cout << "Inserisci il valore di C: ";
            std::cin >> C;
            std::cout << "Inserisci il valore di D: ";
            std::cin >> D;
            std::cout << "Inserisci il valore di x0: ";
            std::cin >> x0;
            std::cout << "Inserisci il valore di y0: ";
            std::cin >> y0;
            break;
        }
        else {
            std::cout << "Errore: Inserisci 'default', 'no' o 'si'.\n";
        }
    }

    std::cout << "Inserisci il tempo totale della simulazione: ";
    std::cin >> totalTime;

    Simulation sim(x0, y0, A, B, C, D, deltat);
    sim.runSimulation(totalTime);
    sim.saveResults("results.csv");

    std::cout << "Simulazione completata. Risultati salvati su results.csv" << std::endl;
    std::cout << "Il valore finale di x è: " << sim.getX() << std::endl;
    std::cout << "Il valore finale di y è: " << sim.getY() << std::endl;

    while (true) {
        std::cout << "Dimmi a che istante di tempo vuoi sapere popolazione x, y e H (scrivi 'q' per uscire): ";
        std::cin >> input;
        if (input == "q") {
            break;
        }

        double queryTime;
        try {
            queryTime = std::stod(input);
        }
        catch (const std::invalid_argument&) {
            std::cout << "Errore: Inserisci un numero valido o 'q' per uscire." << std::endl;
            continue;
        }

        double queriedX = sim.getXAtTime(queryTime);
        double queriedY = sim.getYAtTime(queryTime);
        double queriedH = sim.getHAtTime(queryTime);

        if (queriedX >= 0 && queriedY >= 0 && queriedH >= 0) {
            std::cout << "Il valore di x al tempo " << queryTime << " è: " << queriedX << std::endl;
            std::cout << "Il valore di y al tempo " << queryTime << " è: " << queriedY << std::endl;
            std::cout << "Il valore di H al tempo " << queryTime << " è: " << queriedH << std::endl;
        }
        else {
            std::cout << "Errore: il tempo specificato è fuori dal range della simulazione." << std::endl;
        }
    }

    std::cout << "Programma terminato." << std::endl;
    return 0;
}

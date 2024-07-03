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
        x_rel = x0 / e2_x;  // Valore relativo iniziale
        y_rel = y0 / e2_y;  // Valore relativo iniziale
        x_values.push_back(x0);  // Inizializza il vettore con i valori iniziali assoluti
        y_values.push_back(y0);  // Inizializza il vettore con i valori iniziali assoluti
    }

    void evolve() {
        double new_x_rel = x_rel + (A - B * y_rel * e2_y) * x_rel * deltat;
        double new_y_rel = y_rel + (C * x_rel * e2_x - D) * y_rel * deltat;
        x_rel = new_x_rel > 0 ? new_x_rel : 1e-6;  // evitare numeri non positivi
        y_rel = new_y_rel > 0 ? new_y_rel : 1e-6;  // evitare numeri non positivi
    }

    void runSimulation(double totalTime) {
        int steps = static_cast<int>(totalTime / deltat);
        for (int i = 0; i < steps; ++i) {
            evolve();
            x_values.push_back(x_rel * e2_x);  // Convertire al valore assoluto
            y_values.push_back(y_rel * e2_y);  // Convertire al valore assoluto
        }
    }

    void saveResults(const std::string& filename) const {
        std::ofstream file(filename);
        file << "x,y\n";
        for (size_t i = 0; i < x_values.size(); ++i) {
            file << x_values[i] << "," << y_values[i] << "\n";
        }
    }

    double getX() const {
        return x_rel * e2_x;  // Convertire al valore assoluto
    }

    double getY() const {
        return y_rel * e2_y;  // Convertire al valore assoluto
    }

    double getXAtTime(double time) const {
        int index = static_cast<int>(time / deltat);
        if (index >= 0 && index < x_values.size()) {
            return x_values[index];
        }
        else {
            return -1;  // valore di errore
        }
    }

    double getYAtTime(double time) const {
        int index = static_cast<int>(time / deltat);
        if (index >= 0 && index < y_values.size()) {
            return y_values[index];
        }
        else {
            return -1;  // valore di errore
        }
    }

private:
    double x_rel, y_rel, A, B, C, D, deltat;
    double e2_x, e2_y;
    std::vector<double> x_values, y_values;
};

int main() {
    double x0 = 1200.0;  // Popolazione iniziale delle prede
    double y0 = 1000.0;  // Popolazione iniziale dei predatori
    double A = 2.0;      // Tasso di crescita delle prede
    double B = 0.02;     // Tasso di mortalità delle prede per interazione con i predatori
    double C = 0.01;     // Tasso di crescita dei predatori per interazione con le prede
    double D = 1.0;      // Tasso di mortalità naturale dei predatori
    double deltat = 0.001;
    double totalTime;

    std::string input;
    while (true) {
        std::cout << "Vuoi inserire manualmente i valori di A, B, C, D, x0, y0? (default/no/sì): ";
        std::cin >> input;

        if (input == "default" || input == "no") {
            if (input == "no") {
                std::cout << "Usando i valori predefiniti.\n";
            }
            break;
        }
        else if (input == "sì") {
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
            std::cout << "Errore: Inserisci 'default', 'no' o 'sì'.\n";
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
        std::cout << "Dimmi a che istante di tempo vuoi sapere popolazione x e y (scrivi 'q' per uscire): ";
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

        if (queriedX >= 0 && queriedY >= 0) {
            std::cout << "Il valore di x al tempo " << queryTime << " è: " << queriedX << std::endl;
            std::cout << "Il valore di y al tempo " << queryTime << " è: " << queriedY << std::endl;
        }
        else {
            std::cout << "Errore: il tempo specificato è fuori dal range della simulazione." << std::endl;
        }
    }

    std::cout << "Programma terminato." << std::endl;
    return 0;
}

#include "header.hpp"
#include <iostream>
#include <string>

int main() {
    double x0 = 1200.0;
    double y0 = 1000.0;
    double A = 2.0;
    double B = 0.02;
    double C = 0.01;
    double D = 1.0;
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
    sim.plotResultsWithGnuplot();
    sim.plotHWithGnuplot();

    std::cout << "Simulazione completata. Risultati salvati su results.csv e plot.png" << std::endl;
    std::cout << "Il valore finale di x è: " << sim.getX() << std::endl;
    std::cout << "Il valore finale di y è: " << sim.getY() << std::endl;
    std::cout << "Il valore finale di H è: " << sim.getH() << std::endl;

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

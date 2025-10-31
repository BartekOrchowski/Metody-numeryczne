#include "fitting.h"
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
struct DataPoint {
    double V;
    double I;
};

static std::vector<DataPoint> readCSV(const std::string& filename) {
    std::vector<DataPoint> data;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Nie można otworzyć pliku: " << filename << std::endl;
        return data;
    }

    std::string line;
    std::getline(file, line); // pomijamy nagłówek

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string v_str, i_str;
        if (std::getline(ss, v_str, ',') && std::getline(ss, i_str, ',')) {
            DataPoint dp;
            dp.V = std::stod(v_str);
            dp.I = std::stod(i_str);
            data.push_back(dp);
        }
    }
    return data;
}

// Stałe fizyczne
constexpr double kTq = 0.02585; // V_T ~ 25.85 mV dla 300K

// Funkcja modelu Shockleya
static double modelShockley(double V, double Is, double n) {
    return Is * (std::exp(V / (n * kTq)) - 1.0);
}

// Proste dopasowanie metodą najmniejszych kwadratów (próba wielu kombinacji)
FittingResult fitShockleyModel(const std::string& csvPath) {
    auto data = readCSV(csvPath);
    if (data.empty()) {
        return {0, 0, false};
    }

    double bestIs = 0;
    double bestN = 0;
    double minError = 1e9;

    for (double Is = 1e-9; Is < 1e-2; Is *= 1.5) {
        for (double n = 0.5; n < 3.0; n += 0.05) {
            double error = 0;
            for (auto& p : data) {
                double Ipred = modelShockley(p.V, Is, n);
                error += std::pow(p.I - Ipred, 2);
            }
            if (error < minError) {
                minError = error;
                bestIs = Is;
                bestN = n;
            }
        }
    }

    FittingResult result;
    result.Is = bestIs;
    result.n = bestN;
    result.success = true;
    return result;
}

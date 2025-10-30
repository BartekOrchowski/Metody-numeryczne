#pragma once
#include <string>

struct FittingResult {
    double Is;
    double n;
    bool success;
};

FittingResult fitShockleyModel(const std::string& csvPath);

#pragma once

#include <random>

float randomFloat(float min, float max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_real_distribution<float> dis(min, max);

    return dis(gen);
}

int randomInt(int min, int max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_int_distribution<int> dis(min, max);

    return dis(gen);
}

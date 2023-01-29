#ifndef EXTRA_HPP
#define EXTRA_HPP

#include <string>
#include <algorithm>
#include <random>

using namespace std;

// Removes the quotation marks from the string and converts it to uppercase
string formatString(string s)
{
    transform(s.begin(), s.end(), s.begin(), ::toupper);
    s.erase(remove(s.begin(), s.end(), '\"'), s.end());
    return s;
}

// Generate a random fluctuation value between -10% and 10% of the market price
float generateFluctuation(float marketPrice)
{
    // Generate a random fluctuation value between -10% and 10% of the market price
    random_device rd;                           // Seed the generator
    mt19937 gen(rd());                          // Choose a generator
    uniform_real_distribution<> dis(-0.1, 0.1); // Generate random numbers between -0.1 and 0.1
    float fluctuation = dis(gen) * marketPrice; // Generate a random fluctuation value
    return fluctuation;
}

#endif
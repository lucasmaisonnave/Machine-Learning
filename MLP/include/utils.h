#pragma once

// Set up the random number generator
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>
std::random_device rd;
std::mt19937 gen(rd());
using namespace boost::numeric;

void Normal_Random(ublas::vector<double> &v){
    std::normal_distribution<> uni(0, 1);
    for(auto &i : v)
        i = uni(gen);
}

void Normal_Random(ublas::matrix<double> &v){
    std::normal_distribution<> uni(0, 1);
    for(auto &i : v.data())
        i = uni(gen);
}

ublas::vector<double> sigmoid(const ublas::vector<double> &v){
    ublas::vector<double> z(v.size());
    for(size_t i = 0; i < v.size(); i++)
        z[i] = 1.0/(1.0 + std::exp(-v[i]));
    return z;
}

ublas::vector<double> sigmoid_prime(const ublas::vector<double> &v){
    ublas::vector<double> z(v.size());
    ublas::vector<double> sig = sigmoid(v);

    for(size_t i = 0; i < v.size(); i++)
        z[i] = sig[i] * (1 - sig[i]);
    return z;
}

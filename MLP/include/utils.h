#pragma once

// Set up the random number generator
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/math/special_functions/expm1.hpp>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>
std::random_device rd;
std::mt19937 gen(rd());
using namespace boost::numeric;

void Normal_Random(ublas::vector<double> &v, double stddev = 1.0){
    std::normal_distribution<> uni(0, stddev);
    for(auto &i : v)
        i = uni(gen);
}

void Normal_Random(ublas::matrix<double> &v, double stddev = 1.0){
    std::normal_distribution<> uni(0, stddev);
    for(auto &i : v.data())
        i = uni(gen);
}


class Activation{
private:

public:
    Activation(){

    }
    virtual ~Activation(){

    }
    virtual ublas::vector<double> f(const ublas::vector<double> &v) = 0;
    virtual ublas::vector<double> f_prime(const ublas::vector<double> &v) = 0;
};

class Sigmoid : public Activation{
private:

public:
    Sigmoid(){

    }
    virtual ublas::vector<double> f(const ublas::vector<double> &v) override {
        ublas::vector<double> z(v.size());
        for(size_t i = 0; i < v.size(); i++)
            z[i] = 1.0/(2.0 + boost::math::expm1(-v[i]));
        return z;
    }
    virtual ublas::vector<double> f_prime(const ublas::vector<double> &v) override {
        ublas::vector<double> z(v.size());
        ublas::vector<double> sig = f(v);
        for(size_t i = 0; i < v.size(); i++)
            z[i] = sig[i] * (1 - sig[i]);
        return z;
    }
};

class Softmax : public Activation{
private:

public:
    Softmax(){

    }
    virtual ublas::vector<double> f(const ublas::vector<double> &v) override {
        ublas::vector<double> z(v.size());
        double den = 0;
        for(size_t i = 0; i < v.size(); i++)
            den += boost::math::expm1(v[i]) + 1; 
        for(size_t i = 0; i < v.size(); i++)
            z[i] = (boost::math::expm1(v[i]) + 1) / den;
        return z;
    }
    virtual ublas::vector<double> f_prime(const ublas::vector<double> &v) override {
        ublas::vector<double> soft = f(v);
        ublas::vector<double> z(v.size());
        for(size_t i = 0; i < v.size(); i++)
            z[i] = soft[i] * (1 - soft[i]);
        return z;
    }
};
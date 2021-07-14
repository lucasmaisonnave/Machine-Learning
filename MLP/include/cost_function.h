#pragma once

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/math/special_functions/log1p.hpp>
#include "utils.h"
using namespace boost::numeric;

class cost_function
{
private:
    /* data */
public:
    cost_function(/* args */){
        
    }
    virtual double f(const ublas::vector<double>& a, const ublas::vector<double>& y);
    virtual ublas::vector<double> delta(const ublas::vector<double>& z, const ublas::vector<double>& a, const ublas::vector<double>& y);
};


class Quadratic : public cost_function
{
private:

public:
    Quadratic() : cost_function(){

    }
    virtual double f(const ublas::vector<double>& a, const ublas::vector<double>& y) override {
        auto delta = a - y;
        return std::pow(ublas::norm_1(delta),2) / 2.0f;
    }

    virtual ublas::vector<double> delta(const ublas::vector<double>& z, const ublas::vector<double>& a, const ublas::vector<double>& y) override{
        return boost::numeric::ublas::element_prod(a - y, sigmoid_prime(z));
    }
};


class CrossEntropy : public cost_function
{
private:

public:
    CrossEntropy() : cost_function(){
        
    }
    virtual double f(const ublas::vector<double>& a, const ublas::vector<double>& y) override {
        boost::numeric::ublas::vector<double> vec_1(y.size(), 1);
        auto vec = boost::numeric::ublas::element_prod(y, boost::math::log1p(a)) + boost::numeric::ublas::element_prod(vec_1 - y, boost::math::log1p(vec_1 - a));        
        return boost::numeric::ublas::sum(vec);
    }

    virtual ublas::vector<double> delta(const ublas::vector<double>& z, const ublas::vector<double>& a, const ublas::vector<double>& y) override{
        return a - y;
    }
};

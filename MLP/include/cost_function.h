#pragma once

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/math/special_functions/log1p.hpp>
#include <boost/range/algorithm.hpp>
#include "utils.h"
using namespace boost::numeric;

class cost_function
{
protected:
    Activation* act = nullptr;
public:
    cost_function(/* args */){
        act = new Softmax();
    }
    virtual ~cost_function(/* args */){
        if(act)
            delete act;
    }
    virtual double f(const ublas::vector<double>& a, const ublas::vector<double>& y) = 0;
    virtual ublas::vector<double> delta(const ublas::vector<double>& z, const ublas::vector<double>& a, const ublas::vector<double>& y) = 0;
    void set_activation(Activation* _act){
        if(act)
            delete act;
        act = _act;
    }
};


class Quadratic : public cost_function
{
private:
    

public:
    Quadratic() : cost_function(){
        
    }
    ~Quadratic(){
        
    }
    virtual double f(const ublas::vector<double>& a, const ublas::vector<double>& y) override {
        auto delta = a - y;
        return std::pow(ublas::norm_1(delta),2) / 2.0f;
    }

    virtual ublas::vector<double> delta(const ublas::vector<double>& z, const ublas::vector<double>& a, const ublas::vector<double>& y) override{
        return boost::numeric::ublas::element_prod(a - y, act->f_prime(z));
    }
};


class CrossEntropy : public cost_function
{
private:

public:
    CrossEntropy() : cost_function(){
        
    }
    ~CrossEntropy(){

    }
    virtual double f(const ublas::vector<double>& a, const ublas::vector<double>& y) override {
        double sum = 0;
        for(size_t i = 0; i < a.size(); i++)
            sum -= y[i] * boost::math::log1p(a[i] - 1) + (1 - y[i]) * boost::math::log1p(-a[i]);
        return sum;
    }

    virtual ublas::vector<double> delta(const ublas::vector<double>& z, const ublas::vector<double>& a, const ublas::vector<double>& y) override{
        return a - y;
    }
};

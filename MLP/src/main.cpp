//#include "stdafx.h"

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include "../include/mnist_loader.h"
#include "../include/utils.h"
#include "../include/cost_function.h"
#include <cmath>
#include <iostream>
#include <random>
#include <vector>

using namespace boost::numeric;


class Network {
private:
	std::vector<int> m_sizes;
    int L;
	using BiasesVector = std::vector<ublas::vector<double>>;
	using WeightsVector = std::vector<ublas::matrix<double>>;
	BiasesVector biases;
	WeightsVector weights;
    cost_function* C = nullptr;
    std::vector<Activation*> fct_act;
    int n; //Training set size

    //Velocity used for MGD
    WeightsVector v_w;
    BiasesVector v_b;

public:
	
	// Type definition of the Training data
	using TrainingData = std::pair<ublas::vector<double>, ublas::vector<double>>;
	using TrainingDataIterator = typename std::vector<TrainingData>::iterator;
	
	Network(std::vector<int> sizes, std::vector<Activation*> _fct_act, cost_function* _C) : fct_act(_fct_act), m_sizes(sizes), L(sizes.size()), C(_C){
        for(int i = 1; i < L; i++){
            ublas::vector<double> b(m_sizes[i]);
            Normal_Random(b, 1.0 / std::sqrt(m_sizes[i - 1]));
            biases.push_back(b);
            ublas::matrix<double> w(m_sizes[i], m_sizes[i - 1]);
            Normal_Random(w, 1.0 / std::sqrt(m_sizes[i - 1]));
            weights.push_back(w);
        }
        C->set_activation(fct_act[fct_act.size() - 1]);
        PopulateZeroWeightsAndBiases(v_b, v_w);
    }

    ~Network(){
        if(C)
            delete C;
        for(auto fct : fct_act)
            if(fct)
                delete fct;
    }

    ublas::vector<double> feedforward(ublas::vector<double> a){
        for(int i = 0; i < L - 1; i++)
            a = fct_act[i]->f(prod(weights[i], a) + biases[i]);
        return a;
    }
    //Stochastic Gradient Descent
    void SGD(std::vector<TrainingData> training_data, int epochs, int mini_batch_size, double eta, double lambda,
		std::vector<TrainingData> test_data){
            n = training_data.size();
            for(int i = 0; i < epochs; i++){
                std::shuffle(training_data.begin(), training_data.end(), rd);
                for(TrainingDataIterator k = training_data.begin(); k < training_data.end(); k += mini_batch_size){
                    update_mini_batch_SGD(k, mini_batch_size, eta, lambda);
                    std::cout << (1 - (training_data.end() - k) / (double) n)* 100 << '\r' << std::flush;
                }
                if(test_data.size() != 0)
                    std::cout << "Epoch : " << i + 1 << "/" << epochs << " Test data : " << evaluate(test_data) << "/" << test_data.size() << std::endl;
            }
    }
    //Momentum-based Gradient Descent
    void MGD(std::vector<TrainingData> training_data, int epochs, int mini_batch_size, double eta, double lambda, double mu,
		std::vector<TrainingData> test_data){
            n = training_data.size();
            for(int i = 0; i < epochs; i++){
                std::shuffle(training_data.begin(), training_data.end(), rd);
                for(TrainingDataIterator k = training_data.begin(); k < training_data.end(); k += mini_batch_size){
                    update_mini_batch_MGD(k, mini_batch_size, eta, lambda, mu);
                    std::cout << (1 - (training_data.end() - k) / (double) n)* 100 << '\r' << std::flush;
                }
                if(test_data.size() != 0)
                    std::cout << "Epoch : " << i + 1 << "/" << epochs << " Test data : " << evaluate(test_data) << "/" << test_data.size() << std::endl;
            }
    }

    void PopulateZeroWeightsAndBiases(BiasesVector &b, WeightsVector &w) const {
		for (size_t i = 1; i < m_sizes.size(); ++i) {
			b.push_back(ublas::zero_vector<double>(m_sizes[i]));
			w.push_back(ublas::zero_matrix<double>(m_sizes[i], m_sizes[i - 1]));
		}
	}

    void update_mini_batch_SGD(TrainingDataIterator it, int mini_batch_size, double eta, double lambda = 0){
        WeightsVector nabla_w;
        BiasesVector nabla_b;
        PopulateZeroWeightsAndBiases(nabla_b, nabla_w);
        for(auto i = 0; i < mini_batch_size; ++i, it++){
            auto &x = it->first;  // test data
			auto &y = it->second; // expected result
			BiasesVector delta_nabla_b;
			WeightsVector delta_nabla_w;
			PopulateZeroWeightsAndBiases(delta_nabla_b, delta_nabla_w);
			backwardprop(x, y, delta_nabla_w, delta_nabla_b);
			for (size_t k = 0; k < biases.size(); ++k) {
				nabla_b[k] += delta_nabla_b[k];
				nabla_w[k] += delta_nabla_w[k];
			}
        }
        for (size_t k = 0; k < biases.size(); ++k) {
            weights[k] = (1 - eta * lambda / (double) n) * weights[k] - eta / mini_batch_size * nabla_w[k];
            biases[k] -= eta / mini_batch_size * nabla_b[k];
        }

    }

    void update_mini_batch_MGD(TrainingDataIterator it, int mini_batch_size, double eta, double lambda = 0, double mu = 0.5){
        WeightsVector nabla_w;
        BiasesVector nabla_b;
        PopulateZeroWeightsAndBiases(nabla_b, nabla_w);
        for(auto i = 0; i < mini_batch_size; ++i, it++){
            auto &x = it->first;  // test data
			auto &y = it->second; // expected result
			BiasesVector delta_nabla_b;
			WeightsVector delta_nabla_w;
			PopulateZeroWeightsAndBiases(delta_nabla_b, delta_nabla_w);
			backwardprop(x, y, delta_nabla_w, delta_nabla_b);
			for (size_t k = 0; k < biases.size(); ++k) {
				nabla_b[k] += delta_nabla_b[k];
				nabla_w[k] += delta_nabla_w[k];
			}
        }
        for (size_t k = 0; k < biases.size(); ++k) {
            v_w[k] = mu * v_w[k] - eta / mini_batch_size *  nabla_w[k];
            v_b[k] = mu * v_b[k] - eta / mini_batch_size *  nabla_b[k];
            weights[k] = (1 - eta * lambda / (double) n) * weights[k] + v_w[k];
            biases[k] += v_b[k];
        }

    }

    void backwardprop(const ublas::vector<double>& x, const ublas::vector<double>& y, WeightsVector &nabla_w, BiasesVector &nabla_b){
        
        std::vector<ublas::vector<double>> activations;
        std::vector<ublas::vector<double>> zs;
        auto activation = x;
        activations.push_back(activation);
        ublas::vector<double> z;

        for(size_t i = 0; i < biases.size(); i++){
            z = prod(weights[i], activations[i]) + biases[i];
            zs.push_back(z);
            activation = fct_act[i]->f(z);
            activations.push_back(activation);
        }
        ublas::vector<double> delta_l = C->delta(zs[biases.size() - 1], activations[L - 1], y);
        nabla_b[L - 2] = delta_l;
        nabla_w[L - 2] = boost::numeric::ublas::outer_prod(delta_l, activations[L - 2]);
        
        for(int l = L - 1; l >= 2; l--){
            auto fct_prime = fct_act[l - 2]->f_prime(zs[l - 2]);
            ublas::vector<double> tr = prod( boost::numeric::ublas::trans(weights[l - 1]), delta_l);
            delta_l = boost::numeric::ublas::element_prod(tr, fct_prime);
            nabla_b[l - 2] = delta_l;
            nabla_w[l - 2] = boost::numeric::ublas::outer_prod(delta_l, activations[l - 2]);
        }
        
    }
    
    int evaluate(std::vector<TrainingData>& test_data){
        int cmpt = 0;
        for(auto d : test_data){
            auto labels = feedforward(d.first);
            int label_max = std::max_element(labels.begin(),labels.end()) - labels.begin();
            int y_max = std::max_element(d.second.begin(),d.second.end()) - d.second.begin();
            if(label_max == y_max)
                cmpt++;
        }
        return cmpt;
    }
};

int main() {
	std::vector<Network::TrainingData> td, testData;
	// Load training data
	try {
		mnist_loader<double> loader("./data/train-images.idx3-ubyte", "./data/train-labels.idx1-ubyte", td);
		// Load test data
		mnist_loader<double> loader2("./data/t10k-images.idx3-ubyte", "./data/t10k-labels.idx1-ubyte", testData);
	}
	catch (const char *ex) {
		std::cout << "Error: " << ex << std::endl;
		return 0;
	}
    auto cost = new CrossEntropy();
    Sigmoid* sigmoid = new Sigmoid();
    Softmax* softmax = new Softmax();
	Network net({ 784, 30, 10 }, {sigmoid, softmax}, cost);
	net.MGD(td, 10, 10, 0.1, 5.0, 0.5, testData);

	return 0;
}
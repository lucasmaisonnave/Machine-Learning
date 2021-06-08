#pragma once
#include <tensorflow/c/c_api.h>
#include "../include/TFUtils.h"
#include "../include/Chess.h"
#include "../include/DataCSV.h"

using namespace std;
using namespace DataCSV;

class TFModel{
private:
    TFUtils TFU;
    TFUtils::STATUS status;
    const vector<int64_t> input_dims = {1, 8, 8, 12};
    vector<TF_Output> input_ops;
    vector<TF_Tensor*> input_tensors;

    vector<TF_Output> output_ops;
    vector<TF_Tensor*> output_tensors = {nullptr};
public:
    TFModel(){

    }
    ~TFModel(){

    }
    TFUtils::STATUS Load(string directory, string input_op, string output_op){
        TFUtils::STATUS stat = TFU.LoadModel_TFv2(directory);
        input_ops = {TFU.GetOperationByName(input_op, 0)};
        output_ops = {TFU.GetOperationByName(output_op, 0)};
        return stat;
    }
    double Compute_heuristic(const Chess& chess){
        vector<float> input_vals = chess.get_plat_vec();
        input_tensors = {TFUtils::CreateTensor<float>(TF_FLOAT, input_dims, input_vals)};
        TFU.RunSession(input_ops, input_tensors, output_ops, output_tensors);
        const std::vector<std::vector<float>> data = TFUtils::GetTensorsData<float>(output_tensors);
        TFUtils::DeleteTensors(input_tensors);
        TFUtils::DeleteTensors(output_tensors);
        return data[0][0];
    }
};
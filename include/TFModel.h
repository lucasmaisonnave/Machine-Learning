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
    const vector<TF_Output> input_ops = {TFU.GetOperationByName("serving_default_conv2d_input", 0)};
    vector<TF_Tensor*> input_tensors;

    const vector<TF_Output> output_ops = {TFU.GetOperationByName("StatefulPartitionedCall", 0)};
    vector<TF_Tensor*> output_tensors = {nullptr};
public:
    TFModel(){

    }
    ~TFModel(){

    }
    TFUtils::STATUS Load(string directory){
        return TFU.LoadModel_TFv2(directory);
    }
    double Compute_heuristic(const Chess& chess){
        vector<int64_t> input_vals(8*8*12);
        for(int c = 0; c < CHESS_SIZE; c++)
            for(int l = 0; l < CHESS_SIZE; l++){
            vector<int64_t> new_vect = ConvertPieceToVectInt(chess.getCase(c,l).type, chess.getCase(c,l).couleur);
            for(int i = 0; i < 12; i++)
                input_vals[c*CHESS_SIZE*12 + l*12 + i] = new_vect[i];
            }
        input_tensors = {TFUtils::CreateTensor(TF_FLOAT, input_dims, input_vals)};
        TFU.RunSession(input_ops, input_tensors, output_ops, output_tensors);
        const std::vector<std::vector<float>> data = TFUtils::GetTensorsData<float>(output_tensors);
        TFUtils::DeleteTensors(input_tensors);
        TFUtils::DeleteTensors(output_tensors);
        return data[0][0];
    }
};
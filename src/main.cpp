#include <iostream>
#include <string>
#include <math.h>
#include <map>
#include <stdlib.h>
#include <stdio.h>
#include <tensorflow/c/c_api.h>

#include "../include/Chess.h"
#include "../include/DataCSV.h"
#include "../include/ChessGame.h"
#include "../include/TFUtils.h"

using namespace olc;
using namespace DataCSV;
using namespace std;
//using namespace mlpack::neighbor; // NeighborSearch and NearestNeighborSort
//using namespace mlpack::metric; // ManhattanDistance

#define FILE_NAME "../data/DataGames.csv"



//void NoOpDeallocator(void* data, size_t a, void* b) {}

int main(int argc, char* argv[])
{
  //Ranking rate
	//CreateDataset(atoi(argv[1]));
  /*AIGame game;
	game.Construct(1080, 720, 1, 1);
  game.Start();*/
  // TFUtils init
  TFUtils TFU;
  Chess chess;
  //TFUtils::STATUS status = TFU.LoadModel("./models/model.h5");
  TFUtils::STATUS status = TFU.LoadModel_TFv2("./models/model_h1");

  const vector<int64_t> input_dims = {1, 8, 8, 12};
  vector<int64_t> input_vals(8*8*12);
  for(int c = 0; c < CHESS_SIZE; c++)
    for(int l = 0; l < CHESS_SIZE; l++){
      vector<int64_t> new_vect = ConvertPieceToVectInt(chess.getCase(c,l).type, chess.getCase(c,l).couleur);
      for(int i = 0; i < 12; i++)
        input_vals[c*CHESS_SIZE*12 + l*12 + i] = new_vect[i];
    }
  //Input Tensor Create
  const vector<TF_Output> input_ops = {TFU.GetOperationByName("serving_default_conv2d_input", 0)};
  const vector<TF_Tensor*> input_tensors = {TFUtils::CreateTensor(TF_FLOAT, input_dims, input_vals)};

  // Output Tensor Create
  const vector<TF_Output> output_ops = {TFU.GetOperationByName("StatefulPartitionedCall", 0)};
  vector<TF_Tensor*> output_tensors = {nullptr};
 
  TFU.RunSession(input_ops, input_tensors, output_ops, output_tensors);

  TFUtils::PrinStatus(status);

  if (status == TFUtils::SUCCESS) {
    const std::vector<std::vector<float>> data = TFUtils::GetTensorsData<float>(output_tensors);
    const std::vector<float> result = data[0];
    std::cout << "Output value: " << result[0] << std::endl;
  } 
  else {
    std::cout << "Error run session";
    return 2;
  }

    TFUtils::DeleteTensors(input_tensors);
    TFUtils::DeleteTensors(output_tensors);

	return 0;
}

//saved_model_cli show --dir ~/Desktop/Machine-Learning/mlpack/models/model_h1 --tag_set serve --signature_def serving_default

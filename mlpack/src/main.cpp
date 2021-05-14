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
  TFUtils::STATUS status = TFU.LoadModel("./models/model_h1/saved_model.pb");

	return 0;
}

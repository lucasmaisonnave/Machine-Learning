#include <iostream>
#include <string>
#include <math.h>
#include <map>
#include <stdlib.h>
#include <stdio.h>


#include "../include/Chess.h"
#include "../include/DataCSV.h"
#include "../include/ChessGame.h"
#include "../include/TFUtils.h"
#include "../include/TFModel.h"
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
  AIGame game;
	game.Construct(1080, 720, 1, 1);
  game.Start();
	return 0;
}

//saved_model_cli show --dir ~/Desktop/Machine-Learning/mlpack/models/model_h1 --tag_set serve --signature_def serving_default

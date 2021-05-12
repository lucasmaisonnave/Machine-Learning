#include <iostream>
#include <string>
#include <math.h>
#include <map>
#include <stdlib.h>
#include <stdio.h>
#include <tensorflow/c/c_api.h>
/*
#include <tensorflow/cc/client/client_session.h>
#include <tensorflow/cc/ops/standard_ops.h>
#include <tensorflow/cc/framework/gradients.h>
#include <tensorflow/core/framework/tensor.h>
#include <tensorflow/core/public/session.h>
#include <tensorflow/core/lib/io/path.h>
//#include <tensorflow/core/summary/summary_file_writer.h>
#include <tensorflow/cc/tools/freeze_saved_model.h>
//#include <tensorflow/contrib/image/image_ops.h>
*/

#include "./include/Chess.h"
#include "./include/DataCSV.h"
#include "./include/ChessGame.h"

using namespace olc;
using namespace DataCSV;
using namespace std;
//using namespace mlpack::neighbor; // NeighborSearch and NearestNeighborSort
//using namespace mlpack::metric; // ManhattanDistance

#define FILE_NAME "./data/DataGames.csv"



void NoOpDeallocator(void* data, size_t a, void* b) {}

int main(int argc, char* argv[])
{
  //Ranking rate
	CreateDataset(atoi(argv[1]));
  /*AIGame game;
	game.Construct(1080, 720, 1, 1);
  game.Start();*/
  //********* Read model
    TF_Graph* Graph = TF_NewGraph();
    TF_Status* status = TF_NewStatus();
    TF_SessionOptions* SessionOpts = TF_NewSessionOptions();
    TF_Buffer* RunOpts = NULL;
    
    const char* saved_model_dir = "models/"; 
    const char* tags = "serve"; 
    
    int ntags = 1;
    TF_Session* Session = TF_LoadSessionFromSavedModel(SessionOpts, RunOpts, saved_model_dir, &tags, ntags, Graph, NULL, status);
    
    if(TF_GetCode(status) == TF_OK)
    {
        printf("TF_LoadSessionFromSavedModel OK\n");
    }
    else
    {
        printf("%s",TF_Message(status));
    }
  
	return 0;
}

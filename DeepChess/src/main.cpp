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

#define FILE_NAME "../data/DataGames.csv"

/*
  Usage : play against the AI : ./mlapp
          create dataset : ./mlapp elo, where elo is the minimum elo of games used in the dataset (max : 3300)
*/
int main(int argc, char* argv[])
{
  if(argc == 2){
	  CreateDataset(atoi(argv[1]));
  }
  else if(argc == 1){
    AIGame game;
    game.Construct(1080, 720, 1, 1);
    game.Start();
  }
	return 0;
}



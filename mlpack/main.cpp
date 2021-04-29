#include <mlpack/core.hpp>
#include <mlpack/core/data/split_data.hpp>
#include <iostream>
#include <string>
#include <math.h>
#include <map>
#include <mlpack/methods/ann/layer/layer.hpp>
#include <mlpack/methods/ann/ffn.hpp>
#include <mlpack/methods/ann/init_rules/glorot_init.hpp>
#include <mlpack/methods/ann/regularizer/regularizer.hpp>
#include <ensmallen.hpp>

#include "./include/Chess.h"
#include "./include/DataCSV.h"
#include "./include/ChessGame.h"

using namespace arma;
using namespace mlpack;
using namespace mlpack::ann;
using namespace olc;
using namespace DataCSV;
using namespace std;
using namespace ens;
//using namespace mlpack::neighbor; // NeighborSearch and NearestNeighborSort
//using namespace mlpack::metric; // ManhattanDistance

#define FILE_NAME "./data/DataGames.csv"

/*
	La sortie du modèle étant un vecteur de taille 10 avec 1 proba pour chaque noeud on prend la plus élevé
*/

arma::Row<size_t> getLabels(arma::mat predOut)
{
  arma::Row<size_t> predLabels(predOut.n_cols);
  for (arma::uword i = 0; i < predOut.n_cols; ++i)
  {
    predLabels(i) = predOut.col(i).index_max();
  }
  return predLabels;
}


int main(int argc, char* argv[])
{
  //Ranking rate
	//CreateDataset(atoi(argv[1]));
  AIGame game;
	
	return 0;
}

#include <mlpack/core.hpp>
#include <stdio.h>
#include <string>
#include <mlpack/methods/neighbor_search/neighbor_search.hpp>
#include "../include/rapidcsv.h"
using namespace mlpack;
using namespace mlpack::neighbor; // NeighborSearch and NearestNeighborSort
using namespace mlpack::metric; // ManhattanDistance
using namespace rapidcsv;
using namespace std;
int main()
{
  Document doc("../data/test.csv");
  vector<string> moves = doc.GetColumn<string>("Moves");
  cout << moves.size() << endl;
}
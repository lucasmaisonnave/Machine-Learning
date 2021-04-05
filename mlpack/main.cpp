#include <mlpack/core.hpp>
#include <iostream>
#include <string>
#include <map>
#include <mlpack/methods/neighbor_search/neighbor_search.hpp>
#include "./include/rapidcsv.h"
#include "./include/Chess.h"
using namespace mlpack;
using namespace mlpack::neighbor; // NeighborSearch and NearestNeighborSort
using namespace mlpack::metric; // ManhattanDistance
using namespace std;
using namespace rapidcsv;

vector<string> ExtractMovesSet(string filename){
  rapidcsv::Document doc(filename); 
  std::vector<std::string> col = doc.GetColumn<std::string>("Moves");
  
  for(int j = 0; j < col.size(); j++){
    col[j].erase(0,3);
    for(int i = 0; i < col[j].size(); i++){
      if(col[j][i] == '.'){
        while(col[j][i] != ' '){
          col[j].erase(i, 1);
          i--;
        }
        col[j].erase(i, 1);
      }
      else if(col[j][i] == 'x' || col[j][i] == '+' || col[j][i] == '#')
        col[j].erase(i, 1);
    }
  }
  return col;
}

string ExtractMove(string& moves){
  string move;
  while(moves.front() != ' '){
    move.push_back(moves.front());
    moves.erase(0,1);
  }
  moves.erase(0,1);
  return move;

}
/*
  Convertis a,b,c,d,e,f,g,h -> 1,2,3,4,5,6,7,8 ATTENTION on commence à 1
*/

int ConvertL(const char let){
  switch(let){
    case 'a':
      return 1;
      break;
    case 'b':
      return 2;
      break;
    case 'c':
      return 3;
      break;
    case 'd':
      return 4;
      break;
    case 'e':
      return 5;
      break;
    case 'f':
      return 6;
      break;
    case 'g':
      return 7;
      break;
    case 'h':
      return 8;
      break;
    default:
      break;
  }
  return -1;
}
/*
  Convertis un coup au format standard des échecs en action (c1, l1, c2, l2)
*/
Action ConvertToAction(string move, Chess& chess){
  Action action;
  if(move.size() == 2) //pion
  {
    action.c2 = ConvertL(move.front()) - 1;
    action.l2 = move.back() - 1;
    
  }
}

vector<Chess> ConvertMovesToVectChess(vector<string> movesSet){
  for(int i = 0; i < movesSet.size(); i++){
    Chess chess;

  }

}

int main()
{
  vector<string> movesSet = ExtractMovesSet("./data/ChessGames.csv");
  return 0;
}
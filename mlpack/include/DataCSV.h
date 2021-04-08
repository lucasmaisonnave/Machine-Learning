#ifndef DATACSV_H
#define DATACSV_H
#include <iostream>
#include <string>
#include <map>
#include <mlpack/core.hpp>
#include "rapidcsv.h"
#include "Chess.h"
using namespace rapidcsv;

#define ELO_MIN 2500

typedef Col <int> coli;


vector<string> ExtractMovesSet(string filename){
  rapidcsv::Document doc(filename); 
  std::vector<std::string> col = doc.GetColumn<std::string>("Moves");
  std::vector<int> col_elo = doc.GetColumn<int>("White Elo");
  vector<std::string>::iterator it = col.begin();
  vector<int>::iterator it_elo = col_elo.begin();
  for(int j = 0; j < col.size(); j++, it++, it_elo++){
    if(col_elo[j] < ELO_MIN){
      col.erase(it);
      col_elo.erase(it_elo);
      j--;
      it--;
      it_elo--;
    }
    else{
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
        else if(col[j][i] == '='){
          if(col[j][i + 1] != 'Q'){ //Si on transforme le pion en autre chose que la dame
            col.erase(it);
            j--;
            it--;
          }
          else
            col[j].erase(i, 2);
        }
      }
    }
  }
  return col;
}
/*
  renvoie le premier coup de la pile moves et le supprime de moves
*/
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
      return 8;
      break;
    case 'b':
      return 7;
      break;
    case 'c':
      return 6;
      break;
    case 'd':
      return 5;
      break;
    case 'e':
      return 4;
      break;
    case 'f':
      return 3;
      break;
    case 'g':
      return 2;
      break;
    case 'h':
      return 1;
      break;
    default:
      break;
  }
  return -1;
}
/*
  Convertis une coordonnées de char à int
*/
int ConvertI(const char let){
  switch(let){
    case '1':
      return 1;
      break;
    case '2':
      return 2;
      break;
    case '3':
      return 3;
      break;
    case '4':
      return 4;
      break;
    case '5':
      return 5;
      break;
    case '6':
      return 6;
      break;
    case '7':
      return 7;
      break;
    case '8':
      return 8;
      break;
    default:
      break;
  }
  return -1;
}
/*
  Convertis une pièce B, N, R, Q, K en PIECE
*/
int ConvertP(const char let){
  switch(let){
    case 'B':
      return FOU;
      break;
    case 'N':
      return CAVALIER;
      break;
    case 'R':
      return TOUR;
      break;
    case 'Q':
      return DAME;
      break;
    case 'K':
      return ROI;
      break;
    default:
      break;
  }
  return -1;
}
/*
  Convertis un coup au format standard des échecs en action (c1, l1, c2, l2)
*/
Action ConvertToAction(const string& move, Chess& chess){
  Action action = {-1,-1,-1,-1};

  if(move[0] == 'O'){ //on rook
    if(move.size() == 3){ //petit rook
      action.c1 = 3;
      action.c2 = 1;
      action.l1 = action.l2 = (chess.get_whoplays() == BLANC?0:CHESS_SIZE-1);
      return action;
    }
    else{ //Sinon c'est grand rook
      action.c1 = 3;
      action.c2 = CHESS_SIZE - 2;
      action.l1 = action.l2 = (chess.get_whoplays() == BLANC?0:CHESS_SIZE-1);
      return action;
    }
  }
  else if(move.size() == 2) //pion
  {
    action.c2 = ConvertL(move.front()) - 1;
    action.l2 = ConvertI(move.back())- 1;
    for(int c1 = action.c2 - 1; c1 <= action.c2 + 1; c1++){
      action.c1 = c1;
      action.l1 = action.l2 - (chess.get_whoplays() == BLANC ? 1 : -1);
      //Faire le cas où le pion bouge de 2 case en avant
      if(chess.getCase(action.c1, action.l1).type == PION && chess.getCase(action.c1, action.l1).couleur == (int)chess.get_whoplays())
        if(chess.play(action, true))
          return action;
    }
    //cas où le pion bouge de 2 case en avant
    action.c1 = action.c2;
    action.l1 = action.l2 - 2*(chess.get_whoplays() == BLANC ? 1 : -1);
    if(chess.getCase(action.c1, action.l1).type == PION && chess.getCase(action.c1, action.l1).couleur == chess.get_whoplays())
      if(chess.play(action, true))
        return action;
  }
  else if(move.size() == 3){
    if(ConvertL(move[0]) != -1){ //pion qui bouge mais il y a ambiguité, ex : gf5
      action.c2 = ConvertL(move[1]) - 1;
      action.l2 = ConvertI(move[2]) - 1;
      action.c1 = ConvertL(move[0]) - 1;
      for(int l1 = 0; l1 < CHESS_SIZE; l1++){
        action.l1 = l1;
        if(chess.getCase(action.c1, action.l1).type == PION && chess.getCase(action.c1, action.l1).couleur == chess.get_whoplays())
          if(chess.play(action, true))
            return action;
      }
    }
    else{ //C'est une autre pièce : B, N, R, Q, K
      action.c2 = ConvertL(move[1]) - 1;
      action.l2 = ConvertI(move.back()) - 1;
      int piece = ConvertP(move[0]);
      for(int c = 0; c < CHESS_SIZE; c++)
        for(int l = 0; l < CHESS_SIZE; l++)
          if(chess.getCase(c, l).type == piece && chess.getCase(c, l).couleur == chess.get_whoplays())
            if(chess.play(c, l, action.c2, action.l2, true))
            {
              action.c1 = c;
              action.l1 = l;
              return action;
            }
    }
  }
  else{ //taille 4
 //Ambiguité sur une pièce autre que PION, ex : Nce4
    action.c2 = ConvertL(move[2]) - 1;
    action.l2 = ConvertI(move.back()) - 1;
    action.c1 = ConvertL(move[1]) - 1;
    int piece = ConvertP(move[0]);
    for(int l = 0; l < CHESS_SIZE; l++){
      action.l1 = l;
      if(chess.getCase(action.c1, action.l1).type == piece && chess.getCase(action.c1, action.l1).couleur == chess.get_whoplays())
        if(chess.play(action, true))
          return action;
    }
  }
  return action;
}

coli ConvertPieceToVect(const int piece, const int couleur){
  coli vect = {0,0,0,0,0,0,0,0};
  if(piece == VIDE)
    return vect;
  if(couleur == BLANC)
    vect[7] = 1;
  else
    vect[6] = 1;

  
  switch(piece){
    case PION:
      vect[0] = 1;
      break;
    case DAME:
      vect[4] = 1;
      break;
    case TOUR:
      vect[2] = 1;
      break;
    case FOU:
      vect[3] = 1;
      break;
    case ROI:
      vect[5] = 1;
      break;
    case CAVALIER:
      vect[1] = 1;
      break;
    default:
      break;
  }
  return vect;
}

/*
  Convertis l'ensemble des coups du dataset en matrice utilisable pour la phase d'entrainement du modèle
  On passe la matrice par référence pour de pas avoir à copier le résultat de la fonction, la matrice est
  énorma ça prendrait beaucoup de temps.
*/

void ConvertMovesSetToMat(const vector<string>& movesSet, arma::Mat<int>& mat){
  for(string moves : movesSet){
    Chess chess;
    while(moves.size() != 0)
    {    
      string move = ExtractMove(moves);
      Action act  = ConvertToAction(move, chess);
      coli v; // vecteur image du plateau + action
      for(int c = 0; c < CHESS_SIZE; c++)
        for(int l = 0; l < CHESS_SIZE; l++)
        {
          coli pi = ConvertPieceToVect(chess.getCase(c,l).type, chess.getCase(c,l).couleur);
          v = arma::join_cols(v, pi); //On concataine chaque case dans un seul vecteur
        }
      coli c_act = {act.c1, act.l1, act.c2, act.l2, chess.get_whoplays()};
      v = arma::join_cols(v, c_act);
      mat = arma::join_rows(mat, v); // On concatene la matrice avec le nouveau vecteur 
      chess.play(act);
    }
  }
}



/*void CreateFile(const vector<pair<Chess, Action>>& statesSet, string filename){
  ofstream file;
  file.open(filename, ios::out);
  //Chess
  for(int i = 0; i < statesSet.size(); i++){
    for(int c = 0; c < CHESS_SIZE; c++)
      for(int l = 0; l < CHESS_SIZE; l++){
        int piece = statesSet[i].first.getCase(c,l).type;
        int couleur = statesSet[i].first.getCase(c,l).couleur;
        vec vect = ConvertPieceToVect(piece, couleur);
        for(int j = 0; j < vect.size(); j++){
            file << vect[j] << ",";
        }
      }
    file << statesSet[i].second.c1 << "," << statesSet[i].second.l1 << "," << statesSet[i].second.c2 << "," << statesSet[i].second.l2 << endl;
  }
  //Action
  file.close();
}*/

#endif
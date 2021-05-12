#ifndef DATACSV_H
#define DATACSV_H
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <mlpack/core.hpp>
#include "rapidcsv.h"
#include "Chess.h"
using namespace rapidcsv;
using namespace arma;
using namespace std;
typedef Col <int> coli;
typedef Row <int> rowi;
typedef Mat<int> mati;

#define TAILLE_HOT_VECT 12

vector<string> col_winner;

namespace DataCSV{
  vector<string> ExtractMovesSet(const string& filename, int elo_min){
    rapidcsv::Document doc(filename); 
    vector<string> col_moves = doc.GetColumn<string>("Moves");
    vector<int> col_elo = doc.GetColumn<int>("White Elo");
    col_winner = doc.GetColumn<string>("Result-Winner");
    for(size_t j = 0; j < col_moves.size(); j++){
      if(col_elo[j] < elo_min || col_winner[j] == "Draw"){
        col_moves.erase(col_moves.begin() + j);
        col_elo.erase(col_elo.begin() + j);
        col_winner.erase(col_winner.begin() + j);
        j--;
      }
      else{
        col_moves[j].erase(0,3);
        for(size_t i = 0; i < col_moves[j].size(); i++){
          if(col_moves[j][i] == '.'){
            while(col_moves[j][i] != ' '){
              col_moves[j].erase(col_moves[j].begin() + i);
              i--;
            }
            col_moves[j].erase(col_moves[j].begin() + i);
          }
          else if(col_moves[j][i] == 'x' || col_moves[j][i] == '+' || col_moves[j][i] == '#')
          {
            col_moves[j].erase(col_moves[j].begin() + i);
            i--;
          }
          else if(col_moves[j][i] == '='){
            if(col_moves[j][i + 1] != 'Q'){ //Si on transforme le pion en autre chose que la dame on supprime parce que j'ai pas codé cette possibilité
              col_moves.erase(col_moves.begin() + j);
              col_elo.erase(col_elo.begin() + j);
              col_winner.erase(col_winner.begin() + j);
              j--;
            }
            else
            {
              col_moves[j].erase(i, 2);
              i--;
            }
          }
        }
      }
    }
    cout << "Nombre de parties à générer : " << col_moves.size() << endl;
    return col_moves;
  }
  /*
    renvoie le premier coup de la pile moves et le supprime de moves
  */
  string ExtractMove(string& moves){
    string move;
    while(moves.front() != ' '){
      move.push_back(moves.front());
      moves.erase(moves.begin());
    }
    moves.erase(moves.begin());
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
        action.c2 = CHESS_SIZE - 3;
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
        if(chess.getCase(action.c1, action.l1).type == PION && chess.getCase(action.c1, action.l1).couleur == chess.get_whoplays())
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
          if(chess.getCase(action.c1, action.l1).type == PION && chess.getCase(action.c1, action.l1).couleur == chess.get_whoplays()){
            if(chess.play(action, true)){
              Chess next_chess = chess;
              next_chess.play(action);
              int couleur_next = next_chess.get_whoplays();
              if(!next_chess.checkThreat(next_chess.get_roi_pos(!couleur_next,0), next_chess.get_roi_pos(!couleur_next,1), !couleur_next))
                return action;
            }
          }     
        }
      }
      else{ //C'est une autre pièce : B, N, R, Q, K
        action.c2 = ConvertL(move[1]) - 1;
        action.l2 = ConvertI(move.back()) - 1;
        int piece = ConvertP(move[0]);
        for(int c = 0; c < CHESS_SIZE; c++)
          for(int l = 0; l < CHESS_SIZE; l++)
            if(chess.getCase(c, l).type == piece && chess.getCase(c, l).couleur == chess.get_whoplays()){
              if(chess.play(c, l, action.c2, action.l2, true))
              {
                Chess next_chess = chess;
                next_chess.play(c, l, action.c2, action.l2);
                int couleur_next = next_chess.get_whoplays();
                //On vérifie que le roi n'est pas en danger
                if(!next_chess.checkThreat(next_chess.get_roi_pos(!couleur_next,0), next_chess.get_roi_pos(!couleur_next,1), !couleur_next)){
                  action.c1 = c;
                  action.l1 = l;
                  return action;
                }
              }
            }
      }
    }
    else{ //taille 4
  //Ambiguité sur une pièce autre que PION, ex : Nce4 ou N1d2
      action.c2 = ConvertL(move[2]) - 1;
      action.l2 = ConvertI(move.back()) - 1;
      if(ConvertL(move[1]) != -1) //Le deuxième terme est une lettre
      {
        action.c1 = ConvertL(move[1]) - 1;
        int piece = ConvertP(move[0]);
        for(int l = 0; l < CHESS_SIZE; l++){
          action.l1 = l;
          if(chess.getCase(action.c1, action.l1).type == piece && chess.getCase(action.c1, action.l1).couleur == chess.get_whoplays())
            if(chess.play(action, true))
              return action;
        }
      }
      else
      {
        action.l1 = ConvertI(move[1]) - 1;
        int piece = ConvertP(move[0]);
        for(int c = 0; c < CHESS_SIZE; c++){
          action.c1 = c;
          if(chess.getCase(action.c1, action.l1).type == piece && chess.getCase(action.c1, action.l1).couleur == chess.get_whoplays())
            if(chess.play(action, true))
              return action;
        }
      }
    }
    return action;
  }

  coli ConvertPieceToVect(const int piece, const int couleur){
    
    if(couleur == BLANC && piece == PION){
      coli vect = {1,0,0,0,0,0,0,0,0,0,0,0};
      return vect;
    }
    else if(couleur == BLANC && piece == CAVALIER){
      coli vect = {0,1,0,0,0,0,0,0,0,0,0,0};
      return vect;
    }
    else if(couleur == BLANC && piece == FOU){
      coli vect = {0,0,1,0,0,0,0,0,0,0,0,0};
      return vect;
    }
    else if(couleur == BLANC && piece == TOUR){
      coli vect = {0,0,0,1,0,0,0,0,0,0,0,0};
      return vect;
    }
    else if(couleur == BLANC && piece == DAME){
      coli vect = {0,0,0,0,1,0,0,0,0,0,0,0};
      return vect;
    }
    else if(couleur == BLANC && piece == ROI){
      coli vect = {0,0,0,0,0,1,0,0,0,0,0,0};
      return vect;
    }

    if(couleur == NOIR && piece == PION){
      coli vect = {0,0,0,0,0,0,1,0,0,0,0,0};
      return vect;
    }
    else if(couleur == NOIR && piece == CAVALIER){
      coli vect = {0,0,0,0,0,0,0,1,0,0,0,0};
      return vect;
    }
    else if(couleur == NOIR && piece == FOU){
      coli vect = {0,0,0,0,0,0,0,0,1,0,0,0};
      return vect;
    }
    else if(couleur == NOIR && piece == TOUR){
      coli vect = {0,0,0,0,0,0,0,0,0,1,0,0};
      return vect;
    }
    else if(couleur == NOIR && piece == DAME){
      coli vect = {0,0,0,0,0,0,0,0,0,0,1,0};
      return vect;
    }
    else if(couleur == NOIR && piece == ROI){
      coli vect = {0,0,0,0,0,0,0,0,0,0,0,1};
      return vect;
    }
    coli vect = {0,0,0,0,0,0,0,0,0,0,0,0};
    return vect;
  }

  /*
    Cette fonction bug à la fin : 
    free(): invalid pointer
    Aborted (core dumped)
    mais je n'ai aucune idée de pourquoi, la variable movesSet qui est sensé être constante est modifié par je ne sais quelle magie
    Ça n'empèche pas le programme de fonctionner et d'enregistrer les données mais c'est chiant parce que je comprends pas
  */

  void ConvertMovesSetToFile(const vector<string>& movesSet, const string filename){
    vector<string> test = movesSet;
    ofstream file(filename, ios::trunc);
    size_t nb_games = movesSet.size();
    float k = 0;
    for(string moves : movesSet){
      cout << "Progress : " << (int) (((k + 1)/ nb_games) * 100) << "%\r" << flush;
      Chess chess;
      int winner = col_winner[k] == "White" ? 1 : -1;
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
        coli c_act = {act.c1, act.l1, act.c2, act.l2, chess.get_whoplays(), winner};
        v = arma::join_cols(v, c_act);
        //Save line
        for(size_t i = 0; i < v.size(); i++){
          if(i != v.size() - 1){
            file << v[i] << ",";
          }
          else{
            file << v[i] << endl;
          }
        }
        chess.play(act);
      }
      k++;
    }
    cout << endl;
    file.close();
  }

  void CreateDataset(int elo_min){
    cout << "Loading moves" << endl;
    const vector<string> movesSet = DataCSV::ExtractMovesSet("./data/DataSet.csv", elo_min);
    cout << "Saving data" << endl;
    DataCSV::ConvertMovesSetToFile(movesSet, "./data/DataSet_processed.csv");
  }
}
#endif
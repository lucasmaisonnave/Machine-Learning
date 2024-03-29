#pragma once
#include <iostream>
#include <vector>
using namespace std;
#define CHESS_SIZE 8
#define SGN(l) (l >= 0 ? 1 : -1)


enum PIECES { FOU, ROI, CAVALIER, PION, DAME, TOUR, VIDE };
enum COULEUR { NOIR, BLANC };
int AI_SIDE = NOIR;

typedef struct CASE {
	int type;
	int couleur;
}CASE;
typedef struct Action {
	int c1;
	int l1;
	int c2;
	int l2;
}Action;


template<typename T>
vector<T> ConvertPieceToVectInt(const int piece, const int couleur){ 
  if(couleur == BLANC && piece == PION){
    vector<T> vect = {1,0,0,0,0,0,0,0,0,0,0,0};
    return vect;
  }
  else if(couleur == BLANC && piece == CAVALIER){
    vector<T> vect = {0,1,0,0,0,0,0,0,0,0,0,0};
    return vect;
  }
  else if(couleur == BLANC && piece == FOU){
    vector<T> vect = {0,0,1,0,0,0,0,0,0,0,0,0};
    return vect;
  }
  else if(couleur == BLANC && piece == TOUR){
    vector<T> vect = {0,0,0,1,0,0,0,0,0,0,0,0};
    return vect;
  }
  else if(couleur == BLANC && piece == DAME){
    vector<T> vect = {0,0,0,0,1,0,0,0,0,0,0,0};
    return vect;
  }
  else if(couleur == BLANC && piece == ROI){
    vector<T> vect = {0,0,0,0,0,1,0,0,0,0,0,0};
    return vect;
  }

  if(couleur == NOIR && piece == PION){
    vector<T> vect = {0,0,0,0,0,0,1,0,0,0,0,0};
    return vect;
  }
  else if(couleur == NOIR && piece == CAVALIER){
    vector<T> vect = {0,0,0,0,0,0,0,1,0,0,0,0};
    return vect;
  }
  else if(couleur == NOIR && piece == FOU){
    vector<T> vect = {0,0,0,0,0,0,0,0,1,0,0,0};
    return vect;
  }
  else if(couleur == NOIR && piece == TOUR){
    vector<T> vect = {0,0,0,0,0,0,0,0,0,1,0,0};
    return vect;
  }
  else if(couleur == NOIR && piece == DAME){
    vector<T> vect = {0,0,0,0,0,0,0,0,0,0,1,0};
    return vect;
  }
  else if(couleur == NOIR && piece == ROI){
    vector<T> vect = {0,0,0,0,0,0,0,0,0,0,0,1};
    return vect;
  }
  vector<T> vect = {0,0,0,0,0,0,0,0,0,0,0,0};
  return vect;
}



class Chess {
public:
	Chess() : plat_vec(CHESS_SIZE*CHESS_SIZE*12) {
		int type[CHESS_SIZE][CHESS_SIZE] = {{TOUR, CAVALIER, FOU, ROI, DAME, FOU, CAVALIER, TOUR},
											{PION, PION, PION, PION, PION, PION, PION, PION},
											{VIDE, VIDE, VIDE, VIDE, VIDE, VIDE, VIDE, VIDE},
											{VIDE, VIDE, VIDE, VIDE, VIDE, VIDE, VIDE, VIDE},
											{VIDE, VIDE, VIDE, VIDE, VIDE, VIDE, VIDE, VIDE},
											{VIDE, VIDE, VIDE, VIDE, VIDE, VIDE, VIDE, VIDE},
											{PION, PION, PION, PION, PION, PION, PION, PION},
											{TOUR, CAVALIER, FOU, ROI, DAME, FOU, CAVALIER, TOUR}};
		int couleur[CHESS_SIZE][CHESS_SIZE] = {{BLANC, BLANC, BLANC, BLANC, BLANC, BLANC, BLANC, BLANC},
											   {BLANC, BLANC, BLANC, BLANC, BLANC, BLANC, BLANC, BLANC},
											   {VIDE, VIDE, VIDE, VIDE, VIDE, VIDE, VIDE, VIDE},
											   {VIDE, VIDE, VIDE, VIDE, VIDE, VIDE, VIDE, VIDE},
											   {VIDE, VIDE, VIDE, VIDE, VIDE, VIDE, VIDE, VIDE},
											   {VIDE, VIDE, VIDE, VIDE, VIDE, VIDE, VIDE, VIDE},
											   {NOIR, NOIR, NOIR, NOIR, NOIR, NOIR, NOIR, NOIR},
											   {NOIR, NOIR, NOIR, NOIR, NOIR, NOIR, NOIR, NOIR} };
		
		/*
		int type[CHESS_SIZE][CHESS_SIZE] = {{TOUR, VIDE, VIDE, ROI, VIDE, VIDE, VIDE, TOUR},
											{VIDE, VIDE, VIDE, VIDE, VIDE, VIDE, VIDE, VIDE},
											{VIDE, VIDE, VIDE, VIDE, VIDE, VIDE, VIDE, VIDE},
											{VIDE, VIDE, VIDE, VIDE, VIDE, VIDE, VIDE, VIDE},
											{VIDE, VIDE, VIDE, VIDE, VIDE, VIDE, VIDE, VIDE},
											{VIDE, VIDE, VIDE, VIDE, VIDE, VIDE, VIDE, VIDE},
											{PION, PION, PION, PION, PION, PION, PION, PION},
											{TOUR, CAVALIER, FOU, ROI, DAME, FOU, CAVALIER, TOUR}};
		int couleur[CHESS_SIZE][CHESS_SIZE] = {{BLANC, VIDE, VIDE, BLANC, VIDE, VIDE, VIDE, BLANC},
											   {VIDE, VIDE, VIDE, VIDE, VIDE, VIDE, VIDE, VIDE},
											   {VIDE, VIDE, VIDE, VIDE, VIDE, VIDE, VIDE, VIDE},
											   {VIDE, VIDE, VIDE, VIDE, VIDE, VIDE, VIDE, VIDE},
											   {VIDE, VIDE, VIDE, VIDE, VIDE, VIDE, VIDE, VIDE},
											   {VIDE, VIDE, VIDE, VIDE, VIDE, VIDE, VIDE, VIDE},
											   {NOIR, NOIR, NOIR, NOIR, NOIR, NOIR, NOIR, NOIR},
											   {NOIR, NOIR, NOIR, NOIR, NOIR, NOIR, NOIR, NOIR} };
		*/
		for(int i = 0; i < CHESS_SIZE; i++)
			for (int j = 0; j < CHESS_SIZE; j++)
			{
				plateau[i][j].type = type[i][j];
				plateau[i][j].couleur = couleur[i][j];
				Nb_Piece[plateau[i][j].couleur][plateau[i][j].type]++;
				InsertV(i, j);
			}

	}
	Chess(const Chess& ch) {
		for (int i = 0; i < CHESS_SIZE; i++)
			for (int j = 0; j < CHESS_SIZE; j++)
				plateau[i][j] = ch.plateau[i][j];
		Roi_mov[0] = ch.Roi_mov[0]; Roi_mov[1] = ch.Roi_mov[1];
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++){
				Tour_mov[i][j] = ch.Tour_mov[i][j];
				pos_roi[i][j] = ch.pos_roi[i][j];
			}
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 6; j++)
				Nb_Piece[i][j] = ch.Nb_Piece[i][j];
		who_plays = ch.who_plays;
		plat_vec = ch.plat_vec;
	}
	CASE getCase(int colonne, int ligne) const {
		return plateau[ligne][colonne];
	}
	int getNb_Piece(int couleur, int type) const
	{
		return Nb_Piece[couleur][type];
	}
	int getScoreMat(int col) const {
		return getNb_Piece(col, PION)*10 + getNb_Piece(col, CAVALIER) * 30 + getNb_Piece(col, FOU) * 30 + getNb_Piece(col, TOUR) * 50 + getNb_Piece(col, DAME) * 90 + getNb_Piece(col, ROI) * 900;
	}
	int get_whoplays() const{
		return who_plays;
	}
	/*
		cl : 0 colonne, 0 ligne
	*/
	int get_roi_pos(int couleur, int cl){
		return pos_roi[couleur][cl];
	}
	vector<float> get_plat_vec() const{
		return plat_vec;
	}
	void set_whoplays(uint8_t wp){
		who_plays = wp;
	}
	bool checkFouObstacle(int l1, int c1, int l2, int c2) const {
		int sl = SGN(l2 - l1);
		int sc = SGN(c2 - c1);
		int c = c1 + sc;
		//On test si le coup est sur un diagonale du fou
		if (!(abs(c1 - c2) == abs(l1 - l2)))
			return false;
		//On test les obstacles
		for (int l = l1 + sl; l != l2; l += sl, c += sc)
			if (plateau[l][c].type != VIDE)
				return false;
		return true;
	}
	bool checkTourObstacle(int l1, int c1, int l2, int c2) const {
		if (!(c1 == c2 || l1 == l2))
			return false;

		if (c1 == c2)
		{
			int s = SGN(l2 - l1);
			for (int l = l1 + s; l != l2; l += s)
				if (plateau[l][c1].type != VIDE)
					return false;
		}
		else if (l1 == l2)
		{
			int s = SGN(c2 - c1);
			for (int c = c1 + s; c != c2; c += s)
				if (plateau[l1][c].type != VIDE)
					return false;
		}
		return true;
	}
	//Retourne true s'il y a une menace sur la case (c,l), col �tant la couleur du menac�
	bool checkThreat(int c, int l, int col) {
		for (int _c = 0; _c < CHESS_SIZE; _c++)
		{
			for (int _l = 0; _l < CHESS_SIZE; _l++)
			{
				if (plateau[_l][_c].type != VIDE && plateau[_l][_c].couleur != col)
				{
					if (play(_c, _l, c, l, true))
					{
						return true;
					}
				}
			}
		}
		return false;
	}
	bool play(int c1, int l1, int c2, int l2, bool test = false) {
		bool pion_dame = false; //vrai quand le pion doit se transformer en Dame

		/*On test d'abord si le coup est valide sans tenir compte de type des pions � utiliser:
		la case suivante ne doit pas �tre la m�me que celle de d�part, la case � bouger n'est pas vide, la case o� aller n'est pas pleine et pas de la m�me couleur
		et les coordonn�es rentr�es sont bien dans le plateau
		*/
		if (c1 >= CHESS_SIZE || c1 < 0 || l1 >= CHESS_SIZE || l1 < 0 || c2 >= CHESS_SIZE || c2 < 0 || l2 >= CHESS_SIZE || l2 < 0)
			return false;
		if (plateau[l1][c1].type == VIDE || (l1 == l2 && c1 == c2) || (plateau[l2][c2].type != VIDE && plateau[l2][c2].couleur == plateau[l1][c1].couleur))
			return false;
		int couleur = getCase(c1,l1).couleur; //Pour le PION uniquement
		switch (plateau[l1][c1].type)
		{
		case PION:
			//On v�rifie d'abord que le pion avance, rappel NOIR en bas avec (0,0) en haut � gauche
			if ((plateau[l1][c1].couleur == NOIR && l2 - l1 > 0) || (plateau[l1][c1].couleur == BLANC && l2 - l1 < 0))
				return false;
			//On bouge pas le pion � plus de 2 cases
			if (abs(l1 - l2) > 2)
				return false;
			
			//On autorise de bouger de 1 en diagonale s'il y a un pion adverse, on peut bouger de 1 vers le haut s'il n'y a pas d'obstacle 
			//et on peut bouger de 2 vers le haut si le pion est sur sa case de d�part et qu'il n'y pas de pion sur la case vis�
			//En passant
			if (!((abs(c1 - c2) == 1 && abs(l1 - l2) == 1 && plateau[l2][c2].type != VIDE) || 
				(abs(c1 - c2) == 0 && abs(l1 - l2) == 1 && plateau[l2][c2].type == VIDE) || 
				(abs(c1 - c2) == 0 && abs(l1 - l2) == 2 && (((plateau[l1][c1].couleur == NOIR && l1 == CHESS_SIZE - 2) || (plateau[l1][c1].couleur == BLANC && l1 == 1)) && plateau[l2][c2].type == VIDE && plateau[l2 - SGN(l2 - l1)][c2].type == VIDE)) ||
				(l1 == (couleur == BLANC ? 4 : 3) && abs(c1 - c2) == 1 && abs(l1 - l2) == 1 && getCase(c2,l2).type == VIDE && en_passant[!couleur][c2])))
				return false;
			//On test si le pion est sur la derni�re ligne, on le transforme en Dame
			if ((plateau[l1][c1].couleur == NOIR && l2 == 0) || (plateau[l1][c1].couleur == BLANC && l2 == CHESS_SIZE - 1))
				pion_dame = true;
			//Maj du tab en passant
			if(!test)
			{
				if((l1 == (couleur == BLANC ? 4 : 3) && abs(c1 - c2) == 1 && abs(l1 - l2) == 1 && getCase(c2,l2).type == VIDE && en_passant[!couleur][c2]))
					plateau[l1][c2].type = VIDE;
				for(int col = 0; col < 2; col++)
					for(int c = 0; c < CHESS_SIZE; c++)
						en_passant[col][c] = false;
				if(abs(l1 - l2) == 2)
					en_passant[couleur][c1] = true;
			}
			break;
		case FOU:
			//On v�rifie qu'il n'y a pas d'obstacle + on est sur la diagonale
			if (!checkFouObstacle(l1, c1, l2, c2))
				return false;
			break;
		case TOUR:
			//V�rifier s'il n'y pas d'obstacle entre p1 et p2 + on est en ligne
			if (!checkTourObstacle(l1, c1, l2, c2))
				return false;
			//On test si la tour bouge => pas de rock possible
			if (!test)
			{
				if (l1 == 0 && c1 == 0)
					Tour_mov[BLANC][0] = true;
				else if (l1 == CHESS_SIZE - 1 && c1 == 0)
					Tour_mov[NOIR][0] = true;
				else if (l1 == 0 && c1 == CHESS_SIZE - 1)
					Tour_mov[BLANC][1] = true;
				else if (l1 == CHESS_SIZE - 1 && c1 == CHESS_SIZE - 1)
					Tour_mov[NOIR][1] = true;
			}

			break;
		case CAVALIER:
			if (!((abs(c2 - c1) == 1 && abs(l2 - l1) == 2) || (abs(c2 - c1) == 2 && abs(l2 - l1) == 1)))
				return false;
			break;
		case DAME:
			if (!((abs(c1 - c2) == abs(l1 - l2)) || (c1 == c2 || l1 == l2)))
				return false;
			//On v�rifie les obstacles
			if (!(checkTourObstacle(l1, c1, l2, c2) || checkFouObstacle(l1, c1, l2, c2)))
				return false;
			break;
		case ROI:
			//Test si on roque
			int s = SGN(c2 - c1);
			if (abs(c1 - c2) == 2 && (l1 == l2) && (l1 == (plateau[l1][c1].couleur == NOIR ? CHESS_SIZE - 1 : 0)) && !Roi_mov[plateau[l1][c1].couleur] && !Tour_mov[plateau[l1][c1].couleur][s > 0 ? 1 : 0])
			{
				//Test des obstacles entre le roi et la tour
				for (int c = c1 + s; c != (s > 0 ? CHESS_SIZE - 1 : 0); c += s)
					if (plateau[l1][c].type != VIDE)
						return false;

				//Test des menaces, ici on test aussi la case du roi et de la tour
				if(SGN(c2 - c1) < 0) //petit roque
				{
					for (int c = c1; c >= 0; c -= 1)
						if (checkThreat(c, l1, plateau[l1][c1].couleur))
							return false;
				}
				else //Grand roque, la tour ainsi que sa case adjacente peuvent être menacés
				{
					for (int c = c1; c <= c1 + 2; c += 1)
						if (checkThreat(c, l1, plateau[l1][c1].couleur))
							return false;
				}
				//On met la tour � c�t� du roi
				if (!test) 
				{
					plateau[l1][c2 - s].type = TOUR;
					plateau[l1][c2 - s].couleur = plateau[l1][c1].couleur;
					plateau[l1][s > 0 ? CHESS_SIZE - 1 : 0].type = VIDE;
				}
			}
			//Sinon on bouge de 1
			else if (!(abs(l1 - l2) <= 1 && abs(c1 - c2) <= 1))
			{
				return false;
			}
			//Mise � jour de Roi_mov
			if(!test)
				Roi_mov[plateau[l1][c1].couleur] = true;//On va bouger le roi donc on le marque dans le tableau qui permet de rock
			break;
		}
		//On bouge la pi�ce
		if (!test) {
			if (plateau[l2][c2].type != VIDE)
				Nb_Piece[plateau[l2][c2].couleur][plateau[l2][c2].type]--;
			plateau[l2][c2] = plateau[l1][c1];
			InsertV(l2, c2);
			plateau[l1][c1].type = VIDE;
			InsertV(l1, c1);
			//Mise à jour de la position du roi			
			if(plateau[l2][c2].type == ROI){
				pos_roi[plateau[l2][c2].couleur][0] = c2;
				pos_roi[plateau[l2][c2].couleur][1] = l2;
			}
			if (pion_dame)
			{
				Nb_Piece[plateau[l2][c2].couleur][PION]--;
				Nb_Piece[plateau[l2][c2].couleur][DAME]++;
				plateau[l2][c2].type = DAME;
				InsertV(l2, c2);
			}
			who_plays = !who_plays;
		}
		return true;
	}
	bool play(const Action& action, bool test = false) {
		return play(action.c1, action.l1, action.c2, action.l2, test);
	}
	void InsertV(int l, int c){
		vector<float> v = ConvertPieceToVectInt<float>(plateau[l][c].type, plateau[l][c].couleur);
		for(size_t n = 0; n < v.size(); n++){
			plat_vec[c*CHESS_SIZE*12 + l*12 + n] = v[n];
		}
	}
	
private:
	CASE plateau[CHESS_SIZE][CHESS_SIZE]; //(ligne, colonne)
	vector<float> plat_vec;
	//Pour rock
	bool Roi_mov[2] = { false, false };
	bool Tour_mov[2][2] = { {false, false}, {false, false} };//(couleur, dir)
	int Nb_Piece[2][6] = { {0,0,0,0,0,0}, {0,0,0,0,0,0}};//Nombre de pi�ces par type et couleur
	int who_plays = BLANC;
	bool en_passant[2][8] = { {false,false,false,false,false,false,false,false}, {false,false,false,false,false,false,false,false}};
	int pos_roi[2][2] = {{3,7}, {3,0}}; 
};
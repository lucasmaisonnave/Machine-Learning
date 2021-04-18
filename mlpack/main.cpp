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

using namespace mlpack;
using namespace arma;
using namespace mlpack;
using namespace mlpack::ann;

using namespace std;

using namespace ens;
//using namespace mlpack::neighbor; // NeighborSearch and NearestNeighborSort
//using namespace mlpack::metric; // ManhattanDistance

#include "./include/Chess.h"
#include "./include/DataCSV.h"
#include "./include/ChessGame.h"
using namespace olc;
using namespace std;
using namespace DataCSV;

#define FILE_NAME "./data/DataGames.csv"
#ifndef DARMA_64BIT_WORD	
	#define DARMA_64BIT_WORD 1
#endif

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


int main()
{
	//CreateDataset(2300);

	const float r_split = 0.2;

	// Allow infinite number of iterations until we stopped by EarlyStopAtMinLoss
	constexpr int MAX_ITERATIONS = 0;

	// Step size of the optimizer.
	constexpr double STEP_SIZE = 1.2e-3;

	// Number of data points in each iteration of SGD.
	constexpr int BATCH_SIZE = 50;
	/*AIGame game;
	game.Construct(1080, 720, 1, 1);
	game.Start();*/

	//Test des la création des matrices d'entrées
	arma::mat data;
	arma::mat X;
	rowvec y_c1;
	rowvec y_l1;
	rowvec y_c2;
	rowvec y_l2;

	rowvec couleur;
	cout << "Loading file" << endl;
	data::Load(FILE_NAME, data, true);

	X = data.submat(0,0, data.n_rows - 6, data.n_cols - 1);
	y_c1 = data.row(data.n_rows - 5);
	y_l1 = data.row(data.n_rows - 4);
	y_c2 = data.row(data.n_rows - 3);
	y_l2 = data.row(data.n_rows - 2);
	couleur = data.row(data.n_rows - 1);
	//Le vecteur des labels doit commencer à 1	
	for(size_t i = 0; i < y_c1.n_cols; i++){
		y_c1(i) = y_c1(i) + 1;
		y_l1(i) = y_l1(i) + 1;
		y_c2(i) = y_c2(i) + 1;
		y_l2(i) = y_l2(i) + 1;
	}
	/*
	 	On prend uniquement les coups des blancs
	*/
	arma::mat X_blanc;
	arma::rowvec y_c1_blanc;
	for(size_t i = 0; i < X.n_cols; i++)
		if(couleur(i) == 1){
			X_blanc = arma::join_rows(X_blanc, X.col(i));
			arma::rowvec uw(1) ;
			uw(0) = y_c1(i);
			y_c1_blanc = arma::join_rows(y_c1_blanc, uw);
		}
	//Train set
	arma::mat X_train;
	rowvec y_c1_train;
	rowvec y_l1_train;
	rowvec y_c2_train;
	rowvec y_l2_train;
	//Test set
	arma::mat X_test;
	rowvec y_c1_test;
	rowvec y_l1_test;
	rowvec y_c2_test;
	rowvec y_l2_test;
	
	//Modèle pour y_c1
	mlpack::data::Split(X_blanc, y_c1_blanc, X_train, X_test, y_c1_train, y_c1_test, r_split);
	// Layers schema.
	// 96x8x1 --- conv (6 filters of size 3x3. stride = 1)  ---> 94x6x6
	// 94x6x6 --------------- Leaky ReLU  ---------------------> 94x6x6
	// 94x6x6 --- max pooling (over 2x2 fields. stride = 2)  --> 47x3x6
	// 47x3x6 --- conv (16 filters of size 3x3. stride = 1)  --> 45x2x16
	// 45x2x16 --------------- Leaky ReLU ---------------------> 45x2x16
	// 45x2x16 --- max pooling (over 2x2 fields. stride = 1) --> 45x2x16
	// 45x2x16  ------------------- Dense ----------------------> 8

	FFN<NegativeLogLikelihood<>, RandomInitialization> model;

	model.Add<Convolution<>>(1,  // Number of input activation maps.
                           6,  // Number of output activation maps.
                           3,  // Filter width.
                           3,  // Filter height.
                           1,  // Stride along width.
                           1,  // Stride along height.
                           0,  // Padding width.
                           0,  // Padding height.
						   8*TAILLE_HOT_VECT  ,  // Input width.
						   8 // Input height.
  	);
	  /* Add first ReLU.*/
  	model.Add<LeakyReLU<>>();

  	// Add first pooling layer. Pools over 2x2 fields in the input.
  	model.Add<MaxPooling<>>(2, // Width of field.
                          	2, // Height of field.
                          	2, // Stride along width.
                          	2, // Stride along height.
                          	true);

  	// Add the second convolution layer.
  	model.Add<Convolution<>>(6,  // Number of input activation maps.
                           	16, // Number of output activation maps.
                          	5,  // Filter width.
                           	5,  // Filter height.
                           	1,  // Stride along width.
                           	1,  // Stride along height.
                           	0,  // Padding width.
                           	0,  // Padding height.
                           	31, // Input width.
                           	3  // Input height.
  	);

  	// Add the second ReLU.
  	model.Add<LeakyReLU<>>();

  	// Add the second pooling layer.
  	model.Add<MaxPooling<>>(2, 2, 2, 2, true);
	// Add the final dense layer.
	model.Add<Linear<>>(16 * 2 * 30, 8);
	model.Add<LogSoftMax<>>();
	
	cout << "Start training ..." << endl;

	// Set parameters for the Adam optimizer.
	ens::Adam optimizer(
		STEP_SIZE,  // Step size of the optimizer.
		BATCH_SIZE, // Batch size. Number of data points that are used in each
					// iteration.
		0.9,        // Exponential decay rate for the first moment estimates.
		0.999, // Exponential decay rate for the weighted infinity norm estimates.
		1e-8,  // Value used to initialise the mean squared gradient parameter.
		MAX_ITERATIONS, // Max number of iterations.
		1e-8,           // Tolerance.
		true);

	// Train the CNN model. If this is the first iteration, weights are
	// randomly initialized between -1 and 1. Otherwise, the values of weights
	// from the previous iteration are used.
	cout << X_train.n_cols << endl;
	cout << y_c1_train.n_cols << endl;
	model.Train(X_train,
				y_c1_train,
				optimizer,
				ens::PrintLoss(),
				ens::ProgressBar(),
				// Stop the training using Early Stop at min loss.
				ens::EarlyStopAtMinLoss(
					[&](const arma::mat& /* param */)
					{
						double validationLoss = model.Evaluate(X_test, y_c1_test);
						std::cout << "Validation loss: " << validationLoss
							<< "." << std::endl;
						return validationLoss;
					}));
	// Matrix to store the predictions on train and validation datasets.
	arma::mat predOut;
	// Get predictions on training data points.
	model.Predict(X_train, predOut);
	// Calculate accuracy on training data points.
	arma::Row<size_t> predLabels = getLabels(predOut);
	double trainAccuracy = arma::accu(predLabels == y_c1_train) / (double) y_c1_train.n_elem * 100;
	// Get predictions on validating data points.
	model.Predict(X_test, predOut);
	// Calculate accuracy on validating data points.
	predLabels = getLabels(predOut);
	double validAccuracy =
		arma::accu(predLabels == y_c1_test) / (double) y_c1_test.n_elem * 100;

	std::cout << "Accuracy: train = " << trainAccuracy << "%,"
				<< "\t valid = " << validAccuracy << "%" << std::endl;

	mlpack::data::Save("./models/model.bin", "modelc1", model, false);

	/*std::cout << "Predicting ..." << std::endl;

	// Load test dataset
	// The original file could be download from
	// https://www.kaggle.com/c/digit-recognizer/data
	mlpack::data::Load("../data/mnist_test.csv", dataset, true);
	dataset.shed_row(dataset.n_rows - 1); // Remove labels before predicting.
	
	// Matrix to store the predictions on test dataset.
	mat testPredOut;
	// Get predictions on test data points.
	model.Predict(dataset, testPredOut);
	// Generate labels for the test dataset.
	Row<size_t> testPred = getLabels(testPredOut);
	std::cout << "Saving predicted labels to \"results.csv.\"..." << std::endl;

	// Saving results into Kaggle compatibe CSV file.
	testPred.save("results.csv", arma::csv_ascii);
	std::cout << "Neural network model is saved to \"model.bin\"" << std::endl;
	std::cout << "Finished" << std::endl;
	*/
	
	//Test de la création du fichier
	//DataCSV::CreateDataset(2300);
	return 0;
}

/*int main()
{
  
  return 0;
}*/
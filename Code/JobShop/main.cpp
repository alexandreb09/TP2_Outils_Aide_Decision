#include "Bierwith.h"



main(int, char **) {	 

	afficherIntro();

	// srand(123456897);
	int n_ite = 10;
	t_probleme probleme;

	t_population population;
	lire_fichier("..//Sources//ft06.txt", probleme);
	afficher(probleme);

	t_solution sol;
	generer_vect_alea(probleme, sol);

	inverser_duree(probleme);
	// afficher_Pinv(probleme);
	inverser_machine(probleme);
	// afficher_machine_inv(probleme);

	/* 
	// Exemple cours 3 x 3
	sol.Bierwirth[1] = 1;
	sol.Bierwirth[2] = 2;
	sol.Bierwirth[3] = 2;
	sol.Bierwirth[4] = 3;
	sol.Bierwirth[5] = 3;
	sol.Bierwirth[6] = 1;
	sol.Bierwirth[7] = 2;
	sol.Bierwirth[8] = 1;
	sol.Bierwirth[9] = 3;
	*/

	//evaluer(probleme, sol);

	//rechercheLocal(probleme, sol, n_ite);

	genererPopulationAlea( population,  probleme,  sol, n_ite);
	//croisement(probleme, population.liste[1], population.liste[2], enfant);
	string stop;
	//cout << endl << endl << endl << "Apres " << n_ite << " iterations, on trouve un makespan : " << sol.makespan << endl << "Appuyer pour sortir (^_-)";
	cin >> stop;

	return 0;
}

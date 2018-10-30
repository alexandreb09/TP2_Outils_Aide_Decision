#include "Bierwith.h"


int main(int, char **) {

	srand(123456897);
	t_probleme probleme;
	lire_fichier("..//Sources//ft06.txt", probleme);
	afficher(probleme);

	t_solution sol;
	generer_vect_alea(probleme, sol);

	inverser_duree(probleme);
	afficher_Pinv(probleme);

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

	inverser_machine(probleme);
	afficher_machine_inv(probleme);

	evaluer(probleme, sol);

	string stop;
	cout << endl << "appuyer pour sortir (^_-)";
	cin >> stop;

	return 0;
}
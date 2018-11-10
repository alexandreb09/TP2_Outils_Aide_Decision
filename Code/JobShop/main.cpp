#include "Bierwith.h"




int main(int, char **){

	// srand(123456897);							// Fixer aléatoire

	t_probleme probleme;
	t_population population;
	t_population elite;
	string nom_fichier = "..//Sources//la10.txt";

	afficher_intro();

	lire_fichier(nom_fichier, probleme);
	afficher_probleme(probleme, nom_fichier);

	clock_t begin = clock();

	algoGenetique(probleme, population, elite);

	double tps_ecoule = double(clock() - begin) / CLOCKS_PER_SEC;

	afficher_resultat(elite, tps_ecoule);

	string stop;
	cin >> stop;

	return 0;
}



#include "Bierwith.h"




int main(int, char **){
	t_probleme probleme;
	t_population population;
	t_population elite;
	string nom_fichier = "..//Sources//la10.txt";

	afficher_intro();

	lire_fichier(nom_fichier, probleme);
	afficher_probleme(probleme, nom_fichier);

	clock_t begin = clock();										// D�but mesure temps

	algoGenetique(probleme, population, elite);						// Recherche solution

	double tps_ecoule = double(clock() - begin) / CLOCKS_PER_SEC;	// Temps �coul�

	afficher_resultat(elite, tps_ecoule);							// Affichage r�sultats

	string stop;
	cin >> stop;

	return 0;
}



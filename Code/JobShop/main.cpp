#include "Bierwith.h"




int main(int, char **){
	t_probleme probleme;
	t_population population;
	string nom_fichier = "..//Sources//la09.txt";

	afficher_intro();
	srand(123456780);

	lire_fichier(nom_fichier, probleme);
	afficher_probleme(probleme, nom_fichier);

	clock_t begin = clock();										// D�but mesure temps

	algoGenetique(probleme, population);							// Recherche solution

	double tps_ecoule = double(clock() - begin) / CLOCKS_PER_SEC;	// Temps �coul�

	afficher_resultat(population, tps_ecoule);						// Affichage r�sultats

	string stop;
	cin >> stop;

	return 0;
}



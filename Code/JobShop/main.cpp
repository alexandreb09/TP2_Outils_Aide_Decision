#include "Bierwith.h"




int main(int, char **){

	// srand(123456897);

	t_probleme probleme;
	t_population population;
	t_population elite;

	afficher_intro();

	lire_fichier("..//Sources//la01.txt", probleme);
	afficher(probleme);


	// generer_vect_alea(probleme, sol1);
	// generer_vect_alea(probleme, sol2);

	// croisement(probleme, sol1, sol2, enfant);

	inverser_duree(probleme);
	// afficher_Pinv(probleme);
	inverser_machine(probleme);
	// afficher_machine_inv(probleme);
<<<<<<< HEAD
	
	// evaluer(probleme, sol);

	// genererPopulationAlea(population, probleme);

	// rechercheLocal(probleme, sol, n_ite);

	// genererPopulationAlea(population,  probleme,  sol, 0);

	// cout << population.liste[1].Bierwirth[3];;
	// croisement(probleme, population.liste[1], population.liste[2], enfant);
	// algoGenetique( probleme,  sol, population,  elite, 30, n_ite, population.nbIndividu - 2);

	//cout << endl << endl << endl << "Apres " << n_ite << " iterations, on trouve un makespan : " << sol.makespan << endl << "Appuyer pour sortir (^_-)";
	algoGenetique(probleme, population, elite);
	
	cout << "makespan : " << elite.liste[1].makespan << endl;

	string stop;
=======
	//evaluer(probleme, sol);

	//rechercheLocal(probleme, sol, n_ite);

	t_population elite;
	//hashage(elite, sol);

	//genererPopulationAlea( population,  probleme,  sol, n_ite);
	//selectionBestIndividus(population, 5, elite);
	t_solution enfant;
	//cout << population.liste[1].Bierwirth[3];;
	//croisement(probleme, population.liste[1], population.liste[2], enfant);
	algoGenetique( probleme,  sol, population,  elite, 30, n_ite, population.nbIndividu - 2);
	string stop;
	cout << endl << endl << endl << "Apres " << n_ite << " iterations, on trouve un makespan : " << sol.makespan << endl << "Appuyer pour sortir (^_-)";
>>>>>>> fbd25e96ed718d1e28b47371a72191b44fa93b25
	cin >> stop;

	return 0;
}



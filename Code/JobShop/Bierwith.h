#pragma once

#ifndef BIERWITH
#define BIERWITH

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <cstdlib>								// Aléatoire
#include <vector>
#include <algorithm>							// Tri
#include <time.h>								// Mesure tmp

using namespace std;

const int nb_max_machines = 20;					// Surdimensionnement nombre machines
const int nb_max_pieces = 20;					// Surdimensionnement nombre pieces
const int t_max = 16;							// Taille max tableau générations Bierwith
const int nb_max_population = 100;				// Taille population
const int taille_pop_elite = 10;				// taille population élite
const int nb_max_iteration_RL = 200;			// Nombre itérations recherche locale
const int nb_max_generation = 200;				// Nombre max générations dans la population
const int K = 99999;							// taille table de hashage
const int nb_gene_avant_extermination = 100;	// Nombre génération constante avec extermination
const int infini = 999999;

extern int signature[K];

typedef struct prob {
	int nb_machine;
    int nb_piece;
	int mach[nb_max_pieces + 1][nb_max_machines + 1];
	int mach_inv[nb_max_pieces + 1][nb_max_machines + 1];
	int duree[nb_max_pieces + 1][nb_max_machines + 1];
	int duree_inv[nb_max_pieces + 1][nb_max_machines + 1];
	int T[nb_max_pieces+1][nb_max_machines+1];
}t_probleme;

typedef struct solu{
	int longueur;
	int makespan;
	int Bierwirth[nb_max_pieces*nb_max_machines + 1];
	int ES[nb_max_pieces*nb_max_machines + 1];
	int pere[nb_max_pieces*nb_max_machines + 1];
}t_solution;

typedef struct population{
	int nbIndividu = nb_max_population;
	std::vector<t_solution> liste;
}t_population;


/*============================================*/
/*				PROTOTYPES					  */
/*============================================*/
void lire_fichier(string nom_fichier, t_probleme & probleme);
void inverser_duree(t_probleme & probleme);
void inverser_machine(t_probleme & probleme);
void initT(t_probleme &probleme);

void generer_vect_alea(t_probleme &probleme, t_solution &solution);
void evaluer(t_probleme & probleme, t_solution & solution);
void ajout_pere_init_makespen(t_solution &solution, t_probleme &sprobleme);
void rechercheLocal(t_probleme & probleme, t_solution & solution);

bool testerDouble(t_solution & solution1);
int hashage(t_solution & solution);
void genererPopulationAlea(t_population & population, t_probleme & probleme, t_population & elite);
void selection_population_elite(t_population &population, t_population & elite);
void croisement(t_probleme &probleme, t_solution &parent1, t_solution &parent2, t_solution &enfant);
void algoGenetique(t_probleme & probleme, t_population & population, t_population & elite);
void tuer_population_faible(t_probleme & probleme, t_population & population);

bool sortByMakeSpan(t_solution &lhs, t_solution &rhs);

void afficher_probleme(t_probleme probleme, string nom_fichier);
void afficher_Pinv(t_probleme probleme);
void afficher_machine_inv(t_probleme probleme);
void afficher_solution(t_solution solution);
void afficher_resultat(t_population pop, double tps_ecoule);
void afficher_intro();

#endif // !BIERWITH


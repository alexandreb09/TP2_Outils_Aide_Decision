#pragma once


#ifndef BIERWITH
#define BIERWITH

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <cstdlib>
#include <iostream>
#include <ctime>
#include <vector>


using namespace std;


const int nb_max_machines = 20;
const int nb_max_pieces = 20;
const int t_max = 16;
const int max = 10;
const int k = 9999;

typedef struct prob {
	int nb_machine;
    int nb_piece;
	int mach[nb_max_pieces + 1][nb_max_machines + 1];
	int mach_inv[nb_max_pieces + 1][nb_max_machines + 1];
	int duree[nb_max_pieces + 1][nb_max_machines + 1];
	int duree_inv[nb_max_pieces + 1][nb_max_machines + 1];
	int T[nb_max_pieces+1][nb_max_machines+1];
}t_probleme;

typedef struct solu {
	int longueur;
	int makespan;
	int Bierwirth[nb_max_pieces*nb_max_machines + 1];
	int ES[nb_max_pieces*nb_max_machines + 1];
	int pere[nb_max_pieces*nb_max_machines + 1];
	
}t_solution;

typedef struct population
{ 
	int nbIndividu = max;
	t_solution liste[max];
	int signature[k];

}t_population;

void lire_fichier(string nom_fichier, t_probleme & probleme);

void generer_vect_alea(t_probleme &probleme, t_solution &solution);
void inverser_duree(t_probleme & probleme);
void inverser_machine(t_probleme & probleme);

void evaluer(t_probleme & probleme, t_solution & solution);
void Ajout_PereInit_Makespen(t_solution &solution, t_probleme &sprobleme);
void rechercheLocal(t_probleme & probleme, t_solution & solution, int nbIteration);

void afficher(t_probleme probleme);
void afficher_Pinv(t_probleme probleme);
void afficher_machine_inv(t_probleme probleme);
void afficher_solution(t_solution solution);
void afficherIntro();

bool testerDouble(t_population & population, t_solution & solution1);
int hashage(t_population & population, t_solution & solution1);
void genererPopulationAlea(t_population & population, t_probleme & probleme, t_solution & solution, int nb);
void selectionBestIndividus(t_population &population, int nb, t_population &populationNouvelle);
void croisement(t_probleme &probleme, t_solution &parent1, t_solution &parent2, t_solution &enfant);
void algoGenetique(t_probleme & probleme, t_solution & solution, t_population & generationInitiale, t_population & elite, int nbGeneration, int nbIterRechLocale, int nbIndividuSelection );
#endif // !BIERWITH

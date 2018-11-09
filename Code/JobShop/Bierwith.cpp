#include "Bierwith.h"

using namespace std;

int signature[K] = { 0 };


void lire_fichier(string nom_fichier, t_probleme & probleme){
	ifstream mon_fichier(nom_fichier);
	
	if (mon_fichier) {									// Si lecture OK
		string ligne;
		getline(mon_fichier, ligne);
		istringstream iss(ligne);
		iss >> probleme.nb_piece;
		iss >> probleme.nb_machine;

		int valeur1 , valeur2;
		for (int i = 1; i <= probleme.nb_piece; i++) {
			for (int j = 1; j <= probleme.nb_machine; j++) {
				mon_fichier >> valeur1;
				valeur1++;
				mon_fichier >> valeur2;
				probleme.mach[i][j] = valeur1;
				probleme.duree[i][j] = valeur2;
			}
		}
	}
	else {
		cout << "Lecture echouee !" << endl;
	}
}


void generer_vect_alea(t_probleme & probleme, t_solution & solution) {
	const int nb_elem_V = probleme.nb_machine*probleme.nb_piece;  // 9;
	const int nb_max_num_V = probleme.nb_machine; // 3;
	int max = probleme.nb_piece,i;
	int tab1_pt[t_max + 1];
	int tab2_pt[t_max + 1];
	
	for (int i = 1; i <= probleme.nb_piece; i++) {		// Initialisation tableau tab1 et tab2
		tab1_pt[i] = probleme.nb_machine;
		tab2_pt[i] = i;
	}

	for (int j = 1; j <= nb_elem_V; j++) {			// Remplissage vecteur Bierwith
		i = rand() % (max) + 1;
		tab1_pt[i] --;
		solution.Bierwirth[j] = tab2_pt[i];
		if (tab1_pt[i] == 0) {
			tab1_pt[i] = tab1_pt[max];
			tab2_pt[i] = tab2_pt[max];
			max--;
		}
	}

	/*
	cout << endl << "Vecteur bierwith :" << endl;		// affichage tableau Bierwith
	for (int k = 1; k <= nb_elem_V; k++) {
		cout << solution.Bierwirth[k] << " ";
	}
	cout << endl;
	*/
	
}



void evaluer(t_probleme & probleme, t_solution & solution) {
	int NP[nb_max_machines];
	int m[nb_max_machines];
	int prec, date;

	/*   INITIALISATION		*/	
	solution.longueur = probleme.nb_machine * probleme.nb_piece;
	solution.makespan = 0;
	for (int i = 1; i < nb_max_machines; i++){
		m[i] = 0;
		NP[i] = 0;
	}
	for (int i = 1; i <= solution.longueur; i++) {
		solution.ES[i] = 0;
		solution.pere[i] = 0;
	}

	int cpt = 1;
	for (int i = 1; i <= probleme.nb_piece; i++) {
		for (int k = 1; k <= probleme.nb_machine; k++) {
			probleme.T[i][k] = cpt;
			cpt++;
		}
	}
		
	/*		RESOLUTION		*/	
	for (int i = 1; i <= probleme.nb_machine*probleme.nb_piece; i++) {
		int k = solution.Bierwirth[i];
		NP[k]++;
		int mach = probleme.mach[k][NP[k]];
		if (NP[k] > 1) {
			prec = solution.ES[probleme.T[k][NP[k]-1] ];
			date = prec + probleme.duree[k][NP[k] - 1];
			if (date > solution.ES[probleme.T[k][NP[k]]]) {
				solution.ES[probleme.T[k][NP[k]] ] = date;
				solution.pere[probleme.T[k][NP[k]]] = probleme.T[k][NP[k] - 1];
			}
		}
		if (m[mach] > 0){
			int piece_prec = m[mach];
			int rang = probleme.mach_inv[piece_prec][mach];
			int posi_prec = probleme.T[piece_prec][rang];
			prec = solution.ES[posi_prec];
			int date_prec = probleme.duree[piece_prec][rang];
			if (prec + date_prec > solution.ES[probleme.T[k][NP[k]]]){
				solution.ES[probleme.T[k][NP[k]]] = prec + date_prec;
				solution.pere[probleme.T[k][NP[k]]] = posi_prec;
			}
		}
		m[mach] = k;
	}
	Ajout_PereInit_Makespen(solution, probleme);
	// afficher_solution(solution);
}

void inverser_duree(t_probleme & probleme){
	int machine;
	for (int ligne = 1; ligne <= probleme.nb_machine; ligne++) {
		for (int colonne = 1; colonne <= probleme.nb_machine; colonne++) {
			machine = probleme.mach[ligne][colonne];
			probleme.duree_inv[ligne][machine] = probleme.duree[ligne][colonne];
		}
	}
}

void inverser_machine(t_probleme & probleme) {
	int machine;
	for (int ligne = 1; ligne <= probleme.nb_piece; ligne++) {
		for (int colonne = 1; colonne <= probleme.nb_machine; colonne++) {
			probleme.mach_inv[ligne][probleme.mach[ligne][colonne]] =  colonne;
		}
	}
}

void Ajout_PereInit_Makespen(t_solution &solution, t_probleme &probleme) {
	int num_mach = probleme.nb_machine;
	for (int num_piece = 1; num_piece <= probleme.nb_piece; num_piece++) {
		if (solution.makespan < solution.ES[probleme.T[num_piece][num_mach]] + probleme.duree[num_piece][num_mach]) {
			solution.makespan = solution.ES[probleme.T[num_piece][num_mach]] + probleme.duree[num_piece][num_mach];
			solution.pere[solution.longueur + 1] = probleme.T[num_piece][num_mach];
		}
	}
}



/*============================================*/
/*				AFFICHAGE					  */
/*============================================*/

void afficher(t_probleme probleme) {
	cout << endl << "Probleme : " << endl << "Nb pieces : " << probleme.nb_piece << endl << "Nb machines : " << probleme.nb_machine << endl;
	for (int i = 1; i <= probleme.nb_piece; i++) {
		for (int j = 1; j <= probleme.nb_machine; j++) {
			cout << probleme.mach[i][j] << " " << probleme.duree[i][j] << " ";
		}
		cout << endl;
	}
}

void afficher_Pinv(t_probleme probleme) {
	cout << endl << "P inv : " << endl;
	for (int ligne = 1; ligne <= probleme.nb_machine; ligne++) {
		for (int colonne = 1; colonne <= probleme.nb_machine; colonne++) {
			cout << probleme.duree_inv[ligne][colonne] << " ";
		}
		cout << endl;
	}
}

void afficher_machine_inv(t_probleme probleme) {
	cout << endl << "M inv : " << endl;
	for (int ligne = 1; ligne <= probleme.nb_machine; ligne++) {
		for (int colonne = 1; colonne <= probleme.nb_machine; colonne++) {
			cout << probleme.mach_inv[ligne][colonne] << " ";
		}
		cout << endl;
	}
}

void afficher_solution(t_solution solution) {
	cout << endl << "Vecteur ES : " << endl;
	for (int i = 1; i <= solution.longueur; i++) {
		cout << solution.ES[i] << " ";
	}
	cout << endl << "Vecteur Pere : " << endl;
	for (int i = 1; i <= solution.longueur +1; i++) {
		cout << solution.pere[i] << " ";
	}
	cout << endl << "makespan : " << solution.makespan << endl << "Chemin critique : ";
	int ind = solution.pere[solution.longueur+1];
	while (ind != 0) {
		cout << ind << " ";
		ind = solution.pere[ind];
	}
	cout << "0" << endl;
}

void afficher_intro() {
	cout << "   _______  _______  _______        _______  _    _  _______  _______ " << endl <<
		"  |__   __||   _   ||   _   |      |   ____|| |  | ||   _   ||   _   |" << endl <<
		"     | |   |  | |  ||  |_|  |      |  |___ || |  | ||  | |  ||  |_|  |" << endl <<
		"  _  | |   |  | |  ||   _   |      |____   || |__| ||  | |  ||   ____|" << endl <<
		" | |_| |   |  |_|  ||  |_|  |       ____|  ||  __  ||  |_|  ||  |     " << endl <<
		" |_____|   |_______||_______|      |_______||_|  |_||_______||__|     " << endl;
}

void rechercheLocal(t_probleme & probleme, t_solution & solution){
	evaluer(probleme, solution);

	int i = solution.pere[solution.longueur+1];		// Indice noeud côté fin arc disjonctif
	int j = solution.pere[i];						// Indice noeud côté debut arc disjonctif
	int compteurIteration = 1;						// Nb intéreation recherche lcoale
	int Pi, Pj;										// Position dans vecteur de bierwith
	int comp1 = 0, comp2 = 0;
	int k = 1;
	int l = 1;

	t_solution solution2 = solution;				// Nouvelle solution



	std::vector<int> APP;
	APP.push_back(-1);
	for (int i = 0; i < solution.longueur ; i++) {
		APP.push_back((int) i/probleme.nb_machine+1);
	}
	
	while (j != 0 && compteurIteration < nb_max_iteration_RL){
		compteurIteration++;
		/*
		cout << "Operation numero :" << i<< endl;
		cout << "Le pere de i est " << solution.pere[i] << endl;
		*/

		if (APP[i] != APP[j] ){
			int job_i = (i-1) / probleme.nb_machine + 1;
			int ordre_Op_i = i - (job_i-1) * probleme.nb_machine;

			int job_j = (j-1) / probleme.nb_machine + 1;
			int ordre_Op_j = j - (job_j-1) * probleme.nb_machine;

			/*
			cout <<"job1 "<< job1 << endl;
			cout <<"job2 " << job2 << endl;
			cout << "op1 " << ordreOp1 << endl;
			cout << "op2 " << ordreOp2 << endl;
			*/
			int stop = 0;
			while (k <= probleme.nb_machine*probleme.nb_piece && stop == 0){
				if ((solution.Bierwirth[k] == job_i)){
					comp1++;
					if (comp1 == ordre_Op_i){
						Pi = k;
						stop = 1;
					}
				}
				k++;
			}
			stop = 0;
			while ( l <= probleme.nb_machine*probleme.nb_piece && stop ==0 ){ 
				if (solution.Bierwirth[l] == job_j){
					comp2++;
					if (comp2 == ordre_Op_j){
						Pj = l;
						stop = 1;
					}
				}
				l++;
			}
			/*
			cout << "Indice1 est : "<<Pi << endl;
			cout << "Indice2 est : "<<Pj << endl;
			cout << "test" << endl<<endl;
			cout << solution2.Bierwirth[Pi] << endl;
			cout << solution2.Bierwirth[Pj] << endl;
			cout << endl << "Vecteur bierwith avant permutation :" << endl;		// affichage tableau Bierwith
			
			for (int t = 1; t <= probleme.nb_machine * probleme.nb_piece; t++) {
				cout << solution2.Bierwirth[t] << " ";
			}
			cout << endl;
			*/
			solution2 = solution;
			int tmp = solution2.Bierwirth[Pi];
			solution2.Bierwirth[Pi] = solution2.Bierwirth[Pj];
			solution2.Bierwirth[Pj] = tmp;
			/*
			cout << solution2.Bierwirth[Pi] << endl;
			cout << solution2.Bierwirth[Pj] << endl;
			*/

			/*
			cout << endl << "Vecteur bierwith apres permutation :" << endl;		// affichage tableau Bierwith
			for (int t = 1;t <= probleme.nb_machine * probleme.nb_piece; t++) {
				cout << solution2.Bierwirth[t] << " ";
			}
			cout << endl;
			*/
			evaluer(probleme, solution2);
			/*
			for (int t = 1; t <= probleme.nb_machine * probleme.nb_piece; t++) {
				cout << solution2.Bierwirth[t] << " ";
			}
						
			cout << endl << "Nouveau Makespan est : " << solution2.makespan << endl << endl;
			*/

			if (solution2.makespan < solution.makespan)	{
				solution = solution2;
				solution.pere[solution.longueur + 1];
				j = solution.pere[i];
				//cout << "NewBestMakespan est : " << solution.makespan << endl;
			}
			else{
				i = j;
				j = solution.pere[i]; 
				// cout << "valeur de j  1 est : " << j << endl;
			}
		}
		else {
			i = j;
			j = solution.pere[i];
			// cout << "valeur de j  2 est : " << j << endl;
		}
		/*
		cout << "compteurIteration++ : " << compteurIteration++ << endl; //tests
		cout << "valeur de j est : " << j << endl;
		*/
	}
}



int hashage(t_population & population, t_solution & solution){
	int h = 0;
	for (int i = 1; i <= solution.longueur; i++){
		h += (solution.ES[i]^2) %K;                   
	}
	return h;
}

void testerDouble(t_population & population, t_solution & solution, bool doublant){
	int h = hashage(population, solution);
	doublant = true;
	if (0 == signature[h])	{
		doublant = false;
		signature[h] = 1;
	}
}

void genererPopulationAlea(t_population & population, t_probleme & probleme){
	int i = 0, h;
	t_solution solution;
	while (i < population.nbIndividu){
		generer_vect_alea(probleme, solution);
		rechercheLocal(probleme, solution);
		
		bool doublant = false;
		testerDouble(population,solution, doublant);

		if (false == doublant){
			population.liste.push_back(solution);
			i++;
		}
	}

	std::sort(population.liste.begin(), population.liste.end(), sortByID);

	/*
	for (int k = 0; k < population.nbIndividu; k++){
		cout << (population.liste[k]).makespan << "  " ;
	}
	*/
}



void selection_population_elite(t_population &population, t_population& elite){
	elite.nbIndividu = taille_pop_elite;
	for (int i = 1; i < taille_pop_elite; i++){
		elite.liste[i] = population.liste[i];	
	}
}

void croisement(t_probleme& probleme, t_solution &parent1, t_solution &parent2, t_solution& enfant){
	int taille = probleme.nb_machine * probleme.nb_piece;
	int alea = ( rand() % taille ) + 1;
	/*
	cout << endl << "alea : " << alea << endl << "Vecteur bierwith :" << endl << "parent1 : ";		// affichage tableau Bierwith
	for (int k = 1; k <= taille; k++) {
		cout << parent1.Bierwirth[k] << " ";
	}
	cout << endl << "parent2 : ";
	for (int k = 1; k <= taille; k++) {
		cout << parent2.Bierwirth[k] << " ";
	}
	cout << endl;
	*/
	int nbApparitionJobs[nb_max_pieces] = {0};
	for (int i = 1; i <= alea; i++){
		enfant.Bierwirth[i] = parent1.Bierwirth[i];
		nbApparitionJobs[ parent1.Bierwirth[i] ] ++;
	}

	
	alea++;								// Variable parcours parent 2
	int nb_elem_fils = alea;			// var de parcours fils
	while( alea <= taille){
		if (nbApparitionJobs[parent2.Bierwirth[nb_elem_fils]] < probleme.nb_machine ) {
			enfant.Bierwirth[alea] = parent2.Bierwirth[nb_elem_fils];
			nbApparitionJobs[parent2.Bierwirth[nb_elem_fils]] ++;
			alea++;		
		}
		nb_elem_fils = nb_elem_fils % taille + 1;
	}

	/*
	cout << endl;
	for (int k = 1; k <= taille; k++) {
		cout << enfant.Bierwirth[k] << " ";
	}
	cout << endl;
	*/
}



void algoGenetique(t_probleme & probleme, t_population & population, t_population & elite){
	int k = 0, best_makespan = infini, compt = 0;
	int num_pere_elite;
	t_solution pere_mauvais, enfant, pere_bon;

	for (int i = 1; i <= taille_pop_elite; i++) {
		elite.liste.push_back(pere_mauvais);
	}

	genererPopulationAlea(population, probleme);

	while( k < nb_max_generation){
		selection_population_elite(population, elite);
	
		for (int i=taille_pop_elite; i < nb_max_population; i++){
			num_pere_elite = rand() % (taille_pop_elite-2) + 2;											// On exclut la meilleure solution

			pere_bon = elite.liste[num_pere_elite];
			croisement(probleme, pere_bon, population.liste[i], enfant);
			rechercheLocal(probleme, enfant);

			if (enfant.makespan <= population.liste[i].makespan) {
				population.liste[i] = enfant;
			}
		} 

		if (best_makespan > elite.liste[1].makespan) {
			best_makespan = elite.liste[1].makespan;
			compt = 0;
		}
		else compt++;

		if (compt > nb_gene_avant_extermination) {
			tuer_population_faible(probleme,population);
		}
		cout << "Makespan generation " << k << " : " << population.liste[1].makespan << " " << endl;
		k++;
	}
}


void tuer_population_faible(t_probleme &probleme, t_population & population) {
	for (int i = taille_pop_elite; i < nb_max_population; i++) {
		generer_vect_alea(probleme, population.liste[i]);
		rechercheLocal(probleme, population.liste[i]);
	}
}

bool sortByID(t_solution &lhs, t_solution &rhs) { return lhs.makespan < rhs.makespan; }
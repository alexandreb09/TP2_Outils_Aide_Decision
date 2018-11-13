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
		inverser_duree(probleme);
		inverser_machine(probleme);
		initT(probleme);
	}
	else {
		cout << "Lecture echouee !" << endl;
	}
}


void generer_vect_alea(t_probleme & probleme, t_solution & solution) {
	const int nb_elem_V = probleme.nb_machine*probleme.nb_piece;
	const int nb_max_num_V = probleme.nb_machine;
	int max = probleme.nb_piece,i;
	int tab1_pt[t_max + 1];
	int tab2_pt[t_max + 1];
	
	for (int i = 1; i <= probleme.nb_piece; i++) {		// Initialisation tableau tab1 et tab2
		tab1_pt[i] = probleme.nb_machine;
		tab2_pt[i] = i;
	}

	for (int j = 1; j <= nb_elem_V; j++) {				// Remplissage vecteur Bierwith
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
	ajout_pere_init_makespen(solution, probleme);
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
	for (int ligne = 1; ligne <= probleme.nb_piece; ligne++) {
		for (int colonne = 1; colonne <= probleme.nb_machine; colonne++) {
			probleme.mach_inv[ligne][probleme.mach[ligne][colonne]] =  colonne;
		}
	}
}

void initT(t_probleme &probleme) {
	int cpt = 1;
	for (int i = 1; i <= probleme.nb_piece; i++) {
		for (int k = 1; k <= probleme.nb_machine; k++) {
			probleme.T[i][k] = cpt;
			cpt++;
		}
	}
}

void ajout_pere_init_makespen(t_solution &solution, t_probleme &probleme) {
	int num_mach = probleme.nb_machine;
	for (int num_piece = 1; num_piece <= probleme.nb_piece; num_piece++) {
		if (solution.makespan < solution.ES[probleme.T[num_piece][num_mach]] + probleme.duree[num_piece][num_mach]) {
			solution.makespan = solution.ES[probleme.T[num_piece][num_mach]] + probleme.duree[num_piece][num_mach];
			solution.pere[solution.longueur + 1] = probleme.T[num_piece][num_mach];
		}
	}
}



void rechercheLocal(t_probleme & probleme, t_solution & solution){
	evaluer(probleme, solution);

	int i = solution.pere[solution.longueur+1];									// Indice noeud c�t� fin arc disjonctif
	int j = solution.pere[i];													// Indice noeud c�t� debut arc disjonctif
	int compteurIteration = 1;													// Nb int�reation recherche lcoale
	int Pi, Pj;																	// Position dans vecteur de bierwith
	int comp1 = 0, comp2 = 0;
	int k = 1;
	int l = 1;

	t_solution solution2 = solution;											// Nouvelle solution



	std::vector<int> APP;
	APP.push_back(-1);
	for (int i = 0; i < solution.longueur ; i++) {
		APP.push_back((int) i/probleme.nb_machine+1);
	}
	
	while (j != 0 && compteurIteration < nb_max_iteration_RL){
		compteurIteration++;

		if (APP[i] != APP[j] ){													// Si on se trouve sur un arc disjonctif
			int job_i = (i-1) / probleme.nb_machine + 1;						// Indice job_i
			int ordre_Op_i = i - (job_i-1) * probleme.nb_machine;				// Indice ordre_op i

			int job_j = (j-1) / probleme.nb_machine + 1;
			int ordre_Op_j = j - (job_j-1) * probleme.nb_machine;

			/*
			cout <<"job1 "<< job1 << endl;
			cout <<"job2 " << job2 << endl;
			cout << "op1 " << ordreOp1 << endl;
			cout << "op2 " << ordreOp2 << endl;
			*/
			bool stop = false;
			while (k <= probleme.nb_machine*probleme.nb_piece && !stop){
				if ((solution.Bierwirth[k] == job_i)){
					comp1++;
					if (comp1 == ordre_Op_i){
						Pi = k;
						stop = true;
					}
				}
				k++;
			}
			stop = false;
			while ( l <= probleme.nb_machine*probleme.nb_piece && !stop ){ 
				if (solution.Bierwirth[l] == job_j){
					comp2++;
					if (comp2 == ordre_Op_j){
						Pj = l;
						stop = true;
					}
				}
				l++;
			}

			solution2 = solution;												// Copie vecteur Bierwith
			int tmp = solution2.Bierwirth[Pi];									// Echange Pi - Pj
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
			evaluer(probleme, solution2);										// Evalutaion nouvelle solution

			if (solution2.makespan < solution.makespan)	{						// Si nouvelle solution meilleure
				solution = solution2;											// On garde la nouvelle solution
				i = solution.pere[solution.longueur + 1];						// MAJ sommets de parcours chemin critique
				j = solution.pere[i];
				//cout << "NewBestMakespan est : " << solution.makespan << endl;
			}
			else{																// Si la nouvelle solution n'est pas meilleure
				i = j;															// On remonte le chemin critique
				j = solution.pere[i]; 
				// cout << "j : " << j << endl;
			}
		}
		else {
			i = j;
			j = solution.pere[i];
			// cout << "j : " << j << endl;
		}
	}
}



int hashage(t_solution & solution){													// Calcul la signature de la solution
	int h = 0;
	for (int i = 1; i <= solution.longueur; i++){
		h += (solution.ES[i]^2);                   
	}
	return h % K;
}

bool testerDouble(t_solution & solution){											// V�rifie que la solution n'a pas d�j� �t� test��
	int h = hashage(solution);
	bool identique = true;
	if (0 == signature[h])	{
		identique = false;
		signature[h] = 1;
	}
	return identique;
}

void genererPopulationAlea(t_population & population, t_probleme & probleme, t_population & elite){		// Cr�e une population al�atoirement
	t_solution solution;

	int i = 0;
	while (i < population.nbIndividu){												// Pour chaque individu
		generer_vect_alea(probleme, solution);										// G�n�ration d'un vecteur de Bierwith
		rechercheLocal(probleme, solution);											// Evaluation de ce vecteur
		
		if (false == testerDouble(solution)){										// Si solution nouvelle
			population.liste.push_back(solution);									// Ajout de cette solution dans la liste
			i++;
		}
	}

	std::sort(population.liste.begin(), population.liste.end(), sortByMakeSpan);	// Tri de la population par makespan

	for (int i = 1; i <= taille_pop_elite; i++) {									// Initialisation population �lite
		elite.liste.push_back(population.liste[i]);
	}
	/*
	for (int k = 0; k < population.nbIndividu; k++){
		cout << (population.liste[k]).makespan << "  " ;
	}
	*/
}



void selection_population_elite(t_population &population, t_population& elite){			// Selectionne la population �lite
	std::sort(population.liste.begin(), population.liste.end(), sortByMakeSpan);		// Tri la population
	elite.nbIndividu = taille_pop_elite;
	for (int i = 0; i < taille_pop_elite; i++){											// Selectionne les individus
		elite.liste[i] = population.liste[i];	
	}
}

void croisement(t_probleme& probleme, t_solution &parent1, t_solution &parent2, t_solution& enfant){		// Effectue le croisement entre deux parents
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
	int nbApparitionJobs[nb_max_pieces] = {0};									// Selectionne les alea valeurs du premier parent
	for (int i = 1; i <= alea; i++){
		enfant.Bierwirth[i] = parent1.Bierwirth[i];
		nbApparitionJobs[ parent1.Bierwirth[i] ] ++;
	}

	
	alea++;																		// Variable parcours parent 2
	int nb_elem_fils = alea;													// var de parcours fils
	while( alea <= taille){														// Compl�te le vecteur fils avec celle du 2nd parent
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
	int nombre_generation = 0, best_makespan = infini, compt = 0;
	int nombre_extermination = 0;
	int num_pere_elite;
	t_solution enfant, pere_bon;
	t_population new_pop;

	genererPopulationAlea(population, probleme, elite);										// G�n�ration population al�atoire
	new_pop = population;

	while(nombre_generation < nb_max_generation){											// Pour chaque g�n�ration
		population = new_pop;
		selection_population_elite(population, elite);										// D�finition population �lite

		for (int i=taille_pop_elite; i < nb_max_population; i++){							// Pour chaque individu "mauvais"
			num_pere_elite = rand() % (taille_pop_elite-2) + 2;								// On tire al�atoirement un p�re bon
			pere_bon = elite.liste[num_pere_elite];

			croisement(probleme, pere_bon, population.liste[i], enfant);					// On fait le croisement avec le pere mauvais
			rechercheLocal(probleme, enfant);												// On regarde le makespan de l'enfant cr��
						
			if ((rand() % 2) > 0) {															
				new_pop.liste[i] = enfant;
			}
			else{
				new_pop.liste[num_pere_elite] = enfant;
			}
		} 

		if (best_makespan > elite.liste[1].makespan) {										// Si le best makespan de la pop s'est am�lior�
			best_makespan = elite.liste[1].makespan;										// MAJ best makespan
			compt = 0;																		// R�initialise le compteur
		}
		else compt++;																		// Sinon incr�mentation compteur

		if (compt > nb_gene_avant_extermination) {											// Si makespan constant sur x g�n�rations
			tuer_population_faible(probleme,population);									// On tue tous les parents mauvais
			nombre_extermination++;
		}
		nombre_generation++;																// MAJ nombre g�n�ration
	}
}


void tuer_population_faible(t_probleme &probleme, t_population & population) {				// Tue l'ensemble de la population dite faible
	for (int i = taille_pop_elite; i < nb_max_population; i++) {							// Pour chaque individu de la population faible
		generer_vect_alea(probleme, population.liste[i]);									// On reg�n�re un vecteur Bierwith
		rechercheLocal(probleme, population.liste[i]);										// qui est �valu�
	}
}

bool sortByMakeSpan(t_solution &lhs, t_solution &rhs) { return lhs.makespan < rhs.makespan; }	// Tri par makespan





/*============================================*/
/*				AFFICHAGE					  */
/*============================================*/

void afficher_probleme(t_probleme probleme, string nom_fichier) {
	cout << endl << "Probleme : " << nom_fichier << endl << "Nb pieces : " << probleme.nb_piece << endl << "Nb machines : " << probleme.nb_machine << endl;
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
	for (int i = 1; i <= solution.longueur + 1; i++) {
		cout << solution.pere[i] << " ";
	}
	cout << endl << "makespan : " << solution.makespan << endl << "Chemin critique : ";
	int ind = solution.pere[solution.longueur + 1];
	while (ind != 0) {
		cout << ind << " ";
		ind = solution.pere[ind];
	}
	cout << "0" << endl;
}

void afficher_resultat(t_population elite, double tps_ecoule) {
	cout << endl << "Apres " << nb_max_generation << " generations, makespan : " << elite.liste[0].makespan << endl;
	cout << "Duree execution : " << tps_ecoule << endl;
}

void afficher_intro() {
	cout << "   _______  _______  _______        _______  _    _  _______  _______ " << endl <<
		"  |__   __||   _   ||   _   |      |   ____|| |  | ||   _   ||   _   |" << endl <<
		"     | |   |  | |  ||  |_|  |      |  |___ || |  | ||  | |  ||  |_|  |" << endl <<
		"  _  | |   |  | |  ||   _   |      |____   || |__| ||  | |  ||   ____|" << endl <<
		" | |_| |   |  |_|  ||  |_|  |       ____|  ||  __  ||  |_|  ||  |     " << endl <<
		" |_____|   |_______||_______|      |_______||_|  |_||_______||__|     " << endl;
}

#include "Bierwith.h"

using namespace std;

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

	
	cout << endl << "Vecteur bierwith :" << endl;		// affichage tableau Bierwith
	for (int k = 1; k <= nb_elem_V; k++) {
		cout << solution.Bierwirth[k] << " ";
	}
	cout << endl;
	
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
/*				AFFICHER					  */
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

void afficherIntro() {
	cout << "   _______  _______  _______        _______  _    _  _______  _______ " << endl <<
		"  |__   __||   _   ||   _   |      |   ____|| |  | ||   _   ||   _   |" << endl <<
		"     | |   |  | |  ||  |_|  |      |  |___ || |  | ||  | |  ||  |_|  |" << endl <<
		"  _  | |   |  | |  ||   _   |      |____   || |__| ||  | |  ||   ____|" << endl <<
		" | |_| |   |  |_|  ||  |_|  |       ____|  ||  __  ||  |_|  ||  |     " << endl <<
		" |_____|   |_______||_______|      |_______||_|  |_||_______||__|     " << endl;
}

void rechercheLocal(t_probleme & probleme, t_solution & solution, int nbIteration_max){
	evaluer(probleme, solution);
	cout << "Makespan avant optimisation :" << solution.makespan << endl;

	int i = solution.pere[solution.longueur+1];
	int j = solution.pere[i];
	int compteurIteration = 1;
	int Pi, Pj;										//position dans vecteur de bierwith

	t_solution solution2 = solution;

	int comp1 = 0;
	int comp2 = 0;
	int k = 1;
	int l = 1;

	std::vector<int> APP;
	APP.push_back(-1);
	for (int i = 0; i < solution.longueur ; i++) {
		APP.push_back((int) i/probleme.nb_machine+1);
	}


	while (j != 0 && compteurIteration < nbIteration_max){
		compteurIteration++;
		/*
		cout << "Operation numero :" << i<< endl;
		cout << "Le pere de i est " << solution.pere[i] << endl;
		*/

		if (APP[i] != APP[j] ){
			/*
			int job1;
			int ordreOp1 = i % probleme.nb_machine + 1;
			int job2;
			int ordreOp2 = i % probleme.nb_machine + 1;

			
			if (i <= probleme.nb_machine){
				job1 = 1;
				ordreOp1 = i;
			}
			else if ((i % probleme.nb_machine != 0)){
				job1 = (i / probleme.nb_machine ) + 1;
				ordreOp1 = i % probleme.nb_machine;
			}
			else {
				job1 = (i / probleme.nb_machine);
			}
			if (j <= probleme.nb_machine){
				job2 = 1;
				ordreOp2 = j;
			}
			else if ((j % probleme.nb_machine != 0)){
				job2 = (j / probleme.nb_machine) + 1;
				ordreOp2 = j % probleme.nb_machine ;
			}
			else {
				job2= (j / probleme.nb_machine);
			}
			*/
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
			cout << "test" << endl;
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
			
			cout <<"evaluer2"<< endl;
			*/
			evaluer(probleme, solution2);
			/*
			for (int t = 1; t <= probleme.nb_machine * probleme.nb_piece; t++) {
				cout << solution2.Bierwirth[t] << " ";
			}
			
			cout << endl;
			
			cout << "Nouveau Makespan est : " << solution2.makespan << endl;
			cout << endl;
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

void hashage(t_population & population, t_solution & solution1, int h, int k)
{
	
	for (int i = 1; i <= solution1.longueur; i++)
	{
		h += (solution1.ES[i]^2)%k;                   
	}

}
void testerDouble(t_population & population, t_solution & solution1, bool doublant, int h, int k)
{
	hashage(population, solution1, h, k);
	if (population.signature[h] == 1)
	{
		doublant = true;
	}
	else
	{
		doublant = false;
		population.signature[h] = 1;
	}

}

void genererPopulationAlea(t_population & population, t_probleme & probleme, t_solution & solution, int nb, int k, int h )
{
	int i = 0;
	while (i < population.nbIndividu)
	{
		t_solution nouvelleSolution;
		generer_vect_alea(probleme, solution);
		//evaluer(probleme, solution);
		rechercheLocal(probleme, solution, nb);
		nouvelleSolution = solution;
		bool doublant = false;
		hashage(population, nouvelleSolution , h, k);
		testerDouble(population,nouvelleSolution, doublant, h,k);

		
		if (doublant == false)
		{
			population.liste[i] = nouvelleSolution;
		
			if (i != 0 && population.liste[i].makespan < population.liste[i - 1].makespan)
			{
				t_solution tmp = population.liste[i - 1];
				population.liste[i - 1] = population.liste[i];
				population.liste[i] = tmp;

			}
			i++;
			cout << "Best makespan de la " << i  << "eme solution est " << nouvelleSolution.makespan << endl;

			cout << endl;
		}
		


		
		
	}
	for (int k = 0; k < population.nbIndividu; k++)
	{
		cout << (population.liste[k]).makespan << "  " ;
	}
}



void selectionBestIndividus(t_population &population, int nb, t_population& populationNouvelle)
{
	for (int i = 0; i < nb; i++)
	{
		populationNouvelle.liste[i] = population.liste[i];
	}

}

void croisement(t_probleme& probleme, t_solution &parent1, t_solution &parent2, t_solution& enfant)
{
	int c = probleme.nb_machine * probleme.nb_piece;
	int alea = ( rand() % c ) + 1;
	cout << endl << "Vecteur bierwith :" << endl;		// affichage tableau Bierwith
	for (int k = 1; k <= probleme.nb_machine*probleme.nb_piece; k++) {
		cout << parent1.Bierwirth[k] << " ";
	}
	
	cout << endl;
	int nbApparitionJobs[nb_max_pieces] = {0};
	for (int i = 1; i <= alea; i++) 
	{
		enfant.Bierwirth[i] = parent1.Bierwirth[i];
		nbApparitionJobs[ parent1.Bierwirth[i] ] ++;
		
	}
	for (int i = 1; i <= probleme.nb_piece; i++)
	{
		cout << nbApparitionJobs[i] << " ";
	}
	cout << "fin";
	cout << endl;
	
	int i = alea + 1;
	int j = 1;           // var de parcours du 2eme parent
	while( i <= (probleme.nb_machine*probleme.nb_piece) )
	{
		if ((nbApparitionJobs[parent2.Bierwirth[j]] < probleme.nb_machine )) 
		{
			enfant.Bierwirth[i] = parent2.Bierwirth[j];
			nbApparitionJobs[parent2.Bierwirth[j]] ++;
			i++;
			j++;
			
		}
		else
		{
			j++;
		}
		
	}
	for (int i = 1; i <= probleme.nb_piece; i++)
	{
		cout << nbApparitionJobs[i] << " ";
	}

	cout << "fin";
	
	/*for (int i = 1; i <= probleme.nb_piece; i++)
	{
		cout << nbApparitionJobs[i] << " ";
	}

	cout << "fin";
	

	cout << endl << "Vecteur bierwith :" << endl;		// affichage tableau Bierwith
	for (int k = 1; k <= probleme.nb_machine*probleme.nb_piece; k++) {
		cout << parent1.Bierwirth[k] << " ";
	}
	cout << endl;

	cout << endl << "Vecteur bierwith :" << endl;		// affichage tableau Bierwith
	for (int k = 1; k <= probleme.nb_machine*probleme.nb_piece; k++) {
		cout << parent2.Bierwirth[k] << " " ;
	}
	cout << endl;

	cout << endl << "Vecteur bierwith :" << endl;		// affichage tableau Bierwith
	for (int k = 1; k <= probleme.nb_machine*probleme.nb_piece; k++) {
		cout << enfant.Bierwirth[k] << " ";
	}
	cout << endl; */

}



void algoGenetique(t_probleme & probleme, t_solution & solution, t_population & generationInitiale, t_population & elite, int nbGeneration, int nbIterRechLocale, int nbIndividuSelection, int k, int h)
{
	
	while( k< nbGeneration)
	{
		genererPopulationAlea(generationInitiale, probleme, solution, nbIterRechLocale, k, h);
		selectionBestIndividus(generationInitiale, nbIndividuSelection, elite);
		t_population nouvelleGenerationParCroissement;
		t_solution enfant1;
	
		for (int i=0; i < elite.nbIndividu; i++)
		{
			croisement(probleme, elite.liste[i], elite.liste[i + 1], enfant1);
			rechercheLocal(probleme, enfant1, nbIterRechLocale);
			if ((enfant1.makespan > elite.liste[i].makespan) && (enfant1.makespan > elite.liste[i+1].makespan))
			{
				t_solution worst = elite.liste[i];
				if ( worst.makespan > elite.liste[i + 1].makespan )
				{
					worst = elite.liste[i + 1];
					worst = enfant1;							// on remplace le pire parent par le fils
				}
							  
															// croisement reussi
			}
			else if ((enfant1.makespan > elite.liste[i].makespan))
			{
				elite.liste[i] = enfant1;
							

			}
			else if((enfant1.makespan > elite.liste[i+1].makespan) )
			{
				elite.liste[i+1] = enfant1;
							
			}
		} 

		k++;

		
	}
	
}

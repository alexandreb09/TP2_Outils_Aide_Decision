#include "Bierwith.h"


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
	int max = nb_max_num_V, i;
	int tab1_pt[t_max + 1];
	int tab2_pt[t_max + 1];

	srand(123456789);					// Fixe aléatoire

	for (int i = 1; i <= probleme.nb_machine; i++) {		// Initialisation tableau tab1 et tab2
		tab1_pt[i] = probleme.nb_piece;
		tab2_pt[i] = i;
	}

	for (int j = 1; j <= nb_elem_V; j++) {			// Remplissage vecteur Bierwith
		// i = rand() % max + 1;
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


	afficher_solution(solution);
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
	for (int ligne = 1; ligne <= probleme.nb_machine; ligne++) {
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
	cout << endl << "Probleme : " << endl << "Nb pièces : " << probleme.nb_piece << endl << "Nb machines : " << probleme.nb_machine << endl;
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

void rechercheLocal(t_probleme & probleme, t_solution & solution, int nbIteration)
{
	int compteurIteration = 1;
	t_solution solution2 = solution;
	evaluer(probleme, solution);
	int i = compteurIteration;
	int j = solution.pere[i];
	int Pi ; //position danc vecteur de bierwith
	int Pj ;
	int comp1 = 0;
	int k = 1;
	int comp2 = 0;
	int l = 1;
	

	while (j != 0 && compteurIteration < nbIteration)
	{
		cout << "Operation numero :" << i<< endl;
		cout << "Le pere de i est " << solution.pere[i] << endl;

		if (abs(i - j) > probleme.nb_machine - 1)
		{
			int job1;
			int ordreOp1 = i % probleme.nb_machine + 1;
			int job2;
			int ordreOp2 = i % probleme.nb_machine + 1;

			if (i <= probleme.nb_machine)
			{
				job1 = 1;
				ordreOp1 = i;
			}
			else if ((i % probleme.nb_machine != 0))
			{
				job1 = (i / probleme.nb_machine ) + 1;
				ordreOp1 = i % probleme.nb_machine;
			}
			else
			{
				job1 = (i / probleme.nb_machine);
			}
			if (j <= probleme.nb_machine)
			{
				job2 = 1;
				ordreOp2 = j;
			}
			else if ((j % probleme.nb_machine != 0))
			{
				job2 = (j / probleme.nb_machine) + 1;
				ordreOp2 = j % probleme.nb_machine ;
			}
			else
			{
				job2= (j / probleme.nb_machine);
			}
			cout <<"job1 "<< job1 << endl;
			cout <<"job2 " << job2 << endl;
			cout << "op1 " << ordreOp1 << endl;
			cout << "op2 " << ordreOp2 << endl;
			int stop1 = 0;
			while (k <= probleme.nb_machine*probleme.nb_piece && stop1 == 0)
			{
				if ((solution.Bierwirth[k] == job1))
				{
					comp1++;
					if (comp1 == ordreOp1)
					{
						Pi = k;
						stop1 = 1;
					}
				}
				k++;
			}
			int stop2 = 0;
			while ( l <= probleme.nb_machine*probleme.nb_piece && stop2 ==0 )			
			{ 
				if ((solution.Bierwirth[l] == job2))
				{
					comp2++;
					if (comp2 == ordreOp2)
					{
						Pj = l;
						stop2 = 1;
					}
				}
				l++;
			}
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
			solution2.Bierwirth[Pi] = job2;
			solution2.Bierwirth[Pj] = job1;
			cout << solution2.Bierwirth[Pi] << endl;
			cout << solution2.Bierwirth[Pj] << endl;

			cout << endl << "Vecteur bierwith apres permutation :" << endl;		// affichage tableau Bierwith
			for (int t = 1;t <= probleme.nb_machine * probleme.nb_piece; t++) {
				cout << solution2.Bierwirth[t] << " ";
			}
			cout << endl;
			int bestMakespan = solution.makespan;
			cout <<"evaluer2"<< endl;
			evaluer(probleme, solution2);
			for (int t = 1; t <= probleme.nb_machine * probleme.nb_piece; t++) {
				cout << solution2.Bierwirth[t] << " ";
			}
			cout << endl;
			cout << "Nouveau Makespan est : " << solution2.makespan << endl;
			cout << endl;
			if (solution2.makespan < bestMakespan)
			{
				solution = solution2;
				i = compteurIteration;
				j = solution.pere[i];
				cout << "NewBestMakespan est : " << solution.makespan << endl;

			}
			else
			{
				i = j;
				j = solution.pere[i]; cout << "valeur de j  1 est : " << j << endl;
			}
		}
		else
		{
			i = j;
			j = solution.pere[i];
			cout << "valeur de j  2 est : " << j << endl;
		}
		
		cout << "compteurIteration++ : " << compteurIteration++ << endl; //tests
		cout << "valeur de j est : " << j << endl;
	}
	compteurIteration++;
}

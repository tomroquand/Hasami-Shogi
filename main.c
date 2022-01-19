#include <stdio.h>
#include <stdlib.h>

void rafraichirEcran()
{
	puts("");
	system("clear"); // ce code casse la portabilité windows
}

// Résultats:
//	plateau : Tableau de caractères {Le plateau de jeu qu'on remplit}
// Locales:
//	i, j : Entiers {Compteurs de boucle, i représente la ligne et j la colonne}
void creerPlateau(char plateau[9][9])
{
	// Remplissage N
	int i;
	for (i = 0; i < 2; i++)
	{
		int j;
		for (j = 0; j < 9; j++)
			plateau[i][j] = 'N';
	}

	// Remplissage points
	for (i = 2; i < 7; i++)
	{
		int j;
		for (j = 0; j < 9; j++)
			plateau[i][j] = '.';
	}

	// Remplissage blancs
	for (i = 7; i < 9; i++)
	{
		int j;
		for (j = 0; j < 9; j++)
			plateau[i][j] = 'B';
	}
}

// Données:
//	plateau : Tableau[9][9] de caractères {Le plateau de jeu qu'on remplit}
// Locales:
//	i, j : Entiers {Compteurs de boucle, dans la 2e boucle i représente la ligne et j la colonne}
void afficherPlateau(char plateau[9][9])
{
	// Affichage des indices horizontaux
	printf("  ");
	int i;
	for (i = 1; i <= 9; i++)
		printf("%d ", i);
	puts("");

	// Affichage des lignes
	for (i = 0; i < 9; i++)
	{
		// Affichage de l'indice de la ligne
		printf("%c ", 'A' + i);
		// Affichage de la ligne
		int j;
		for (j = 0; j < 9; j++)
			printf("%c ", plateau[i][j]);
		puts("");
	}
}

// Définition d'un Coup
typedef struct Coup
{
	int ligDep, colDep;	// Coords de départ
	int ligArr, colArr;	// Coords d'arrivée
} Coup;

// Résultats:
//	coup : Coup {Le coup que saisit l'utilisateur, sous forme de données exploitables
//				 par notre programme}
// Locales: 
// coup_s : Chaine de caractères de taille 6 {Chaine contenant le coup entré par l'utilisateur}
void saisieCoup(Coup* coup)
{
	puts("Veuillez entrer un coup (ex: B1,E1 envoie le pion B1 sur la case E1)");
	char coup_s[6];
	scanf("%s", coup_s);
	coup->ligDep = coup_s[0] - 'A';
	coup->colDep = coup_s[1] - '1';
	coup->ligArr = coup_s[3] - 'A';
	coup->colArr = coup_s[4] - '1';
}

// Données:
//	coup : Coup {Le coup dont on vérifie la validité}
//	plateau : Tableau[9][9] de caractères {Le plateau de jeu actuel (avant que le coup soit effectué)}
//	joueurActuel : Caractère {Le joueur qui essaye de jouer ce coup}
// Locales:
//	ret : Booléen {Variable de retour valant faux si le coup est impossible et vrai si il est possible}
//	delta_c, delta_l : Entiers {le déplacement relatif du pion, respectivement pour les colonnes et les lignes}
// 	incrementLig, incrementCol : Entiers {Les incrément qui déterminent dans quel sens on effectue le scan des cases}
//	l_scan, c_scan : Entiers {La coordonnée scannée actuellement}
int coupPossible(Coup coup, char plateau[9][9], char joueurActuel)
{
	int ret;
	if (coup.ligDep < 0 || coup.ligDep > 8 ||
		coup.colDep < 0 || coup.colDep > 8 ||
		coup.ligArr < 0 || coup.ligArr > 8 ||
		coup.colArr < 0 || coup.colArr > 8) // Si une coordonnée est hors de la table de jeu
		ret = 0;
	else if (coup.ligDep == coup.ligArr && coup.colDep == coup.colArr) // Si le coup ne bouge pas le pion
		ret = 0;
	else if (plateau[coup.ligDep][coup.colDep] != joueurActuel) // Si on essaye de bouger un pion adverse ou bien un point
		ret = 0;
	else
	{
		int delta_c = coup.colArr - coup.colDep; // déplacement horizontal	(gauche -> droite)
		int delta_l = coup.ligArr - coup.ligDep; // déplacement vertical	(haut -> bas)
		int incrementLig;
		int incrementCol;

		if (delta_c != 0 && delta_l != 0) // Si on essaye de bouger en diagonale
			ret = 0;
		else
		{
			if (delta_c == 0) // Cas 1: on bouge verticalement
			{
				incrementCol = 0;
				if(delta_l > 0)
					incrementLig = 1;
				else
					incrementLig = -1;
			}
			else // Cas 2: on bouge horizontalement
			{
				incrementLig = 0;
				if(delta_c > 0)
					incrementCol = 1;
				else
					incrementCol = -1;
			}

			if((delta_c == 2 || delta_l == 2 || delta_c == -2 || delta_l == -2) && plateau[coup.ligArr][coup.colArr] == '.') // Couvre le cas du saut de pions
				ret = 1;
			else
			{
				int l_scan = coup.ligDep + incrementLig;
				int c_scan = coup.colDep + incrementCol;
				// On peut maintenant scanner dans le sens indiqué par les deux variables incrementCol et incrementLig
				ret = 1;
				// On effectue le scan de la case de départ exclue jusqu'à la case d'arrivée inclue.
				const int l_finScan = coup.ligArr + incrementLig;
				const int c_finScan = coup.colArr + incrementCol;
				while((l_scan != l_finScan || c_scan != c_finScan) && ret != 0)
				{
					if(plateau[l_scan][c_scan] != '.') // Si lors du scan on tombe sur autre chose qu'un point
						ret = 0;
					l_scan += incrementLig;
					c_scan += incrementCol;
				}
			}
		}

	}
	return ret;
}

// Données:
//	coup : Coup {Le coup effectué}
// Données/Résultats:
//	plateau : Tableau[9][9] de caractères {Le plateau avec les pion déplacé selon les
// 										   instructions du coup}
void deplacerPion(Coup coup, char plateau[9][9])
{
	char pion = plateau[coup.ligDep][coup.colDep];
	plateau[coup.ligDep][coup.colDep] = '.';
	plateau[coup.ligArr][coup.colArr] = pion;
}

// Données:
//	coup : Coup {Le coup qui a été joué ce tour}
// Données/Résultats:
//	plateau : Tableau[9][9] de caractères {Le plateau de jeu, après déplacement du pion accordément au coup, qu'on modifie
//										   pour effectuer la capture}
// Locales:
//	n_pionsCaptures : Entier {Variable de retour qui garde le compte du nombre de pions capturés parce tour}
// 	casesScannees : Entier {Le nombre de cases ayant été scanées dans le sens actuel (car on scan dans les 4 sens possibles)}
//	couleurPion : Caractère {Le pion qui a joué ce tour ('N' ou 'B')}
// 	incrementLig, incrementCol : Entiers {Les incrément qui déterminent dans quel sens on effectue le scan des cases}
//	l_scan, c_scan : Entiers {La coordonnée scannée actuellement}
int capturePions(Coup coup, char plateau[9][9])
{
	int n_pionsCaptures = 0;
	char couleurPion = plateau[coup.ligArr][coup.colArr];

	int i;
	for (i = 0; i < 4; i++)
	{
		int incrementCol, incrementLig;
		if (i == 0)
		{
			// On va de gauche à droite
			incrementCol = 1;
			incrementLig = 0;
		}
		else if (i == 1)
		{
			// On va de haut en bas
			incrementCol = 0;
			incrementLig = 1;
		}
		else if (i == 2)
		{
			// On va de droite à gauche
			incrementCol = -1;
			incrementLig = 0;
		}
		else
		{
			// On va de bas en haut
			incrementCol = 0;
			incrementLig = -1;
		}
		
		int casesScannees = 0;
		int c_scan = coup.colArr + incrementCol;
		int l_scan = coup.ligArr + incrementLig;
		// Tant qu'on est pas sortis du plateau et qu'on est toujours entrain de scanner des pions adverses
		while ( 0 <= c_scan && c_scan < 9 &&
				0 <= l_scan && l_scan < 9 &&
				plateau[l_scan][c_scan] != '.' &&
				plateau[l_scan][c_scan] != couleurPion)
		{
			casesScannees++;
			c_scan += incrementCol;
			l_scan += incrementLig;
		}

		// Si on est bien arrivés sur notre pion encadrant, et qu'on est donc ni sortis du plateau, ni arrivés sur un point
		if(0 <= c_scan && c_scan < 9 &&
		   0 <= l_scan && l_scan < 9 &&
		   plateau[l_scan][c_scan] == couleurPion)
		{
			n_pionsCaptures += casesScannees;
			// On décompte casesScannees jusque 0. Ceci nous évite de créer une variable supplémentaire comme compteur.
			// On remet c_scan et l_scan sur les cases précédentes, on en supprime autant qu'on en a scannées (= toutes les cases encadrées)
			for (; casesScannees > 0; casesScannees--)
			{
				c_scan -= incrementCol;
				l_scan -= incrementLig;
				plateau[l_scan][c_scan] = '.'; // On supprime la case actuelle.
			}
		}

		
	}
	return n_pionsCaptures;
}


int main()
{
	// Création du plateau
	char plateau[9][9];
	creerPlateau(plateau);

	// Compteurs de pions
	int n_pionsBlancs = 18;
	int n_pionsNoirs = 18;

	// Joueur Actuel
	char joueurActuel = 'B';

	while (!(n_pionsBlancs <= 5 || n_pionsNoirs <=5))
	{
		rafraichirEcran();
		// Affichage des informations de jeu
		printf("Joueur %s\n__________________________\n", joueurActuel == 'B' ? "blanc" : "noir");
		printf("Pions du joueur blanc : %d\nPions du joueur noir : %d\n\n", n_pionsBlancs, n_pionsNoirs);
		// Affichage du plateau
		afficherPlateau(plateau);
		puts("");

		// Saisie utilisateur du coup
		Coup coup;
		// on effectue la saisie du coup jusqu'à ce qu'il soit légal
		do
		{
			saisieCoup(&coup);
		} while (!coupPossible(coup, plateau, joueurActuel));
		
		// On continue le programme avec un coup légal

		// On déplace le pion à l'endroit voulu	
		deplacerPion(coup, plateau);
		
		// On répercute les actions du coup sur le plateau
		if (joueurActuel == 'B')
			n_pionsNoirs -= capturePions(coup, plateau);
		else
			n_pionsBlancs -= capturePions(coup, plateau);
			
		if(joueurActuel == 'B')
			joueurActuel = 'N';
		else
			joueurActuel = 'B';
	}

	rafraichirEcran();
	// Affichage des informations de jeu
	printf("Victoire du joueur %s !!!\n__________________________\n", joueurActuel == 'B' ? "noir" : "blanc");
	printf("Pions du joueur blanc : %d\nPions du joueur noir : %d\n\n", n_pionsBlancs, n_pionsNoirs);
	// Affichage du plateau
	afficherPlateau(plateau);

	return 0;
}

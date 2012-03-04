/*************************************************************************
                           XXX  -  description
                             -------------------
    début                : XXX
    copyright            : (C) XXX par XXX
    e-mail               : XXX
*************************************************************************/

//---------- Réalisation de la tâche <Clavier> (fichier Clavier.cpp) ---

/////////////////////////////////////////////////////////////////  INCLUDE
//-------------------------------------------------------- Include système
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
//------------------------------------------------------ Include personnel
#include "Clavier.h"
#include "/public/tp/tp-multitache/Menu.h"
#include "Struct.h"
///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types
//---------------------------------------------------- Variables statiques
static t_voiture voiture;
static int listeDescW[4];
static int nbCompteur = 0;
//------------------------------------------------------ Fonctions privées

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
void Commande(char code, unsigned int valeur)
// Algorithme :
//
{
	switch (code)
	{
		case 'e' :
		case 'E' : exit(0);
		break;
		case 'p' :
		case 'P' :
		{
			voiture.usager = PROF;
			switch(valeur)
			{
			case 1:{
				nbCompteur ++;
				voiture.numVoiture = nbCompteur % 999;
				write(listeDescW[0], &voiture, sizeof(t_voiture));
				break;
				}
			case 2:{
				nbCompteur ++;
				voiture.numVoiture = nbCompteur % 999;
				write(listeDescW[2], &voiture, sizeof(t_voiture));
				break;
				}
			}
			break;
		}
		case 'a':
		case 'A':
		{

			voiture.usager = AUTRE;
			switch(valeur)
			{
			case 1:{
				nbCompteur ++;
				voiture.numVoiture = nbCompteur % 999;
				write(listeDescW[1], &voiture, sizeof(t_voiture));
				break;
				}
			case 2:{
				nbCompteur ++;
				voiture.numVoiture = nbCompteur % 999;
				write(listeDescW[2], &voiture, sizeof(t_voiture));
				break;
				}
			}
			break;
		}

		case 's':
		case 'S':
		{
			write(listeDescW[3],&valeur, sizeof(valeur));
		break;
		}

	}
}

void Clavier(int uneListeDescW[4])
// Algorithme :
//
{
	for (int i=0 ; i<4 ; i++)
	{
		listeDescW[i] = uneListeDescW[i];
	}

	for(;;)
	{
		Menu();
	}
} //----- fin de Nom


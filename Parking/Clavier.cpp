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
//------------------------------------------------------ Fonctions privées
//static type nom ( liste de paramètres )
// Mode d'emploi :
//
// Contrat :
//
// Algorithme :
//
//{
//} //----- fin de nom

//static type nom ( liste de paramètres )
// Mode d'emploi :
//
// Contrat :
//
// Algorithme :
//
//{
//} //----- fin de nom

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
void Commande(char code, unsigned int valeur)
{
	switch (code)
	{
	case 'e' :
	case 'E' : exit(0);
	break;
	case 'p' :
	case 'P' :
	{
		switch(valeur)
		{
		case 1:{
			voiture.numVoiture = 100;
			voiture.usager = PROF;
			write(listeDescW[0], &voiture, sizeof(t_voiture));
			}
		}
	}
	break;
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


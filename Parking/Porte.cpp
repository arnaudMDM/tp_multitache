/*************************************************************************
                           XXX  -  description
                             -------------------
    début                : XXX
    copyright            : (C) XXX par XXX
    e-mail               : XXX
*************************************************************************/

//---------- Réalisation de la tâche <Porte> (fichier Porte.cpp) ---

/////////////////////////////////////////////////////////////////  INCLUDE
//-------------------------------------------------------- Include système
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <set>

//------------------------------------------------------ Include personnel
#include "Porte.h"
#include "Struct.h"
#include "/public/tp/tp-multitache/Outils.h"

///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques
static int descR;
static t_voiture voiture;
static TypeBarriere barriere;
static set<int> setPid;

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

static void handler(int numero)
// Mode d'emploi :
//
// Contrat :
//
// Algorithme :
//
{
	if(numero == SIGUSR2)
	{
		exit(0);
	}
	if(set<int>::iterator it = setPid.find(numero) != set::end)
	{

	}

} //----- fin de nom

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
void Porte(int uneDescR, TypeBarriere uneBarriere)
// Algorithme :
//
{
	for(int i=0 ; i<4 ; i++)
	{
		descR = uneDescR;
	}

	barriere = uneBarriere;

	struct sigaction action;
	action.sa_handler = handler;
	sigemptyset (&action.sa_mask);
	action.sa_flags = 0;
	sigaction(SIGUSR2, &action, NULL);
	sigaction(SIGCHILD, &action, NULL);

	for(;;)
	{
			if(read(descR,&voiture,sizeof(t_voiture)) != 0)
			{
				setPid.insert(GarerVoiture(barriere));
				sleep(1);
			}
	}
} //----- fin de Nom


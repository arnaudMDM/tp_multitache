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
#include <sys/wait.h>
#include <unistd.h>
#include <map>
#include <time.h>
#include <fstream>

//------------------------------------------------------ Include personnel
#include "Porte.h"
#include "Struct.h"
#include "/public/tp/tp-multitache/Outils.h"

///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types
struct t_place{
	t_voiture voiture;
	time_t dateArrivee;
};
//---------------------------------------------------- Variables statiques
static int descR;
static t_requete requete;
static TypeBarriere barriere;
static map<int,t_place> listePid;

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
		for(map<int,t_place>::iterator i=listePid.begin() ; i!=listePid.end() ; i++)
		{
			kill(i->first,SIGUSR2);
		}
		exit(0);
	}
	int numPlace;
	int pid = waitpid(0,&numPlace,0);
	numPlace=numPlace/256;
	map<int,t_place>::iterator it=listePid.find(pid);

	AfficherPlace(numPlace,it->second.voiture.usager,it->second.voiture.numVoiture, it->second.dateArrivee);
	listePid.erase(pid);


} //----- fin de nom

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
void Porte(int uneDescR, TypeBarriere uneBarriere)
// Algorithme :
//
{
	struct t_place place;
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
	sigaction(SIGCHLD, &action, NULL);

	for(;;)
	{
			if(read(descR,&(place.voiture),sizeof(t_voiture))!=-1)
			{
				place.dateArrivee = time(NULL);
				listePid.insert(pair<int,t_place>( GarerVoiture(barriere),place));
				sleep(1);
			}
	}
} //----- fin de Nom


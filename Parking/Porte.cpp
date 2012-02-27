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
#include <sys/shm.h>
#include <sys/sem.h>

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
void Porte(int uneDescR, TypeBarriere barriere, int idSM, int idSemGeneral)
// Algorithme :
//
{

	struct t_place place;
	for(int i=0 ; i<4 ; i++)
	{
		descR = uneDescR;
	}

	t_mem *etatEntreeParking = (t_mem *)shmat(idSM, NULL, 0);

	struct sembuf semMemPP;
	semMemPP.sem_num = 4;
	semMemPP.sem_op = 1;
	semMemPP.sem_flg = 0;

	struct sembuf semMemPR;
	semMemPR.sem_num = 4;
	semMemPR.sem_op = -1;
	semMemPR.sem_flg = 0;

	struct sembuf semSortie;
	semSortie.sem_op = -1;
	semSortie.sem_flg = 0;

	int idPorte;

	switch(barriere)
	{
	case PROF_BLAISE_PASCAL : idPorte = 0;
	break;
	case AUTRE_BLAISE_PASCAL : idPorte = 1;
	break;
	case ENTREE_GASTON_BERGER : idPorte = 2;
	break;
	}

	semSortie.sem_num = idPorte;

	struct sigaction action;
	action.sa_handler = handler;
	sigemptyset (&action.sa_mask);
	action.sa_flags = 0;
	sigaction(SIGUSR2, &action, NULL);
	sigaction(SIGCHLD, &action, NULL);

	t_requete requete;

	for(;;)
	{
			if(read(descR,&(place.voiture),sizeof(t_voiture))!=-1)
			{
				DessinerVoitureBarriere(barriere,place.voiture.usager);
				place.dateArrivee = time(NULL);
				semop(idSemGeneral,&semMemPP,1);
				if(etatEntreeParking->nbVoiture < 8)
				{
					etatEntreeParking->nbVoiture += 1;
					semop(idSemGeneral, &semMemPR,1);
					listePid.insert(pair<int,t_place>( GarerVoiture(barriere),place));
					sleep(1);
				}
				else
				{
					requete.dateArrivee = place.dateArrivee;
					requete.usager = place.voiture.usager;
					etatEntreeParking->tabRequete[idPorte] = requete;
					AfficherRequete(barriere,requete.usager,requete.dateArrivee);
					semop(idSemGeneral, &semMemPR,1);
					semop(idSemGeneral,&semSortie,1);
					listePid.insert(pair<int,t_place>( GarerVoiture(barriere),place));
					sleep(1);
				}
			}
	}
} //----- fin de Nom


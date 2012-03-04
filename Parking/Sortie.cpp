/*************************************************************************
                           Sortie  -  description
                             -------------------
    d√©but                : 24/02/2012
    copyright            : (C) B3336 par XXX
    e-mail               : XXX
*************************************************************************/

//---------- R√©alisation de la t√¢che <<Sortie> (fichier Sortie.cpp) ---

/////////////////////////////////////////////////////////////////  INCLUDE
//-------------------------------------------------------- Include syst√®me
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <vector>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fstream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <errno.h>
//------------------------------------------------------ Include personnel
#include "Sortie.h"
#include "Struct.h"
///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques

//Zone d'attachement de la memoire
static t_mem  *etatParking;
static vector <pid_t> listePid;
static int canal;
static int idSemGeneral;
static struct sembuf semMemPrendre = {3,-1, 0};
static struct sembuf semMemRendre = {3, 1, 0};
//------------------------------------------------------ Fonctions priv√©es

static bool premierePrioritaire(const t_requete *req1, const t_requete *req2)
// Algorithme :
// true si la premiere requete est prioritaire par rapport ‡ la seconde, faux
// sinon.
// Cas particulier :
// Si les deux requetes sont vides, alors la seconde est quand mÍme considÈrÈe comme prioritaire
//
{
	if(req1->dateArrivee == -1)
	{
		return false;
	}

	if(req2->dateArrivee == -1)
	{
		return true;
	}
	//Sinon rÈsolution des conflicts de prioritÈs sur PROF d'abord puis dateArrivee ensuite si nÈcessaire.
	if(req1->usager == PROF)
	{
		if(req2->usager == PROF)
		{
			//Tous les 2 sont PROF, regarder date
			if(req1->dateArrivee < req2->dateArrivee)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		return true;
	}
	else
	{
		if(req2->usager == PROF)
		{
			return false;
		}
		//Tous les 2 sont AUTRE, regarder date
		else
		{
			if(req1->dateArrivee < req2->dateArrivee)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}

}


static void fin (int noSignal)
// Algorithme :
//
{
	//Liberation des ressources
	shmdt(etatParking);

	close(canal);
	for(vector<pid_t>::iterator it = listePid.begin(); it != listePid.end(); it++)
	{
		kill(*it, SIGUSR2);
	}
	exit (0);
} //----- fin de fin

static void effacerPlace(int numPlace)
// Algorithme :
//
{
	switch(numPlace)
	{
		case 1 : Effacer(ETAT_P1);
		break;
		case 2 : Effacer(ETAT_P2);
		break;
		case 3 : Effacer(ETAT_P3);
		break;
		case 4 : Effacer(ETAT_P4);
		break;
		case 5 : Effacer(ETAT_P5);
		break;
		case 6 : Effacer(ETAT_P6);
		break;
		case 7 : Effacer(ETAT_P7);
		break;
		case 8 : Effacer(ETAT_P8);
		break;
		default:
		break;
	}

}
static void sortieVoiture(int noSignal)
// Mode d'emploi :
//Handler de SIGCHLD
// Contrat :
//
// Algorithme :
{
	int status;
	vector<vector<pid_t>::iterator> aEffacer;

	for(vector<pid_t>::iterator it = listePid.begin(); it != listePid.end(); it++)
	{
		waitpid(*it, &status,WNOHANG);

		if( WIFEXITED(status))
		{
			aEffacer.push_back(it);
			unsigned int numPlace = WEXITSTATUS(status);

			semop(idSemGeneral, &semMemPrendre, 1);

			t_place *placeLiberee = &(etatParking->placesParking[numPlace]);

			AfficherSortie(placeLiberee->voiture.usager, numPlace, placeLiberee->dateArrivee, time(NULL) );
			effacerPlace(numPlace);

			if(etatParking->nbVoiture == 8)
			{
				// -1 signifie une erreur pour l'id du semaphore
				struct sembuf semPorte = {-1, 1, 0};

				t_requete *laRequete = NULL;
				int indexRequete = -1;

				if(etatParking->tabRequete[0].dateArrivee != -1)
				{
					laRequete = &etatParking->tabRequete[0];
					indexRequete = 0;
				}
				if(premierePrioritaire(&etatParking->tabRequete[1], laRequete))
				{
					laRequete = &etatParking->tabRequete[1];
					indexRequete = 1;
				}
				if(premierePrioritaire(&etatParking->tabRequete[2], laRequete))
				{
					laRequete = &etatParking->tabRequete[2];
					indexRequete = 2;
				}

				if( laRequete)
				{
					semPorte.sem_num = indexRequete;
					semop(idSemGeneral,&semPorte,1);
				}
				else
				{
					etatParking->nbVoiture --;
				}

			}
			else
			{
				etatParking->nbVoiture --;
			}
			semop(idSemGeneral, &semMemRendre, 1);
		}
	}

	for(vector<vector<pid_t>::iterator>::iterator it = aEffacer.begin(); it != aEffacer.end(); it++)
	{
		listePid.erase(*it);
	}

}
//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
void Sortie (int uneDescR, key_t keySM, key_t keySem)
// Algorithme :
//
{
	idSemGeneral = semget(keySem, 4, 0);
	int idSM = shmget(keySM, sizeof(int)+3*sizeof(t_requete), 0);
//Phase d'initialisation
	canal = uneDescR;

	//Preparation de la memoire partagee
	etatParking = (t_mem *)shmat(idSM, NULL, 0);

	//Mise en place du handler de fin de tache
	struct sigaction actionFin;
	actionFin.sa_handler = fin;
	sigemptyset (&actionFin.sa_mask);
	actionFin.sa_flags = 0;
	sigaction(SIGUSR2, &actionFin, NULL);
	
	//Mise en place du handler pour traiter les sorties de voitures
	struct sigaction actionMoteur;
	actionMoteur.sa_handler = sortieVoiture;
	sigemptyset(&actionMoteur.sa_mask);
	actionMoteur.sa_flags = 0;
	sigaction(SIGCHLD, &actionMoteur, NULL);

	//Initialisation de la zone mÈmoire
	while( (semop(idSemGeneral, &semMemPrendre, 1) == -1) && (errno == EINTR)){}
	etatParking->nbVoiture = 0;
	semop(idSemGeneral, &semMemRendre,1);

	unsigned int numPlace = 0;
	for(;;)
	{
		if(read(canal,&(numPlace),sizeof(unsigned int)) != -1 )
		{
			pid_t pid = SortirVoiture(numPlace);
			if(pid != -1)
			{
				listePid.push_back(pid);
			}
		}
	}
} //----- fin de Sortie


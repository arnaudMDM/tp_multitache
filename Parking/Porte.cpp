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
#include <sys/types.h>
#include <sys/ipc.h>
#include <errno.h>
//------------------------------------------------------ Include personnel
#include "Porte.h"
#include "Struct.h"

///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques
static int descR;
static map<int, t_place> listePid;
static t_mem *etatParking;
static int idSemGeneral;
static struct sembuf semMemPrendre = { 3, -1, 0 };
static struct sembuf semMemRendre = { 3, 1, 0 };
static TypeZone zoneRequete;
//------------------------------------------------------ Fonctions privées

static void handler ( int numero )
// Mode d'emploi :
//
// Contrat :
//
// Algorithme :
//
{
	if (numero == SIGUSR2)
	{
		for (map<int, t_place>::iterator i = listePid.begin();
				i != listePid.end(); i++)
				{
			kill(i->first, SIGUSR2);
		}
		exit(0);
	}

	int status;
	int pid = waitpid(0, &status, 0);

	if(WIFEXITED(status))
	{
		int numPlace = WEXITSTATUS(status);
		map<int, t_place>::iterator it = listePid.find(pid);


		semop(idSemGeneral, &semMemPrendre, 1);

		etatParking->placesParking[numPlace] = it->second;

		semop(idSemGeneral, &semMemRendre, 1);


		AfficherPlace(numPlace, it->second.voiture.usager,
				it->second.voiture.numVoiture, it->second.dateArrivee);

		listePid.erase(pid);
	}

}    //----- fin de nom

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
void Porte ( int uneDescR, TypeBarriere barriere, key_t keySM, key_t keySem )
// Algorithme :
//
{

	idSemGeneral = semget(keySem, 4, 0);
	int idSM = shmget(keySM, sizeof(int)+3*sizeof(t_requete), 0);


	struct t_place place;
	for (int i = 0; i < 4; i++)
	{
		descR = uneDescR;
	}

	etatParking = (t_mem *) shmat(idSM, NULL, 0);

	struct sembuf semSortie;
	semSortie.sem_op = -1;
	semSortie.sem_flg = 0;

	int idPorte;

	switch (barriere)
	{
		case PROF_BLAISE_PASCAL:
			zoneRequete = REQUETE_R1;
			idPorte = 0;
			break;
		case AUTRE_BLAISE_PASCAL:
			zoneRequete = REQUETE_R2;
			idPorte = 1;
			break;
		case ENTREE_GASTON_BERGER:
			zoneRequete = REQUETE_R3;
			idPorte = 2;
			break;
	}

	semSortie.sem_num = idPorte;

	semop(idSemGeneral, &semMemPrendre, 1);

	etatParking->tabRequete[idPorte].dateArrivee = -1;

	semop (idSemGeneral, &semMemRendre, 1);

	struct sigaction action;
	action.sa_handler = handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	sigaction(SIGUSR2, &action, NULL);
	sigaction(SIGCHLD, &action, NULL);

	t_requete requete;

	for (;;)
	{
		if (read(descR, &(place.voiture), sizeof(t_voiture)) != -1)
		{
			DessinerVoitureBarriere(barriere, place.voiture.usager);
			place.dateArrivee = time(NULL);

			while( (semop(idSemGeneral, &semMemPrendre, 1) == -1) && (errno == EINTR)){}


			if (etatParking->nbVoiture < 8)
			{
				etatParking->nbVoiture += 1;
				semop(idSemGeneral, &semMemRendre, 1);

				listePid.insert(
						pair<int, t_place>(GarerVoiture(barriere), place));
				sleep(1);
			}
			else
			{
				requete.dateArrivee = place.dateArrivee;
				requete.usager = place.voiture.usager;
				etatParking->tabRequete[idPorte] = requete;
				AfficherRequete(barriere, requete.usager, requete.dateArrivee);
				semop(idSemGeneral, &semMemRendre, 1);


				while( (semop(idSemGeneral, &semSortie, 1) == -1) && (errno == EINTR)){}


				Effacer(zoneRequete);


				while( (semop(idSemGeneral, &semMemPrendre, 1) == -1) && (errno == EINTR)){}
				//Invalide la requete
				etatParking->tabRequete[idPorte].dateArrivee = -1;

				semop(idSemGeneral, &semMemRendre, 1);
				listePid.insert(
						pair<int, t_place>(GarerVoiture(barriere), place));
				sleep(1);
			}
		}
	}
}    //----- fin de Nom


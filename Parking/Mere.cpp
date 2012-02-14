/*************************************************************************
                           Mere  -  description
                             -------------------
    début                : XXX
    copyright            : (C) XXX par XXX
    e-mail               : XXX
*************************************************************************/

//---------- Réalisation de la tâche <Mere> (fichier Mere.cpp) ---

/////////////////////////////////////////////////////////////////  INCLUDE
//-------------------------------------------------------- Include système
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <signal.h>
//------------------------------------------------------ Include personnel
#include "Mere.h"
#include "Clavier.h"
#include "/public/tp/tp-multitache/Outils.h"
#include "/public/tp/tp-multitache/Heure.h"
///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types
struct t_requete{
	time_t dateArrivee;
	TypeUsager usager;
	TypeBarriere porte;
};
//---------------------------------------------------- Variables statiques
static int idSemGeneral;
static int listePipes[4][2];
static int idSM;
//------------------------------------------------------ Fonctions privées
static void initialiserParking()
// Mode d'emploi :
//
// Contrat :
//
// Algorithme :
//
{
	struct sigaction action;
	action.sa_handler = SIG_IGN;
	sigemptyset (&action.sa_mask);
	action.sa_flags = 0;
	sigaction(SIGINT, &action, NULL);
	sigaction(SIGUSR2, &action, NULL);

	InitialiserApplication(XTERM);

	idSemGeneral = semget(IPC_PRIVATE, 4, S_IRUSR | S_IWUSR);

	int desc[2];
	for(int i = 0; i < 4; i++)
	{
		pipe(desc);
		listePipes[i][0] = desc[0];
		listePipes[i][1] = desc[1];
	}

	idSM = shmget(IPC_PRIVATE, sizeof(int)+3*sizeof(t_requete), S_IRUSR | S_IWUSR );

	if( pid_t pid = fork() == 0)
	{
		Clavier();
	}
	else
	{
		waitpid(pid, NULL,0);
	}
}

static void terminerParking()
// Mode d'emploi :
//
// Contrat :
//
// Algorithme :
//
{
	semctl(idSemGeneral, 0, IPC_RMID, 0);

	shmctl(idSM, IPC_RMID, 0);

	TerminerApplication();
}

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
int main ()
// Algorithme :
//
{
	initialiserParking();

	terminerParking();

	return 0;
} //----- fin de Nom

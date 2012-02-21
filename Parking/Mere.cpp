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
#include "Struct.h"
///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques
static int idSemGeneral;
static int idSM;
static int listeDescW[4];
static int listeDescR[4];
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
	sigaction(SIGCHILD, &action, NULL);

	InitialiserApplication(XTERM);

	idSemGeneral = semget(IPC_PRIVATE, 4, S_IRUSR | S_IWUSR);

	int desc[2];
	for(int i = 0; i < 4; i++)
	{
		pipe(desc);
		listeDescR[i] = desc[0];
		listeDescW[i] = desc[1];
	}

	idSM = shmget(IPC_PRIVATE, sizeof(int)+3*sizeof(t_requete), S_IRUSR | S_IWUSR );

	if(fork() == 0)
	{
		Porte(,listeDescR[0], PROF_BLAISE_PASCAL);
	}
	else
	{
		if( pid_t pid = fork() == 0)
		{
			Clavier(listeDescW);
		}
		else
		{
			waitpid(pid, NULL,0);
		}
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

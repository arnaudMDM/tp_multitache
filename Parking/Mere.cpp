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
#include "Porte.h"
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
static pid_t pidPorteBP;
static pid_t pidClavier;
static pid_t pidPorteA;
static pid_t pidPorteG;
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
	sigaction(SIGCHLD, &action, NULL);

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

	if((pidPorteBP=fork()) == 0)
	{
		Porte(listeDescR[0], PROF_BLAISE_PASCAL, idSM,idSemGeneral);
	}

	if((pidPorteA=fork()) == 0)
	{
		Porte(listeDescR[1], AUTRE_BLAISE_PASCAL, idSM,idSemGeneral);
	}

	if((pidPorteG=fork()) == 0)
	{
		Porte(listeDescR[2], ENTREE_GASTON_BERGER, idSM,idSemGeneral);
	}



	if( (pidClavier= fork()) == 0)
	{
		Clavier(listeDescW);
	}
	else
	{
		waitpid(pidClavier, NULL,0);
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
	kill(pidPorteBP,SIGUSR2);
	kill(pidPorteA,SIGUSR2);
	kill(pidPorteG,SIGUSR2);
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



#include <iostream>

using namespace std;

#include <sys/sem.h>


int main()
{
	struct sembuf semPrendre = {0, -1,0};
	struct sembuf semRendre = {0, 1,0};
	int idSem = semget(IPC_PRIVATE, 1, S_IRUSR | S_IWUSR);

	semctl(idSem, 1,SETVAL,1);
	semop(idSem, &semPrendre, 1);
	cout<<"Test"<<endl;
	semop(idSem, &semRendre, 1);

	semctl(idSem, 0, IPC_RMID, 0);
}

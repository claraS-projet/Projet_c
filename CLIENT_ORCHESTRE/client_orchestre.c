#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/sem.h>


#include <semaphore.h>


#include "myassert.h"

#include "client_orchestre.h"

//Déclaration globale des tubes et semaphore
int o2c;
int c2o;
int semoc;
int c2olect;
int o2cwrite;
int c2owrite;
int o2clect;

//contiennent les routines de communications entre l'orchestre et un client (tubes nommés).


void creationComoc(){
// Création du tube pour le sens orchestre vers client
    o2c = mkfifo("pipe_o2c", 0644); 
    assert(o2c != -1);
    

// Création du tube pour le sens client vers orchestre
    c2o = mkfifo("pipe_c2o", 0644); 
    assert(c2o != -1);
    
}

void destructionComoc(){
// Destruction du tube pour le sens orchestre vers client
	o2c = unlink("pipe_o2c");
   	assert(o2c == 0);

// Destruction du tube pour le sens client vers orchestre
	c2o = unlink("pipe_c2o");
	assert(c2o == 0);

}

void creationSemoc(){
// - création d'un sémaphore pour que deux clients ne
//   ne communiquent pas en même temps avec l'orchestre
    
    semoc = semget(5, 1, IPC_CREAT | 0644);
    assert( semoc != -1);
}

void initSemoc(){
//Initaialisation du semaphore
    
    int initsem = semctl(semoc, 0, SETVAL, 1);
    assert(initsem != -1);
}

void openComOrchestre(){
// ouverture ici des tubes nommés avec un client
// attente d'une demande de service du client
        
        c2olect = open("pipe_c2o", O_RDONLY);
        assert( c2olect != -1);
        printf("L'orchestre a ouvert le tube c2o en lecture !\n");
        
        o2cwrite = open("pipe_o2c", O_WRONLY);
        assert (o2c != -1);
        printf("L'orchestre a ouvert le tube o2c en ecriture !\n");
}

void openComClient(){
//Ouverture du tube pour que le client envoie une demande à l'orchestre
	c2owrite = open("../ORCHESTRE/pipe_c2o", O_WRONLY);
	assert(c2owrite != -1);
	printf("Le client a ouvert le tube c2o en ecriture\n");
	
//Ouverture du tube pour que le client lise les reponses de l'orchestre
	o2clect = open("../ORCHESTRE/pipe_o2c", O_RDONLY);
	assert(o2clect != -1);
	printf("Le client a ouvert le tube o2c en lecture\n");
}

void openSemClient(){
// - le sémaphore pour que 2 clients ne conversent pas en même
//   temps avec l'orchestre
	semoc = semget(5, 1, 0);
	assert(semoc != -1);
}
// Opération sur le semaphore pour gérer l'acces en zone critique
void myOp(){
	struct sembuf myOpclient[1];
	myOpclient[0].sem_num = 0;
	myOpclient[0].sem_op = +1;
	myOpclient[0].sem_flg = 0;
}

void sentNum(int num){
//Envoie d'une demande pour l'utilisation du service num a l'orchestre
	int ret = write(c2o, &num, sizeof(int));
	assert(ret != -1);
	assert(ret == sizeof(int));
	printf("Envoie du num %d du service a l'orchestre OK\n", num);
}

int readNum(){
	int num;
	printf("ok ");
	int ret = read(c2o, &num, sizeof(int));
	printf("ok ");
	assert(ret != 0);
	printf("ok ");
	assert(ret == sizeof(int));
	printf("ok ");
	printf("L'orchestre a lu la demande du service %d par le client", num);
	return num;
}

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
#include <string.h>


#include <semaphore.h>


#include "myassert.h"

#include "client_orchestre.h"

//Déclaration globale des tubes et semaphore
int semoc;
int c2olect;
int o2cwrite;
int c2owrite;
int o2clect;

//contiennent les routines de communications entre l'orchestre et un client (tubes nommés).


void creationComoc(){
// Création du tube pour le sens orchestre vers client
/* if(access("pipe_o2c", F_OK) == 0)
	int rem = remove("pipi_o2c")
	assert(rem != 0);*/
    int ret;
    ret = mkfifo("pipe_o2c", 0644); 
    assert(ret != -1);
    

// Création du tube pour le sens client vers orchestre
    ret = mkfifo("pipe_c2o", 0644); 
    assert(ret != -1);
    
}

void destructionComoc(){
// Destruction du tube pour le sens orchestre vers client
	// juste int ret = comme pour mkfifo
    int ret;
	ret = unlink("pipe_o2c");
   	assert(ret == 0);

// Destruction du tube pour le sens client vers orchestre
	ret = unlink("pipe_c2o");
	assert(ret == 0);

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
        assert (o2cwrite != -1);
        printf("L'orchestre a ouvert le tube o2c en ecriture !\n");
}

void closeComOrchestre(){
// fermer les tubes vers le client
	int ret = close(c2olect);
	assert (ret == 0);
	printf("L'orchestre a fermé le tube c2o en lecture !\n");
	
	int ret2 = close(o2cwrite);
	assert (ret2 == 0);
	printf("L'orchestre a fermé le tube o2c en ecriture !\n");
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
/*
void openSemClient(){
// - le sémaphore pour que 2 clients ne conversent pas en même
//   temps avec l'orchestre
	semoc = semget(5, 1, 0);
	assert(semoc != -1);
}
// Opération sur le semaphore pour gérer l'acces en zone critique
void myOpadd(int semId){
	struct sembuf myOpclientadd[1];
	myOpclientadd[0].sem_num = 0;
	myOpclientadd[0].sem_op = +1;
	myOpclientadd[0].sem_flg = 0;
	
	int add = semop(semId, myOpclientadd, 1);
	assert( add != -1); 
}

void myOpmoins(int semId){
	struct sembuf myOpclientmoins[1];
	myOpclientmoins[0].sem_num = 0;
	myOpclientmoins[0].sem_op = +1;
	myOpclientmoins[0].sem_flg = 0;
	
	int moins = semop(semId, myOpclientmoins, 1);
	assert( moins != -1);	
}*/

void sentNum(int num){
//Envoie d'une demande pour l'utilisation du service num a l'orchestre
//ajouter l'op -1 au semaphore pour passer ne section critique
	/*myOpmoins(semoc);*/
	
	int ret = write(c2owrite, &num, sizeof(int));
	assert(ret != -1);
	assert(ret == sizeof(int));
	printf("Envoie du num %d du service a l'orchestre OK\n", num);
}

int readNum(){
	int num;
	printf("ok le tube : %d\n", c2olect);
	int ret = read(c2olect, &num, sizeof(int));
	assert(ret != 0);
	assert(ret == sizeof(int));
	printf("L'orchestre a lu la demande du service %d par le client\n", num);
	return num;
}

void codeAcceptationoc(){
	int codeAccept = 1;
	int ret = write(o2cwrite, &codeAccept, sizeof(int));
	assert( ret != -1);
	assert(ret == sizeof(int));
	printf("L'orchestre a envoyé le code d'acceptation : 1 au Client\n"); 
}

void codeErreuroc(){
	int codeErreur = -1;
	int ret = write(o2cwrite, &codeErreur, sizeof(int));
	assert( ret != -1);
	assert(ret == sizeof(int));
	printf("L'orchestre a envoyé le code d'erreur : -1 au Client\n"); 
}

int readCoderetour(){
	int codeRetour;
	int ret = read(o2clect, &codeRetour, sizeof(int));
	assert(ret != 0);
	assert(ret == sizeof(int));
	printf("Le client a lu le code de retour : %d \n", codeRetour);
	return codeRetour;
}

void SentMdp(int mdpService){
//Envoi du mot de passe au client (via le tube nommé)
	int ret = write(o2cwrite, &mdpService, sizeof(int));
	assert( ret != -1);
	assert(ret == sizeof(int));
	printf("L'orchestre a envoyé le mot de passe : %d au Client\n", mdpService);
}

void sentTube(int numService){
//Détermination du nom du tube en fonction du numéro du service demandé
	char tube_s2c[11];
	char tube_c2s[11];
	switch(numService){
		case 0 :
			strcpy(tube_s2c, "pipe_s2c_0");
			strcpy(tube_c2s, "pipe_c2s_0");
			break;
		case 1 :
			strcpy(tube_s2c, "pipe_s2c_1");
			strcpy(tube_c2s, "pipe_c2s_1");
			break;
		case 2 :
			strcpy(tube_s2c, "pipe_s2c_2");
			strcpy(tube_c2s, "pipe_c2s_2");
	}
	int len = 11; //les noms des tubes font tous la même taille

//Envoi des noms des tubes nommés au client (via le tube nommé)	
	int ret2 = write(o2cwrite, tube_s2c, sizeof(char)* len);
	assert( ret2 != -1);
	printf("L'orchestre a envoyé le nom du tube 1 :%s au Client\n", tube_s2c);
	
	int ret3 = write(o2cwrite, tube_c2s, sizeof(char)* len);
	assert( ret3 != -1);
	printf("L'orchestre a envoyé le nom du tube 2 :%s au Client\n", tube_c2s);
}

int readMdp(){
//Récupération du mot de passe et des noms des 2 tubes
	int Mdp;
	int ret = read(o2clect, &Mdp, sizeof(int));
	assert(ret != 0);
	assert(ret == sizeof(int));
	printf("Le client a lu le Mdp : %d \n", Mdp);
	return Mdp;
	
}

char *readTubeName(){
	int len = 11; //les noms des tubes font tous la même taille
	char *tubeName;
	tubeName = malloc(len *sizeof(char));
	if (tubeName == NULL) {
		perror("Erreur d'allocation de la mémoire");
		exit(EXIT_FAILURE);
	}
	int ret = read(o2clect, tubeName, sizeof(char)* len);
	assert( ret != -1);
	printf("Le client a lu le nom du tube : %s \n", tubeName);
	return tubeName;
}

void finTransactionClient(){
//Envoie d'un code a l'ochestre pour signaler la fin de la communication
	int codeFin = -1;
	int ret = write(c2owrite, &codeFin, sizeof(int));
	assert( ret != -1);
	assert(ret == sizeof(int));
	printf("Le client a envoyé le code de fin : -1 à l'orchestre\n"); 
	
//Fermeture des tubes de communication avec l'orchestre
	int ret1 = close(c2owrite);
	assert (ret1 == 0);
	printf("Le client a fermé le tube c2o en ecriture !\n");
	
	int ret2 = close(o2clect);
	assert (ret2 == 0);
	printf("Le client a fermé le tube o2c en lecture !\n");
}

int finTransactionOrchestre(){
//Lecture du code signalant la fin de la transaction entre le client et l'orchestre
	int numfin;
	int ret = read(c2olect, &numfin, sizeof(int));
	assert(ret != 0);
	assert(ret == sizeof(int));
	printf("L'orchestre a lu l'accusé de reception envoyé par le client\n");
	return numfin;
}



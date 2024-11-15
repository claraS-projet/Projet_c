#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>


#include "myassert.h"
#include "client_orchestre.h"

int main() {

//Ouverture du tube pour que le client envoie une demande à l'orchest
    int c2owrite = open("pipe_c2o", O_WRONLY);
    assert(c2owrite != -1);
    printf("Le client a ouvert le tube c2o en ecriture");
    
//Ouverture du tube pour que le client lise les reponses de l'orchestre
    int o2clect = open("pipe_o2c", O_RDONLY);
    assert(o2clect != -1);
    printf("Le client a ouvert le tube o2c en lecture");

// - le sémaphore pour que 2 clients ne conversent pas en même
//   temps avec l'orchestre
    int semoc = semget(5, 1, 0);
    assert(semoc != -1);
    
// Opération sur le semaphore pour gérer l'acces en zone critique
    struct sembuf myOpclient[1];
    myOpclient[0].sem_nim = 0;
    myOpclient[0].sem_op = +1;
    myOpclient[0].seem_flg = 0;


    
}

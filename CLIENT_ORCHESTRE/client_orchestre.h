#ifndef CLIENT_ORCHESTRE_H
#define CLIENT_ORCHESTRE_H

// Ici toutes les communications entre l'orchestre et les clients :
// - le sémaphore pour que 2 clients ne conversent pas en même
//   temps avec l'orchestre
// - les deux tubes nommés pour la communication bidirectionnelle
void creationComoc();
void destructionComoc();
void creationSemoc();
void destructionSemoc();
void initSemoc();
void openComOrchestre();
void closeComOrchestre();
void openComClient();
int openSemClient();
void myOpadd(int semId);
void myOpmoins(int semId);
void sentNum(int num);
int readNum();
void codeAcceptationoc();
void codeErreuroc();
int readCoderetour();
void SentMdp(int mdpService);
void sentTube(int numService);
int readMdp();
char * readTubeName();
void finTransactionClient();
int finTransactionOrchestre();

#endif

#ifndef CLIENT_ORCHESTRE_H
#define CLIENT_ORCHESTRE_H

// Ici toutes les communications entre l'orchestre et les clients :
// - le sémaphore pour que 2 clients ne conversent pas en même
//   temps avec l'orchestre
// - les deux tubes nommés pour la communication bidirectionnelle
void creationComoc();
void destructionComoc();
void creationSemoc();
void initSemoc();
void openComOrchestre();
void openComClient();
void openSemClient();
void myOp();
void sentNum(int num);
int readNum();

#endif

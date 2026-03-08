/*
 * Obiettivo: capire i puntatori a struct
 */

#include <stdio.h>
#include <string.h>

// Dichiariamo una struttura; l'OS assegna 4 byte per "int valore" e 20 byte per
// "char nome"
struct Sensore {
  int valore;
  char nome[20];
};
// Totale: 24 byte

void leggi_sensore(
    struct Sensore *ptr) { // la funzione prende come parametro un puntatore
                           // alla struttura "Sensore";
  strcpy(ptr->nome, "Termometro"); // copiamo, tramite il puntatore, un valore
                                   // all'interno di nome e valore
  ptr->valore = 25;
}

// Senza toccare la struttura nel main, ci ritroviamo la variabile struct
// "sensore" riempita; questo perché "leggi_sensore" ha operato direttamente
// sulla memoria.
int main() {
  struct Sensore sensore;
  leggi_sensore(&sensore);
  printf("Nome sensore: %s\n", sensore.nome);
  printf("Valore: %d", sensore.valore);
}

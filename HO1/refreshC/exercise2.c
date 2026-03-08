/*
 *
 * Capire come non andare oltre lo spazio allocato di memoria
 */

#include <stdio.h>
#include <string.h>

void copia_sicura(char *stringaDaCopiare, char *buffer, int byteDaCopiare) {
  for (int i = 0; i < byteDaCopiare; i += 1) {
    buffer[i] = stringaDaCopiare[i];
  }
  buffer[byteDaCopiare] = '\0';
}

int main() {
  char buffer[1024];
  char *messaggio = "Ciao mondo!";
  copia_sicura(messaggio, buffer, strlen(messaggio));
  printf("Contenuto buffer: %s", buffer);
}

# C Refresher
Il seguente documento serve a fare un ripasso sugli argomenti principali del linguaggio C.

# I puntatori
In C, ogni variabile viene salvata in una scatola della memoria RAM, ogni scatola ha un indirizzo specifico e univoco. Un puntatore non è altro che una variabiile che, invece di contenere un dato contiene l'indirizzo di memoria di un'altra variabile:
- Con l'operatore `&` si ottiene l'indirizzo di memoria di una variabile;
- Con l'operatore `*` (deferenziazione) si accede al contenuto della scatola a cui in indirizzo punta.

## Esempio
```c
int numero = 42;
int *puntatore_a_numero = &numero;

// Posso camibiare il contenuto di "numero" dal puntatore:
*puntatore_a_numero = 100; // ora "numero" = 100
```

# Struct
Una struct è un modo per raggruppare variaibli di tipo diverso sotto un unico nome. Con esse, creiamo un blocco di memoria **contiguo**. I campi di una normale struct sono accessibili con il punto (.)
## Esempio
```c
typedef struct {
    uint32_t length;
    uint8_t type;
  } PacketHeader;

PacketHeader packet;
packet.length = 10;
packet.type = 1;
```

# Puntatori a Struct (`->`)
Quando passiamo una struct ad una funzione, C la copia tutta. Nel caso in cui abbiamo struct grandi (per esempio, con un ampio buffer), la copia diventa inefficiente (sprechiamo memoria). La soluzione è passare il solo puntatore alla struct, già allocata e definita.
Per accedere ai campi non si può utilizzare la notazione con il punto in questo caso, bisognerebbe prima deferenziare il puntatore e usare il punto (del tipo: (*puntatore_a_struct).dato). Per evitare questa lunga notazione, in C esiste la notazione a "freccia".
Essenzialmente, scrivere (*puntatore_a_struct).dato e puntatore_a_struct->dato è la stessa cosa.
## Esempio
```c
PacketHeader header;
PacketHeader *puntatore_a_header = &header;

(*puntatore_a_header).length = 10;
puntatore_a_header->lenght = 10; // entrambe le istruzioni sono equivalenti
```

# Memoria dinamica
In C è possibile richiedere all'OS dei blocchi di memoria dinamici (scelti in fase di esecuzione del programma) dalla memoria "Heap". Per farlo, si uilizzano le funzioni della famiglia `malloc()`.
## Esempio `malloc`
`malloc` prende un solo parametro, ovvero quanti byte ci servono. Se ha successom restituisce un puntatore al primo byte di questo spazio, altrimenti NULL:
```c
int *array_dinamico = malloc(100*sizeof(int));

// controllo se ritorna NULL...
// if(...) {}

array_dinamico[0] = 40;

// ... altro codice

free(array_dinamico) // FONDAMENTALE: alla fine di ogni scopo che utilizza la memoria dinamica, occorre liberarla con `free()`, altrimenti il blocco NON viene restituito all'OS anche se il dato non serve più (memory leak)
```

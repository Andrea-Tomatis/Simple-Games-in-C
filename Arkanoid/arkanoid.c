/*
-------------------------------------------------------------------------------------
                            Author: Andrea Tomatis
                        compito estate 2020: Arkanoid
-------------------------------------------------------------------------------------
*/

#include <conio.h> //kbhit getch
#include <stdio.h>
#include <stdbool.h>
#include "./librerie/myUtility.h"
#include "./librerie/partita.h"

//definizioni costanti tasti
#define ENTER 13 
#define UP 72
#define DOWN 80
#define MAX_CLASSIFICA 100  //dimensione massima della classifica
#define LUNG_STRINGA 20  //è la lunghezza massima per i nomi dei giocatori (vedi funzione fine partita)

typedef struct cursor{  //struttura cursore: posiziona il cursore nella selezione del menù
	int x, y;
	char car;
}Cursor;

typedef struct classificato{  //struttura per salvare i dati dei vari giocatori
	char nome[LUNG_STRINGA];  //nome del giocatore
	int punti;  //livello raggiunto
}Classificato;


//-------PROTOTIPI FUNZIONI-----------------------------------
int stampatitolo();  //stampa la pagina del menù e ne gestisce le interazioni con l'utente
void creaCursore(Cursor *c);  //imposta le impostazioni di default per il cursore
void cancellaCursore(Cursor c);  //cancella il cursore
void disegnaCursore(Cursor c);  //stampa il cursore
void spostaCursore(Cursor *c);  //si occupa di spostare il cursore se è il caso
void finePartita(int lvl);  //si occupa del salvataggio dei dati e di tutte le necessità di fine partita
void salvaClassifica(char s[], int lvl);  //salva in un file i dati del giocatore che ha appena giocato
void visualizzaClassifica();  //compie tutte le operazioni necessarie alla visualizzazione della classifica
void ordinaClassifica(Classificato classifica[], int n);  //ordina al classifica dal primo all'ultimo
void stampaClassifica(Classificato classifica[], int n);  //stampa la classifica a schermo
void scambio(Classificato *a, Classificato *b);  //scambia due classificati




void main(){
	cursor_hide(); //imposta il cursore invisibile
	int opz;  //tasto premuto dall'utente
	int lvl;  //livello raggiunto in ogni partita
	
	do{  //ciclo per la navigazione nel menù
		opz = stampatitolo();  //stampa il menù

		switch (opz){  //in base al tasto premuto vengono chiamate delle funzioni
			case 1:  //il primo caso fa partire una nuova partita
				clrscr(); //pulisce lo schermo prima di iniziare
				lvl = 1;  //imposta il livello a 1 per iniziare
				iniziaPartita(&lvl);  //contiene tutto il codice della partita (si veda la librerisa partita.h)
				finePartita(lvl);  //salva tutti i dati relativi alla partita appena conclusa
				break;
			case 2:
				visualizzaClassifica();  //visualizza la classifica
				break;
			case 3:
				opz = ESC;  //esce
				break;
			}
	}while (opz != ESC); //finchè l'opzione è diversa da esc cicla

	return;
}


//--------------FUNZIONI------------------------------
int stampatitolo(){  //stampa la pagina del menù e ne gestisce le interazioni con l'utente
	clrscr();
	unsigned short oldtextcolor = getTextColor();  //salva il precedente colore del testo

	int key;  //tasto premuto dall'utente
	Cursor cursore;  //cursore che punta le opzioni
	creaCursore(&cursore); //assegna le corrette impostazioni al cursore
	
	textcolor(ROSSO); //imposta il colore rosso per il testo
	//stampa il nome del gioco
	gotoxy(0, 4);
	printf("------------------------------------------------");
	gotoxy(20, 5);
	printf("ARKANOID");
	gotoxy(0, 6);
	printf("------------------------------------------------");

	//stampa le opzioni del menù
	gotoxy(21,9);
	printf("GIOCA!");
	gotoxy(14,11);
	printf("visualizza classifica");
	gotoxy(17,13);
	printf("esci dal gioco");
	
	//stampa un consiglio per muoversi nel menù
	gotoxy(0,38);
	printf("premere la freccia in su per spostarsi nel men%c",163);
	

	disegnaCursore(cursore);  //disegna il cursore per la prima volta
	do{
		key = getch(); //prende un input da tastiera
		switch (key){  //in base al tasto premuto il cursore viene mosso o viene selezionata un'opzione
			case UP:  //il cursore viene spostato più in alto
				spostaCursore(&cursore);
				break;
			case ENTER:  //in base a dove si trova il cursore una delle opzioni viene selezionata (restituita al main con un numero)
				switch (cursore.y){
					case 9:
						return 1;  //inizia la partita
						break;
					case 11:
						return 2;  //visualizza la classifica
						break;
					case 13:
						return 3;  //esce dal gioco
						break;
				}
				break;
			default:
				break;
		}
		
	}while (key != ESC);  //se il tasto premuto è ESC esce dal ciclo
	
	setTextColor(oldtextcolor);
	return 3;
}


void creaCursore(Cursor *c){   //imposta le impostazioni di default per il cursore
	c->car = '>';
	//le coordinate iniziali sono quelle della prima opzione nel menù
	c->x = 12;
	c->y = 9;
	return;
}

void cancellaCursore(Cursor c){   //cancella il cursore
	unsigned short oldcolor = getTextColor();
	gotoxy(c.x,c.y);
	textcolor(NERO);
	printf(" ");
	setTextColor(oldcolor);
	return;
}

void disegnaCursore(Cursor c){  //stampa il cursore
	gotoxy(c.x,c.y);
	printf("%c",c.car);
	return;
}

void spostaCursore(Cursor *c){  //si occupa di spostare il cursore se è il caso
	cancellaCursore(*c);   //la precedente posizione viene cancellata
	c->y += -1;  //il cursore sale di 1	 nelle opzioni
	c->y = c->y % 5 + 9;  //fa si che il cursore rimanga dentro i confini del menù
	if (c->y % 2 == 0){  //dato che tra un'opzione e l'altra c'è una riga vuota questa if si assicura che venga saltata
		c->y += 1;
	}
	disegnaCursore(*c);  //stampa il cursore nella nuova posizione
	return;
}

void finePartita(int lvl){   //si occupa del salvataggio dei dati e di tutte le necessità di fine partita
	unsigned short oldtextcolor = getTextColor();  //il vecchio colore viene salvato
	clrscr();  //lo schermo si cancella
	char s[LUNG_STRINGA];  //è la stringa che conterrà il nome del giocatore che ha appena giocato

	//viene stampato il risultato insieme ad una notifica di sconfitta
	textcolor(VERDE);
	gotoxy(19, 20);
	printf("HAI PERSO :(");
	gotoxy(10, 21);
	printf("HAI RAGGIUNTO IL LIVELLO %2d", lvl - 1);
	setTextColor(oldtextcolor);

	//viene richiesto al giocatore un nome con cui salvare il risultato
	gotoxy(10,23);
	printf("inserisci il tuo nome: ");
	cursor_show();
	fflush(stdin);
	scanf("%s",s);
	cursor_hide();

	salvaClassifica(s,lvl-1);  //salva il nome del player e il livello raggiunto in un file
	return;
}

void salvaClassifica(char s[], int lvl){  //salva in un file i dati del giocatore che ha appena giocato
	FILE *fp;
	fp = fopen("classifica.dat","a");  //il file viene aperto in append così da non cancellare i precedenti dati in fase di scittura
	fprintf(fp,"%s %d\n",s,lvl);
	fclose(fp);
	return;
}



void visualizzaClassifica(){  //compie tutte le operazioni necessarie alla visualizzazione della classifica
	clrscr();  //lo schermo viene ripulito
	int k = 0; //conta il numero di giocatori presenti in classifica
	Classificato classifica[MAX_CLASSIFICA];  //vettore contenente tutti i dati dei giocatori (letti da file)
	FILE *fp;

	//il file, se esiste, viene letto fino alla fine o fino a quando il numero dei giocatori raggiunge il massimo (100)
	if((fp = fopen("classifica.dat","r")) == NULL)
		printf("non sono state giocate ancora partite");
	else{
		while (fscanf(fp,"%s %d",classifica[k].nome,&classifica[k].punti) != EOF  && k < MAX_CLASSIFICA)  
			k++;
		fclose(fp);
		ordinaClassifica(classifica,k);  //il vettore classifica viene ordinato così da poter essere stampato correttamente
		stampaClassifica(classifica,k);  //il vettore ordinato viene stampato
	}
	
	getch();  //serve per dare il tempo all'utente di leggere la classifica
	return;
}


void ordinaClassifica(Classificato classifica[], int n){  //ordina al classifica dal primo all'ultimo
	//tramite il bubble sort la classifica viene ordinata in ordine crescente
	int k, sup;
	for (sup = n-1; sup > 0; sup--){
		for (k = 0; k < sup; k++){
			if (classifica[k].punti > classifica[k+1].punti)
				scambio(&classifica[k],&classifica[k+1]);
		}
	}
	
	//tramite un vettore di appoggio la classifica viene invertita così da essere ordinata in ordine decrescente
	Classificato classifica2[n];
	for(int i=0; i<n; i++)
    	classifica2[i] = classifica[n-i-1];
	for(int i = 0; i < n; i++)
		classifica[i] = classifica2[i];

	return;
}

void stampaClassifica(Classificato classifica[], int n){  //stampa la classifica a schermo
	for (int i = 0; i< n; i++){
		printf("%d^posto: %s livello raggiunto %d\n",i+1,classifica[i].nome,classifica[i].punti);
	}
	return;
}

void scambio(Classificato *a, Classificato *b){  //scambia due classificati
	Classificato temp = *a;
	*a = *b;
	*b = temp;
	return;
}
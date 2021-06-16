/*---------------------------------------------------------------------------------
							  libreria Arkanoid
								Andrea Tomatis
								   anno:2020
								   
								   
Questa libreria contiene le funzioni pi� importanti del programma, quelle che gestiscono le partite
----------------------------------------------------------------------------------------------------*/			

//---------------------DEFINIZIONE COSTANTI-------------------------------------

//dimensioni schermo
#define HEIGH 40
#define WIDTH 48

//codici ascii da tastiera
#define A 97
#define A_MAIUSCOLA 65
#define D 100
#define D_MAIUSCOLA 68
#define DX 77
#define SX 75
#define QUATTRO 52
#define SEI 54
#define ESC 27

//caratteri per elementi grafici
#define QUADRATO 219
#define PALLA 223
#define SPAZIO 32




//lunghezze e limiti
#define LIM 36
#define LUNGBASE 7
#define DIM 15
#define LUNG 48
#define LUNGMATTONE 3

//colori
#define NERO 0
#define BIANCO 15
#define GRIGIO 9
#define VERDE 2 
#define ROSSO 4
#define VIOLA 5
#define ORO 6
#define GRIGIOSCURO 8
#define BORDEAUX 12
#define GIALLO 14


//---------------------------------------------------------------------

//-----------------STRUTTURE DATI-------------------------------------
typedef struct palla{
	int x,y;
	int carattere;
	int colore;
}Palla;

typedef struct blocco{
	int x, y;
	int carattere;
	int colore;
	int lung;
}Blocco;
//--------------------------------------------------------------------

//-----------PROTOTIPI DI FUNZIONI----------------------------------------
void iniziaPartita(int *lvl); //contiene tutto il codice della partita
void creaPalla(Palla *p);	//assegna alla struttura palla tutte le caratteristiche necessarie
void creaBase(Blocco *b);	//assegna alla struttura base tutte le caratteristiche necessarie
void disegnaPalla(Palla p);  //funzione che disegna la palla nella correta posizione
void disegnaBase(Blocco b); //funzione che disegna la base
void disegnaLIM(int lim);	//funzione che disegna il limite oltre cui si perde per avvisare il giocatore del pericolo
void muoviBase (Blocco *b, int direzione); //funzione che muove la base
void cancellaPalla(Palla p);  //funzione che cancella la palla
void cancellaBase(Blocco b);	//funzione che cancella la base

int estraiDirezione();	//estrae a sorte una tra tre direzioni per far si che la partita non inizi sempre nella medesima direzione
int estraicolore(); //estrae casualmente un colore per il blocco

void pulisciLivello(int livelli[][LUNG], int n); //resetta tutta la matrice livello a 0
void caricaMappa(int lvl, int livelli[][LUNG], int n); //carica la matrice livello con gli 1 per indicare la presenza di blocchi
void disegnaMappa(int livelli[][LUNG], int numerorighe); //stampa a schermo la matrice livello

bool toccaBordoX(Palla p);  //restituisce true se la palla incontra i bordi laterali
bool toccaBordoY(Palla p);  //restituisce true se la palla incontra il bordo superiore o inferiore
int toccabase(Palla p, Blocco base);  //restutuisce 1 o -1 in base a se la palla incontra la parte destra o sinistra della base
bool toccablocco(Palla p, int livello[][LUNG], int dx, int dy);  //restituisce true se la palla incontra un blocco

void trovaBlocco(int livello[][LUNG], int x, int y);  //se la palla incontra un mattoncino capisce in quale dei tre pixel che lo formano ha sbattuto e gli cancella tutti e tre
void cancellaBlocco(int x, int y, int lung, int livello[][LUNG]); //cancella un mattoncino

void stampalivello(int lvl);  //in fondo allo schermo stampa il livello corrente
bool livellocompletato(int livello[][LUNG], int numrighe);  //restituisce true se il livello � stato superato

void spostaPalla(Palla *p, int *dX, int *dY, Blocco base, int livello[][LUNG], int numerorighe); //cambia la palla di posizione applicando tutti i controlli necessari


//-----------------------------------------------------------------------


//------------------------------------------------------------------------
//-----------------FUNZIONE PRINCIPALE (MAIN)-----------------------------
//------------------------------------------------------------------------

void iniziaPartita(int *lvl){
	
	textcolor(BIANCO); //imposta il colore bianco per iniziare
	
	//variabili
	int key; //variabile che memorizza l'ultimo tasto premuto
	int fine = false;  //se vale true la partita viene interrotta
	bool loadLvl = true; //se vale true significa che � ora di caricare un nuovo livello
	int direzBase = 1; //se vale 1 va  a destra altrimenti a sinistra 
	int direzPallaX = estraiDirezione();  //la palla pu� iniziare sia verso destra che verso sinistra
	int direzPallaY = -1; //se vale -1 va in alto, se vale 1 in basso
	int livelli[DIM][LUNG]; //matrice che contiene i mattoni
	int numerorighe = *lvl + 3;
	int numeroblocchi = numerorighe * 8; //numero dei blocchi da eliminare
	
	Palla p; //la palla del gioco
	Blocco b; //la base su mobile su cui la palla rimbalza
	creaPalla(&p); //imposta correttamente i parametri necessari per la palla
	creaBase(&b); //imposta correttamente i parametri necessari per la base
	disegnaPalla(p); //stampa la palla
	disegnaBase(b); //stampa la base
	disegnaLIM(LIM); //stampa la linea del limite
	

	do{ //finch� fine � falsa...
	
		if (loadLvl){ //se � il caso di caricare un nuovo livello lo fa
			*lvl += 1; //il livello aumenta
			pulisciLivello(livelli,DIM); //inizialmente ripulisce la matrice dei livelli
			caricaMappa(*lvl, livelli, DIM); //olre a caricare la mappa nella matrice calcola il numero di blocchi totali
			disegnaMappa(livelli,numeroblocchi); //stampa la matrice dei livelli
			stampalivello(*lvl); //stampa il livello corrente
			loadLvl = false; //fino a nuovo ordine non viene caricato un nuovo livello
		}

		if (kbhit()){ //se viene premuto un tasto
			key = getch(); //il tasto viene salvato

			switch(key){ //in base al tasto ci sono pi� possibilit�
				//la freccia direzionale verso sinistra, il numero 4 e la lettera A fanno muovere la base a sinistra
				case SX:
				case QUATTRO:
				case A_MAIUSCOLA:
				case A:
					direzBase = -1;
					muoviBase(&b, direzBase);
					break;
				//la freccia direzionale verso destra, il numero 6 e la lettera D fanno muovere la base a sinistra
				case DX:
				case SEI:
				case D_MAIUSCOLA:
				case D:
					direzBase = 1;
					muoviBase(&b, direzBase);
					break;
				case ESC:  //il pulsante ESC provoca la fine della partita
				    fine = true;
					break;
				default:
					break;
			} //fine switch case
		} //fine if
		
		spostaPalla(&p,&direzPallaX,&direzPallaY, b, livelli, numeroblocchi/8); //la palla viene spostata
		if (livellocompletato(livelli, numeroblocchi/8)) loadLvl = true; //se il livello viene completato deve essere caricato un'altro
		if (p.y >= LIM) fine = true; //se la palla raggiunge il limite la partita � conclusa
	}while(!fine); //ciclo finch� la partita non finisce

	return;
}//fine funzione iniziaPartita()

//-------------------------------------------------------------------------

//------------------FUNZIONI--------------------------------------------

void creaPalla(Palla *p){ //assegna ad un elemento di tipo Palla tutte le caratteristiche necessarie
	//la palla si trova al centro dello schermo di 1y sopra la base
	p->x = WIDTH/2;
	p->y = LIM - 2;
	p->carattere = PALLA;
	p->colore = BIANCO;
	return;
}

void creaBase(Blocco *b){  //assegna ad un elemento di tipo Base tutte le caratteristiche necessarie
	b->carattere = QUADRATO;
	b->colore = GRIGIO;
	b->lung = LUNGBASE;
	//la base si trova al centro dello schermo di 1y sopra il limite di sconfitta
	b->x = WIDTH/2 - 3;
	b->y = LIM - 1;
	return;
}


void disegnaPalla(Palla p){  //funzione che disegna la palla nella correta posizione
	unsigned short oldcolor = getTextColor();
	textcolor(p.colore);
	gotoxy(p.x,p.y);
	printf("%c",p.carattere);
	setTextColor(oldcolor);
	return;
}


void disegnaBase(Blocco b){ //funzione che disegna la base nella correta posizione
	unsigned short oldcolor = getTextColor(); 
	gotoxy(b.x,b.y);
	textcolor(b.colore);
	for(int k = 0; k < b.lung; k++)
		printf("%c",b.carattere);
	setTextColor(oldcolor);
	return;
}

void disegnaLIM(int lim){  //funzione che disegna il limite oltre cui si perde per avvisare il giocatore del pericolo
	gotoxy(0,lim);
	for (int k = 0; k < WIDTH; k++)
		printf("-");
	return;
}


void muoviBase (Blocco *b, int direzione){ //funzione che muove la base
	cancellaBase(*b); //cancella la base e ne cambia la x
	if ((b->x + direzione) >= 0 && (b->x + direzione + LUNGBASE) <= WIDTH )
        b->x += direzione;
	disegnaBase(*b); //ridisegna la base nella nuova posizione
	return;
}


void cancellaPalla(Palla p){ //funzione che cancella la palla
	unsigned short oldcolor = getTextColor();
    gotoxy(p.x, p.y);
    textcolor(NERO);
    printf("%c", SPAZIO);
    setTextColor(oldcolor);
    return;
}

void cancellaBase(Blocco b){ //funzione che cancella la base
	unsigned short oldcolor = getTextColor();
	gotoxy(b.x,b.y);
	textcolor(NERO);
	for (int i = 0; i < b.lung; i++) //stampa tanti spazi quanti quelli lunga la base
		printf("%c",SPAZIO);
	setTextColor(oldcolor);
	return;
}






int estraiDirezione(){  //estrae a sorte una tra tre direzioni per far si che la partita non inizi sempre nella medesima direzione
	
	srand(time(NULL)); //cambia il seme (seed) del random
	int maxdir = 2; //estrae un numero compreso tra 0 e 1 (quindi 2 valori)
	int dir = rand() % maxdir;
	//in base al numero risultante restituisce la direzione
	if (dir == 0) return -1; 	
	else 
		return 1;
}

int estraicolore(){ //estrae casualmente un colore per il blocco
	int col;
	int max = 7;	//estrae 7 colori (tra l'1 e il 7)
	col = (rand() % max)+1; 
	switch (col){
		case 1:
			return VERDE;
			break;
		case 2:
			return ROSSO;
			break;
		case 3:
			return VIOLA;
			break;
		case 4:
			return ORO;
			break;
		case 5:
			return BORDEAUX;
			break;
		case 6:
			return GIALLO;
			break;
		case 7:
			return GRIGIOSCURO;
			break;
		default:
			return NERO;
			break;
	}

}
 
void pulisciLivello(int livelli[][LUNG], int n){  //resetta tutta la matrice livello a 0
	//svuota la matrice con due cicli for
	for (int i = 0; i < n; i++){
		for (int j = 0; j < LUNG; j++){
			livelli[i][j] = 0;
		}
	}
	return;
}

void caricaMappa(int lvl, int livelli[][LUNG], int n){  //carica la matrice livello con gli 1 per indicare la presenza di blocchi
	int numeroRighe = 3 + lvl; //calcolo del numero di righe da disegnare
	int pezzo = 0; //valore che serve per far si che le righe vengano allineate per la x una volta si e una no
	
	
	for (int i = 0; i < numeroRighe; i++){ //finchè non stampa tutte le righe...
		if (i % 2 == 0) pezzo = 0;	//se la riga è pari allora il primo blocco avrà x = 0
		else pezzo = 3; //altrimenti avrà x = 3
		for (int j = pezzo; j < LUNG; j += 6){ //j viene incrementato di sei in sei in quanto dall'inizio di un blocco all'altro devono esserci 6 spazi
			//assegnando 1 a tre posizioni consecutive della matrice crea un blocco
			livelli[i][j] = 1; 
			livelli[i][j+1] = 1;
			livelli[i][j+2] = 1;
		}
		
	}

	return; 
}

void disegnaMappa(int livelli[][LUNG], int numerorighe){  //stampa a schermo la matrice livello
	unsigned short oldcolor = getTextColor(); 
	numerorighe /= 8;
	int lungcolore = 0; //variabile che mantiene il colore sempre uguale per tutti e 3 i pixel del blocco
	Blocco mattone; //creazione di un blocco da stampare
	mattone.carattere = QUADRATO;	//assegnazione del carattere e della lunghezza
	mattone.lung = LUNGMATTONE;
	for (int i = 0; i < numerorighe; i++){
		for (int j = 0; j< LUNG; j++){
			if (livelli[i][j] == 1){ //se la matrice ha valore 1 in quella determinata posizione disegna un blocco
				if (lungcolore % mattone.lung == 0) mattone.colore = estraicolore(); //se è un nuovo blocco cambia colore
				//assegna i e j alle coordinate del mattore (o blocco)
				mattone.x = j; 
				mattone.y = i;
				gotoxy(mattone.x,mattone.y);
				textcolor(mattone.colore);
				printf("%c",mattone.carattere);
				lungcolore++;	//incrementa la lunghezza colore
			}
		}
	}
	
	setTextColor(oldcolor);
	return;
}

bool toccaBordoX(Palla p){ //restituisce true se la palla incontra i bordi laterali
    return (p.x >= WIDTH-1  || p.x <= 0);
}

bool toccaBordoY(Palla p){  //restituisce true se la palla incontra il bordo superiore o inferiore
    return (p.y == 0 || p.y >= HEIGH-1);
}

int toccabase(Palla p, Blocco base){  //restutuisce 1 o -1 in base a se la palla incontra la parte destra o sinistra della base
	if (p.y == base.y-1 && p.x >= base.x-1 && p.x <= (base.x+base.lung+1))  //if che si accerta che la pallina tocchi la base
		return (p.x <= (base.x + (base.lung / 2))) ? -1 : 1;  //un altra if che controlla se ha colpito la base a dx o a sx
	
	return 0;
}

bool toccablocco(Palla p, int livello[][LUNG], int dx, int dy){  //restituisce true se la palla incontra un mattoncino
	return (livello[p.y+dy][p.x+dx] == 1);  //se la palla si trover� nell'istante futuro in conincidenza con un mattoncino restituisce true
}

void trovaBlocco(int livello[][LUNG], int x, int y){  //se la palla incontra un mattoncino capisce in quale dei tre pixel che lo formano ha sbattuto e gli cancella tutti e tre
	
	if(y % 2 == 0){ //in base alla riga si possono ricavare le posizioni dei mattoncini e cancellarli
		if (x % 2 == 1){
			cancellaBlocco(x-1,y,3,livello);
		}else if (x % 6 == 0){
			cancellaBlocco(x,y,3,livello);	
		}else if (x % 6 == 1){
			cancellaBlocco(x-2,y,3,livello);
		}
	}else{
		if (x % 2 == 0){
			cancellaBlocco(x-1,y,3,livello);
		}else if (x % 3 == 0){
			cancellaBlocco(x,y,3,livello);	
		}else {
			cancellaBlocco(x-2,y,3,livello);
		}
	}
	return;
}

void cancellaBlocco(int x, int y, int lung, int livello[][LUNG]){  //cancella un mattoncino
	gotoxy(x,y);
	unsigned short oldcolor = getTextColor();
	textcolor(NERO);
	for (int i = 0; i < lung; i++){
		printf("%c",SPAZIO);
		livello[y][x+i] = 0; //resetta a 0 la posizione in cui si trovava il blocco	
	}
	setTextColor(oldcolor);
	return;
}

void stampalivello(int lvl){  //in fondo allo schermo stampa il livello corrente
	gotoxy(20,38);
	printf("livello: %d",lvl-1);
	return;
}

bool livellocompletato(int livello[][LUNG], int numrighe){  //restituisce true se il livello � stato superato
	//per farlo controlla se la matrice � vuota o meno
	for (int i = 0; i < numrighe; i++)
		for (int j = 0; j < LUNG; j++)
			if (livello[i][j] == 1) return false;
		
	return true;
}

void spostaPalla(Palla *p, int *dX, int *dY, Blocco base, int livello[][LUNG], int numerorighe){  //cambia la palla di posizione applicando tutti i controlli necessari
    //disegno la pallina nella posizione corrente
    disegnaPalla(*p);
    //attesa
    delay(75);
    //cancello la pallina dalla posizione corrente
    cancellaPalla(*p);
	//imposto la nuovo posizione
    p->x += *dX;
    p->y += *dY;
	//queste variabili booleane servono per controllare che la palla non cambi 2 volte direzione andando fuori schermo
	bool invertiy = false;
	bool invertix = false;
	//se tocca un blocco devo cambiare direzione
	if (p->y <= numerorighe){  //se la y della palla � minore o uguale al numero di righe si � sicuri che non ci saranno problemi con le posizioni della matrice
		if (toccablocco(*p,livello,0, *dY)){
			trovaBlocco(livello,p->x, p->y+*dY); //trova e cancella il mattoncino
			*dY = *dY * (-1); //inverte la direzione della y
			invertiy = true; //impedisce che la y possa essere reinvertita
		}
		if (toccablocco(*p,livello, *dX, 0)){
			trovaBlocco(livello,p->x+*dX, p->y); //trova e cancella il mattoncino
			*dX = *dX * (-1); //inverte la direzione della x
			*dY = *dY * (-1); //inverte la direzione della y
			invertiy = true; //inverte la direzione della y
			invertix = true; //inverte la direzione della x
		}
		if (toccablocco(*p,livello, *dX, *dY)){
			trovaBlocco(livello,p->x+(*dX), p->y+(*dY)); //trova e cancella il mattoncino
			if (!invertix){
				*dX = *dX * (-1); //inverte la direzione della x
				invertix = true;
			}
			if (!invertiy){
				*dY = *dY * (-1); //inverte la direzione della y
				invertix = true;
			}
		}
	}

	//se tocca la base deve cambiare direzione
	if (toccabase(*p,base) != 0){
		*dX = toccabase(*p,base); //assegna alla x la giusta direzione
		*dY = *dY * (-1);
	}
		
    //se tocca il bordo a destra o a sinistra devo cambiare direzione della x
    if (toccaBordoX(*p)){
    	if (!invertix)
        	*dX = *dX * (-1);
    }
    //se tocca il bordo a in alto o in basso devo cambiare direzione della x
    if (toccaBordoY(*p)){
    	if (!invertiy)
        	*dY = *dY * (-1);
    }
	
	return;
}

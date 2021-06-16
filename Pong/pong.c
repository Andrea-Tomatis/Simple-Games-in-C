#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>
#include "myUtility.h"

#define I 'i'
#define I_MIN 'I'
#define K 'k'
#define K_MIN 'K'
#define W 'w'
#define W_MIN 'W'
#define S 's'
#define S_MIN 'S'
#define ESC 27
#define ENTER 13
#define UP 72
#define DOWN 80
#define QUADRATO 254

#define HEIGHT 30
#define WIDTH 120
#define LIMSX 5
#define LIMDX 113
#define LUNG_NOME 20
#define LUNG_STR 200
#define DIM_STR 30

#define NERO 0
#define VERDE 2 
#define VIOLA 5
#define ORO 6
#define BLU 9
#define ROSSO 12
#define GIALLO 14
#define BIANCO 15

#define CONTORNO_VERTICALE 186
#define CONTORNO_ORIZZONTALE 205
#define ANGOLO_ALTO_SX 201
#define ANGOLO_ALTO_DX 187
#define ANGOLO_BASSO_SX 200
#define ANGOLO_BASSO_DX 188


typedef struct palla{
    int x,color,y;
    char car;
}Palla;

typedef struct slider{
    int x,y,color,dim;
    char car;
}Slider;

typedef struct cursor{  //struttura cursore: posiziona il cursore nella selezione del menù
	int x, y;
	char car;
}Cursor;


void partita(int *punto1, int *punto2);
void disegnaCampo();
void creaPalla(Palla *ball);
void creaSlider(Slider *sld, int nplayer);
void disegnaPalla(Palla ball);
void disegnaSlider(Slider sld);
void spostaPalla(Palla *p, int *dx, int *dy, Slider sld1, Slider sld2,int velocita);
bool toccaBordoY(Palla p);
bool toccaBordoX(Palla p);
bool toccaSlider(Palla p, Slider sld, int nplayer);
void cancellaPalla(Palla p);
void muoviSlider(Slider *sld, int direction);
void cancellaSlider(Slider sld);
int controllaPunto(Palla p);
void stampaPunti(int punti1, int punti2, char nome1[], char nome2[]);
void finepartita(int *punti1, int *punti2, char nome1[], char nome2[]);
void disegnaLineeCampo();
void disegnaCursore(Cursor c);  //stampa il cursore
void creaCursore(Cursor *c);  //imposta le impostazioni di default per il cursore
void cancellaCursore(Cursor c);
void spostaCursore(Cursor *c, int dir);
void visualizzaRegistro();
void salvaDati(int punti1, int punti2, char nome1[], char nome2[]);
void pausa();
void disegnaPrimaPagina();
void disegnaCornice();
void chiediNomi(char nome1[],char nome2[]);
void invertifile(char nomefile[]);


int main(){
    int key;
    int punto1, punto2;
    Cursor cursore;  //cursore che punta le opzioni
	creaCursore(&cursore); //assegna le corrette impostazioni al cursore
    cursor_hide();

    disegnaPrimaPagina();
    

    disegnaCursore(cursore);  //disegna il cursore per la prima volta
    do{
		key = getch(); //prende un input da tastiera
		switch (key){  //in base al tasto premuto il cursore viene mosso o viene selezionata un'opzione
			case UP:  //il cursore viene spostato più in alto
				spostaCursore(&cursore, -1);
				break;
            case DOWN:
                spostaCursore(&cursore, 1);
				break;
            case ENTER:
                Beep(750,700);
				switch (cursore.y){
					case 11:
						punto1 = 0;
                        punto2 = 0;
                        partita(&punto1,&punto2);
						break;
					case 13:
                        visualizzaRegistro();
						break;
					case 15:
						key = ESC;
						break;
				}
                disegnaPrimaPagina();
                disegnaCursore(cursore);
				break;
			default:
				break;
		}
		
	}while (key != ESC);  //se il tasto premuto è ESC esce dal ciclo
    clrscr();

    return 0;
}

void disegnaPrimaPagina(){
    unsigned short oldcolor = getTextColor();
    disegnaCornice();

    textcolor(VERDE);
    gotoxy(43,2);
    printf(" _____   _____  _      _  _______");
    textcolor(VIOLA);
    gotoxy(43,3);
    printf("|  _  | |     || \\    | ||  _____|");
    textcolor(ORO);
    gotoxy(43,4);
    printf("| |_| | |  _  ||  \\   | || |");
    textcolor(BIANCO);
    gotoxy(43,5);
    printf("| ____| | | | ||   \\  | || |  ___");
    textcolor(ROSSO);
    gotoxy(43,6);
    printf("| |     | |_| || \\  \\ | || | |_  | ");
    textcolor(GIALLO);
    gotoxy(43,7);
    printf("| |     |     || |\\  \\| || |___| |");
    textcolor(BLU);
    gotoxy(43,8);
    printf("|_|     |_____||_| \\____||_______|");

    setTextColor(oldcolor);
    gotoxy(47,10);
    printf("scegli un'opzione:");
    gotoxy(47,11);
    printf("GIOCA");
    gotoxy(47,13);
    printf("registro partite");
    gotoxy(47,15);
    printf("esci");
    return;
}



void disegnaCampo(){
    int x = 0, y = 0;
    unsigned short oldcolor = getTextColor();
    textcolor(BLU);
    for (x; x < WIDTH-1; x++){
        gotoxy(x,y);
        printf("%c",219);
    }  
    
    for (y; y < HEIGHT-1; y++){
        gotoxy(x,y);
        printf("%c",219);
    }
    x = 0;
    y = 0;
    for (y; y < HEIGHT-1; y++){
        gotoxy(x,y);
        printf("%c",219);
    }
    for (x; x < WIDTH-1; x++){
        gotoxy(x,y);
        printf("%c",219);
    }  
    setTextColor(oldcolor);

    
    return;
}

void disegnaLineeCampo(){
    int x = 61;
    int y = 0;
    for (y; y < HEIGHT-1; y++){
        gotoxy(x,y);
        if (y % 2 == 0) printf("|");
    }

    x = 5;
    y = 1;
    for (y; y < HEIGHT-1; y++){
        gotoxy(x,y);
        if (y % 2 == 0) printf("|");
    }
    
    y = 7;
    for(x = 6; x < 17; x++){
        gotoxy(x,y);
        if (x % 2 == 0) printf("_");
    }
    y = 22;
    for(x = 6; x < 17; x++){
        gotoxy(x,y);
        if (x % 2 == 0) printf("_");
    }
    x = 17;
    for (y = 8; y < 23; y++){
        gotoxy(x,y);
        if (y % 2 == 0) printf("|");
    }



    x = 115;
    y = 1;
    for (y; y < HEIGHT-1; y++){
        gotoxy(x,y);
        if (y % 2 == 0) printf("|");
    }
    y = 7;
    for(x = 114; x > 103; x--){
        gotoxy(x,y);
        if (x % 2 == 0) printf("_");
    }
    y = 22;
    for(x = 114; x > 103; x--){
        gotoxy(x,y);
        if (x % 2 == 0) printf("_");
    }
    x = 103;
    for (y = 8; y < 23; y++){
        gotoxy(x,y);
        if (y % 2 == 0) printf("|");
    }

    x = 61;
    y = 14;
    gotoxy(61,15);
    printf("%c",QUADRATO);

    return;
}
void creaPalla(Palla *ball){
    ball->x = 60;
    ball->y = 15;
    ball->color = GIALLO;
    ball->car = '*';
    return;
}

void creaSlider(Slider *sld, int nplayer){
    sld->color = ROSSO;
    sld->dim = 3;
    sld->car = 219;
    sld->y = 14;
    sld->x = (nplayer == 1) ? 6 : 112;
    return; 
}

void disegnaPalla(Palla ball){
    gotoxy(ball.x, ball.y);
    unsigned short oldcolor = getTextColor();
    textcolor(ball.color);
    printf("%c",ball.car);
    setTextColor(oldcolor);
    return;
}

void disegnaSlider(Slider sld){
    unsigned short oldcolor = getTextColor();
    textcolor(sld.color);
    int y = sld.y;
    for (int i = 0; i < 3; i++){
        gotoxy(sld.x, y);
        printf("%c",sld.car);
        y++;
    }
    setTextColor(oldcolor);
    return;
}


void spostaPalla(Palla *p, int *dx, int *dy, Slider sld1, Slider sld2, int velocita){
    delay(velocita);
    cancellaPalla(*p);

    p->x += *dx;
    p->y += *dy;
    
    //se tocca il bordo in alto o in basso devo cambiare direzione della y
    if (toccaBordoY(*p))
        *dy = *dy * (-1);
    //se tocca il bordo a destra o a sinistra devo cambiare direzione della x
    if (toccaBordoX(*p))
        *dx = *dx * (-1);
    if (toccaSlider(*p,sld1,1) || toccaSlider(*p,sld2,-1)){
        *dx = *dx * (-1);
        Beep(700,150);
    }
}

//dice se la palla ha toccato i bordi sopra o sotto
bool toccaBordoY(Palla p){
    return (p.y >= HEIGHT - 2 || p.y <= 1);
}

//dice se la palla ha toccatoi bordi a dx o a sx
bool toccaBordoX(Palla p){
    return (p.x >= WIDTH - 1 || p.x <= 0);
}

void cancellaPalla(Palla p){
    gotoxy(p.x,p.y);
    printf(" ");
    return;
}

void muoviSlider(Slider *sld, int direction){
    cancellaSlider(*sld);
    if (sld->y + direction >= 1 && sld->y + sld->dim + direction <= 29)
        sld->y += direction;
    disegnaSlider(*sld);
    return;
}

void cancellaSlider(Slider sld){
    int y = sld.y;
    for (int i = 0; i < sld.dim; i++){
        gotoxy(sld.x,y);
        printf(" ");
        y++;
    }
    return;
}


bool toccaSlider(Palla p, Slider sld, int nplayer){
    return ((p.x == sld.x + nplayer && p.y >= sld.y-1 && p.y <= sld.y + sld.dim + 1) ? true : false);
}

int controllaPunto(Palla p){
    if (p.x <= LIMSX)  return 1;
    else if (p.x >= LIMDX) return 2;
    else return 0;
}

void stampaPunti(int punti1, int punti2, char nome1[], char nome2[]){
    gotoxy(58-strlen(nome1),0);
    printf("%s 0%d 0%d %s",nome1,punti1,punti2,nome2);
    return;
}

void finepartita(int *punti1, int *punti2, char nome1[], char nome2[]){
    clrscr();
    int x=0,y=12;
    for(x;x<WIDTH;x++){
        gotoxy(x,y);
        printf("%c",CONTORNO_ORIZZONTALE);
    }
    y=19;
    for(x=0;x<WIDTH;x++){
        gotoxy(x,y);
        printf("%c",CONTORNO_ORIZZONTALE);
    }

    gotoxy(55,15);
    if(*punti1 == *punti2) printf("pareggio!");
    else printf("%s hai vinto!", (*punti1 > *punti2) ? nome1 : nome2);
    gotoxy(55,16);
    printf("%s: %d",nome1,*punti1);
    gotoxy(55,17);
    printf("%s: %d",nome2,*punti2);
    Beep(1000,1000);
    salvaDati(*punti1,*punti2,nome1,nome2);
    *punti1 = 0;
    *punti2 = 0;
    getch();
    return;
}

void pausa(){
    getch();
    return;
}




void partita(int *punto1, int *punto2){
    cursor_show();
    clrscr();

    char nome1[LUNG_NOME];
    char nome2[LUNG_NOME];
    
    chiediNomi(nome1,nome2);
    cursor_hide();
    clrscr();
    Palla ball;
    Slider plr1, plr2;
    int dx = 1;
    int dy = 1;
    int key;
    bool fine = false;
    int velocita = 120;

    creaPalla(&ball);
    creaSlider(&plr1,1);
    creaSlider(&plr2,0);
    disegnaSlider(plr1);
    disegnaSlider(plr2);
    disegnaCampo();
    stampaPunti(*punto1,*punto2, nome1, nome2);
    do{
        disegnaLineeCampo();
        disegnaSlider(plr1);
        disegnaSlider(plr2);
        disegnaPalla(ball);
        if (kbhit()){
            key = getch();
            switch (key){
                case I_MIN:
                case I:
                    muoviSlider(&plr2,-1);
                    break;
                case K_MIN:
                case K:
                    muoviSlider(&plr2,1);
                    break;
                case W_MIN:
                case W:
                    muoviSlider(&plr1,-1);
                    break;
                case S_MIN:
                case S:
                    muoviSlider(&plr1,1);
                    break;
                case ESC:
                    key = getch();
                    if (key == ESC) fine = true;
                default:
                    break;
            }
        }
        spostaPalla(&ball,&dx,&dy,plr1,plr2,velocita);
        switch (controllaPunto(ball)){
            case 0:
                break;
            case 1:
                *punto2+=1;
                creaPalla(&ball);
                if (velocita > 50) velocita -= 15;
                Beep(500,1000);
                stampaPunti(*punto1,*punto2, nome1, nome2);
                pausa();
                break;
            case 2:
                *punto1+=1;
                creaPalla(&ball);
                if (velocita > 50) velocita -= 15;
                Beep(500,1000);
                stampaPunti(*punto1,*punto2, nome1, nome2);
                getch();
                break;
            default:
                break;
        }
        
    }while(*punto1 < 5 && *punto2 < 5 && !fine);
    finepartita(punto1,punto2,nome1,nome2);
    clrscr();
    return;
}

void disegnaCursore(Cursor c){  //stampa il cursore
	gotoxy(c.x,c.y);
	printf("%c",c.car);
	return;
}

void creaCursore(Cursor *c){   //imposta le impostazioni di default per il cursore
	c->car = '>';
	//le coordinate iniziali sono quelle della prima opzione nel menù
	c->x = 46;
	c->y = 11;
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

void spostaCursore(Cursor *c, int dir){  //si occupa di spostare il cursore se è il caso
    cursor_hide();
	cancellaCursore(*c);
    c->y = (c->y + dir) % (dir == -1 ? 6 : 5) + 11;
    if (c->y % 2 == 0) c->y -= dir;
	disegnaCursore(*c);  //stampa il cursore nella nuova posizione
	return;
}

void visualizzaRegistro(){
    clrscr();
    gotoxy(60-(strlen("REGISTRO PARTITE")/2),1);
    printf("REGISTRO PARTITE");
    gotoxy(0,2);
    for(int i = 0; i < WIDTH; i++)
        printf("-");
    int punti1,punti2;
    char nome1[LUNG_NOME];
    char nome2[LUNG_NOME];
    int anno,mese,giorno,ora,min,sec;
    FILE *fp;
    int k = 3;
    if ((fp = fopen("registro.dat","r")) == NULL){
        gotoxy(55,k);
        printf("non hai ancora giocato partite");
    }else{
        while (fscanf(fp,"%d-%d-%d %d:%d:%d %s %d %s %d",&anno, &mese, &giorno, &ora, &min, &sec, nome1,&punti1,nome2,&punti2) != EOF && k <= HEIGHT-2){
            gotoxy(35,k++);
            printf("%d-%02d-%02d %02d:%02d:%02d %s vs %s punteggio: %d-%d",anno,mese,giorno,ora,min,sec, nome1,nome2,punti1,punti2);
            gotoxy(0,k++);
            for(int i = 0; i < WIDTH; i++)
                printf("-");
        }
        fclose(fp);
    }
    getch();
    clrscr();
    return;
}

void salvaDati(int punti1, int punti2, char nome1[], char nome2[]){
    FILE *fp = fopen("registro.dat","a");
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(fp,"%d-%02d-%02d %02d:%02d:%02d %s %d %s %d\n",tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, nome1,punti1,nome2,punti2);
    fclose(fp);

    invertifile("registro.dat");
    return;
}

void disegnaCornice(){
    int x = 0, y = 0;
    for(y; y < HEIGHT-1; y++){
        gotoxy(x,y);
        printf("%c",CONTORNO_VERTICALE);
    }
    for(x; x < WIDTH-1; x++){
        gotoxy(x,y);
        printf("%c",CONTORNO_ORIZZONTALE);
    }
    for(y; y >= 1; y--){
        gotoxy(x,y);
        printf("%c",CONTORNO_VERTICALE);
    }
    for(x; x >= 0; x--){
        gotoxy(x,y);
        printf("%c",CONTORNO_ORIZZONTALE);
    }

    gotoxy(0,0);
    printf("%c",ANGOLO_ALTO_SX);
    
    gotoxy(WIDTH-1,0);
    printf("%c",ANGOLO_ALTO_DX);
    
    gotoxy(0,HEIGHT-2);
    printf("%c",ANGOLO_BASSO_SX);
    
    gotoxy(WIDTH-1,HEIGHT-2);
    printf("%c",ANGOLO_BASSO_DX);

    return;
}

void chiediNomi(char nome1[],char nome2[]){
    int x=0,y=10;

    for(x;x<WIDTH;x++){
        gotoxy(x,y);
        printf("%c",CONTORNO_ORIZZONTALE);
    }
    y=17;
    for(x=0;x<WIDTH;x++){
        gotoxy(x,y);
        printf("%c",CONTORNO_ORIZZONTALE);
    }

    gotoxy(35,12);
    printf("inserisci il nome del primo giocatore: ");
    gotoxy(35,15);
    printf("inserisci il nome del secondo giocatore: ");

    gotoxy(35+strlen("inserisci il nome del primo giocatore: "),12);
    fflush(stdin);
    scanf("%s",nome1);
    Beep(750,500);

    gotoxy(35+strlen("inserisci il nome del secondo giocatore: "),15);
    fflush(stdin);
    scanf("%s",nome2);
    Beep(750,100);
    
    return;
}

void invertifile(char nomefile[]){
    
    char riga[DIM_STR][LUNG_STR];
    int k = 0;
    FILE*fp;
    FILE*fout;

    fp=fopen(nomefile,"r");
    while(fgets(riga[k],LUNG_STR,fp) != NULL && k < DIM_STR)
        k++;
    fclose(fp);

    remove(nomefile);

    fout = fopen(nomefile,"w");
    for(k; k >= 0; k--)
        fprintf(fout,"%s",riga[k]);
    fclose(fout);

    return;
}
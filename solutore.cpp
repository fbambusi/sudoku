#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#define path "C:/Users/Fulvio/Desktop/Java/sudoku/sudoku"
#define si 1
#define no 2
#define bo 0
#define dimensione 3
#define dim dimensione*dimensione
typedef struct nod{//la lista così definita conterrà la coda delle caselle da controllare
	int riga;
	int colonna;
	nod* next;
}nodo;
typedef nodo* lista;//è una CODAH
void stampa(lista a){
	if(a==NULL){
		printf("-|\n");
			return;
	}
	else{
		printf("%d=%d-",a->riga,a->colonna);
		stampa(a->next);
	}

}
void importa(int numeroSchema,int contenitore[dim][dim],int brutta[dim][dim][dim]){//i parenti certi
	FILE* ingresso;//il file da cui entra il sudoku, e su cui verrà riportato lo schema risolto
	FILE*uscita;
	char nomeFile[strlen(path)+3],nomeUscita[strlen(path)+12];//la stringa con il nome completo del file
	char buffer[11];//ci carico sopra una riga alla volta
	char buffo;
	sprintf(nomeFile,"%s%d.txt",path,numeroSchema);
    ingresso=fopen(nomeFile,"r");//tento l'apertura del file
    sprintf(nomeUscita,"%s%d_solved.txt",path,numeroSchema);
    uscita=fopen(nomeUscita,"w"); 
    if(ingresso!=NULL){//il file è stato aperto
	 int numeroSchema,riga=0,colonna=0;
	 int cont,cont2;
  
     while(riga<dim){
     	colonna=0;
     	while(colonna<9){//in larghezza pesa 10 perché che il \n
     	
		buffo=fgetc(ingresso);           //legge un carattere
		fprintf(uscita,"%c ",buffo);
		buffo-='0';
	    if(buffo>=1&&buffo<=dim){//se è un numero
	    	contenitore[riga][colonna]=buffo;//lo annota in bella
	    	for(numeroSchema=0;numeroSchema<dim;numeroSchema++){
	    		if(numeroSchema+1==buffo){
	    			brutta[numeroSchema][riga][colonna]=si;
				}
				else{
					brutta[numeroSchema][riga][colonna]=no;
				}
			}
		}
		colonna++;

}
riga++;
fgetc(ingresso);
fprintf(uscita,"\n");
}
     fclose(ingresso);
     fclose(uscita);
	}
	else{//il file non è stato aperto
	 printf("error!\nFile %s couldn't be read'",nomeFile);
	}
}
//fine importa

int vediSchemi(int brutta[dim][dim][dim]){
	int numeroSchema,riga,colonna;
	for(numeroSchema=0;numeroSchema<dim;numeroSchema++){
		printf("Lo schema relativo al numero %d\n\n",numeroSchema+1);
		for(riga=0;riga<dim;riga++){
			for(colonna=0;colonna<dim;colonna++){
				printf("%d ",brutta[numeroSchema][riga][colonna]);
			}
			printf("\n");
			}
		printf("\n");
		}
	}
	
lista inserisci(int riga,int colonna,lista attiva){//aggiunge in coda un nodo coi campi pari ai parametri

	if(attiva==NULL){	
	lista a=(lista)malloc(sizeof(nodo));
	a->colonna=colonna;
	a->riga=riga;
	a->next=NULL;
		return a;
}
else if(attiva->riga!=riga||attiva->colonna!=colonna)
attiva->next=inserisci(riga,colonna,attiva->next);
return attiva;
}

lista elimina(lista attiva){//toglie il primo nodo della lista
	if(attiva!=NULL){
	lista tmp=attiva;
	attiva=attiva->next;
	free(tmp);}
	return attiva;
}

int unica(int schemino[dim][dim],int riga,int colonna){//restituisce 1 se è unica su riga, colonna o quadrato. 
	int cont,rigaAux,colAux;
	int quanteIncerteRiga=0,quanteIncerteColonna=0,quanteIncerteQuadrato=0;
	for(cont=0;cont<dim;cont++){
		if(schemino[riga][cont]==bo)
		quanteIncerteRiga++;
		
		if(schemino[cont][colonna]==bo)
		quanteIncerteColonna++;
		rigaAux=(riga/dimensione)*dimensione+cont%dimensione;
		colAux=(colonna/dimensione)*dimensione+cont/dimensione;
		if(schemino[rigaAux][colAux]==bo)
		quanteIncerteQuadrato++;
	
	}
	if(quanteIncerteRiga==1||quanteIncerteColonna==1||quanteIncerteQuadrato==1){
		
				return 1;//è unica sul numero in questione
	}

	return 0;
	
}

int haParentiCerti(int schemino[dim][dim],int riga,int colonna){//ritorna 1 se ha parenti certi, 0 se non ne ha
	int cont,rigaAux,colAux;
	for(cont=0;cont<dim;cont++){
		
		if(schemino[riga][cont]%3==si&&cont!=colonna)
		return 1;
		if(schemino[cont][colonna]%3==si&&cont!=riga)
		return 1;
		rigaAux=(riga/dimensione)*dimensione+cont%dimensione;
		colAux=(colonna/dimensione)*dimensione+cont/dimensione;
		if(schemino[rigaAux][colAux]%3==si&&(rigaAux!=riga||colAux!=colonna))
		return 1;		
	}
	return 0;
}



int certa2(int schemino[dim][dim][dim],int bella[dim][dim],int riga,int colonna,int livHp){//SBAGLIATA
	int numeroSchema,quanti=0;
	for(numeroSchema=0;numeroSchema<dim;numeroSchema++){  //conta quante inceret ho tra quelle caselle
		if(schemino[numeroSchema][riga][colonna]==bo){
			quanti++;
		}
	}
	if(quanti==1){//se è una sola, evidentemente è certa
		for(numeroSchema=0;numeroSchema<dim;numeroSchema++){//li ripercorro (ahimè)
			if(schemino[numeroSchema][riga][colonna]==bo){//se becco quella incerta la affermo
				schemino[numeroSchema][riga][colonna]=si+6*livHp;
				bella[riga][colonna]=numeroSchema+1; //e la annoto pure in bella
			}
			else{//se becco le altre le nego
			schemino[numeroSchema][riga][colonna]=no+6*livHp;
			}
		}
		return 1;
	}
	else     //non è unica
	return 0;
}

lista aggiungiParentiIncerti(int bella[dim][dim],int riga,int colonna,lista controllo){
	int cont,rigAux,colAux;
	for(cont=0;cont<dim;cont++){
		if(bella[riga][cont]==bo)//se il primo parente di riga è incerto aggiungilo
		controllo=inserisci(riga,cont,controllo);
		if(bella[cont][colonna]==bo)//se il primo parente di colonna è incerto aggiungili
	    controllo=inserisci(cont,colonna,controllo);
	    rigAux=(riga/dimensione)*dimensione+cont%dimensione;
	    colAux=(colonna/dimensione)*dimensione+cont/dimensione;
		if(bella[rigAux][colAux]==bo)//se il primo parente di quadrato è incertyo aggiungilo
		controllo=inserisci(rigAux,colAux,controllo);		
	}
	return controllo;
}

int finitoGiusto(int bella[dim][dim]){//ritorna 1 se è finito correttamente, 0 se non finito o sbagliato
	int cont,cont2;//cont indica il numero di riga, colonna o quadrato; cont2 il numero della casella all'interno di tale insieme(ordinate come leggendo)
	int numero;
	int trovatoRiga,trovatoColonna,trovatoQuadrato=0;
	for(numero=1;numero<dim+1;numero++){
	
	for(cont=0;cont<dim;cont++){	
		trovatoRiga=0;
		trovatoColonna=0;
		trovatoQuadrato=0;			
			for(cont2=0;cont2<dim;cont2++){
				if(bella[cont][cont2]==numero)
				trovatoRiga=1;
				if(bella[cont2][cont]==numero)
				trovatoColonna=1;
				if(bella[(cont/dimensione)*dimensione+cont2/dimensione][(cont%dimensione)*dimensione+cont2%dimensione]==numero)
				trovatoQuadrato=1;
			}
		if(trovatoRiga==0||trovatoColonna==0||trovatoQuadrato==0)//se alla fine dello scorrimento di riga, colonna o quadrato cont-esimi non ha trovato un numero, tutto lo skema erra
			return 0;
		}		
	}
	return 1;
}

lista risolvi3(lista controllo,int bella[dim][dim],int brutta[dim][dim][dim],int livHp){//trae le conclusioni su uno schema ipotetico
int numeroSchema;
int riga;
int colonna;
int cont;
int accertata;//variabile di controllo

while(controllo!=NULL){	
numeroSchema=0;
riga=controllo->riga;
colonna=controllo->colonna;
accertata=0;//0 se la casella è incerta-e quindi deve lavorare-1 se certa e quindi ignorabile
controllo=elimina(controllo);
while(numeroSchema<dim&&accertata==0){//per ogni schema
	if(brutta[numeroSchema][riga][colonna]==bo) {//solo se la casella è incerta sullo schema corrente//
	if(haParentiCerti(brutta[numeroSchema],riga,colonna)){//ha parenti certi? <------elimina il parente certo solo su questo schema; magari ne dovrebbe levare altri. no; non ne può restare meno di uno
	brutta[numeroSchema][riga][colonna]=no+livHp*6;//se sì, negala                                          
    if(certa2(brutta,bella,riga,colonna,livHp)){//ora è certa?      <-------------------------------------------------      FUNZIONE AGGIORNATA
    accertata=1;
	}
	controllo=aggiungiParentiIncerti(bella,riga,colonna,controllo);      // <-------------------------------------funzione AGGIORNATA                               
}
	else if(unica(brutta[numeroSchema],riga,colonna)){  //se è unica (per riga, colonna o quadrato), ovvero: è l'unico zero della riga;cazzata:unico zero, e no uni<------------------funzione aggiornata
	accertata=1;
	for(cont=0;cont<dim;cont++){//allora nega gli altri, e afferma qui
		if(numeroSchema==cont){
			brutta[numeroSchema][riga][colonna]=si+6*livHp;
			bella[riga][colonna]=numeroSchema+1;
			//printf("ho messo il numero %d in posizione %d %d per unicità\n",numeroSchema+1,riga,colonna);
			controllo=aggiungiParentiIncerti(bella,riga,colonna,controllo);
		}
		else
		brutta[cont][riga][colonna]=no+livHp*6;
		}
	}
}
else if ((brutta[numeroSchema][riga][colonna]%3==si)){

	accertata=1;
}	
numeroSchema++;	
}
}
return controllo;
}


void risolvi2(lista controllo,int bella[dim][dim],int brutta[dim][dim][dim]){
int cicliFatti=0;
int numeroSchema;
int riga;
int colonna;
int cont;
int accertata;//variabile di controllo
int rigaAux,colAux;

while(controllo!=NULL){
numeroSchema=0;
riga=controllo->riga;
colonna=controllo->colonna;
accertata=0;//0 se la casella è incerta-e quindi deve lavorare-1 se certa e quindi ignorabile
printf("cicli fatti: %d\n",cicliFatti);
cicliFatti++;
controllo=elimina(controllo);

while(numeroSchema<dim&&accertata==0){//per ogni schema
	if(brutta[numeroSchema][riga][colonna]==bo) {//solo se la casella è incerta sullo schema corrente
	if(haParentiCerti(brutta[numeroSchema],riga,colonna)){//ha parenti certi?	
	brutta[numeroSchema][riga][colonna]=no;//se sì, negala
    if(certa2(brutta,bella,riga,colonna,0)){//ora è certa?
    accertata=1;
    
    //se sì, esce dal ciclo; tanto ha già aggiuornato tutto l'altra funzione 
	}
	controllo=aggiungiParentiIncerti(bella,riga,colonna,controllo);
}
	else if(unica(brutta[numeroSchema],riga,colonna)){  //se è unica (per riga, colonna o quadrato), ovvero: è l'unico zero della riga;cazzata:unico zero, e no uni
	accertata=1;
	for(cont=0;cont<dim;cont++){//allora nega gli altri, e afferma qui
		if(numeroSchema==cont){
			brutta[numeroSchema][riga][colonna]=si;
			bella[riga][colonna]=numeroSchema+1;
			controllo=aggiungiParentiIncerti(bella,riga,colonna,controllo);
		}
		else
		brutta[cont][riga][colonna]=no;
		}
	}	
}
else if ((brutta[numeroSchema][riga][colonna]==si)){//modulo inutile

	accertata=1;
}
numeroSchema++;	
}//finito scorrimento schemi
}
//ho finito di leggere la lista

for(riga=0;riga<dim;riga++){
	for(colonna=0;colonna<dim;colonna++){
		printf("%d ",bella[riga][colonna]);
	}
	printf("\n");
}
//scanf("%d");
if(finitoGiusto(bella)==1){//se è giusto allora torna indietro con fiducia
printf("finito solo costruendo!\n");
return;}
int quanteMesse=0;
int livHp=0;//0 verità, e a crescere

while(quanteMesse!=81&&livHp>=0){
int totaleInsiemi=3*dim;
int incerti[totaleInsiemi];//prima le nove righe, poi le novce colonne, poi i 9 quadrtai
int indiceMin=-1;//la posizione occupata su "incerti" dall'insieme con meno zeri
int cont2;//una volta fissato un insieme, serve per scorrerlo
int messo=0;//diventa 1 non appena scrivo l'ipotesi
int completo=0;//una volta tratte le conclusioni, per verificare se è completo

livHp++;
//printf("sto costruendo l'ipotesi di livello numero %d, corrispondente a %d numeri\n",livHp,quanteMesse);


for(cont=0;cont<totaleInsiemi;cont++){
incerti[cont]=0;
		
}//ho inizializzato il tutto, al massimo (essendo che dovrò trovare il minimo

for(riga=0;riga<dim;riga++){
	for(colonna=0;colonna<dim;colonna++){
		if(bella[riga][colonna]==0){
		incerti[riga]++;
		incerti[colonna+dim]++;
		incerti[2*dim+(riga/dimensione)*dimensione+colonna/dimensione]++;		
		}
		
	}
}//ho annotato quanti incerti ci sono in ogni riga, colonna e quadrato

for(cont=0;cont<totaleInsiemi;cont++){
	if(indiceMin==-1&&incerti[cont]!=0||(incerti[cont]<=incerti[indiceMin]&&incerti[cont]>0)){
		indiceMin=cont;
	}
}
//adesso indiceMin indica la riga, la colonna o il quadrato con meno caselle incerte(che sono cmq più di una)
if(indiceMin<dim){//se è una riga
//printf("è una riga %d\n",indiceMin+1);

	for(colonna=0;colonna<dim&&messo==0;colonna++){//su tutte le colonne della riga<--------------------------------------------------EVVATO!
		for(numeroSchema=0;numeroSchema<dim;numeroSchema++){
	if(brutta[numeroSchema][indiceMin][colonna]==0){
		if(messo==0){
			brutta[numeroSchema][indiceMin][colonna]=si+3+(livHp-1)*6;   //lo schema è questo: bo si no ics || origine no ics siCond noCond ics || origine no ics siCond noCond ics
	        bella[indiceMin][colonna]=numeroSchema+1;                    //                    realtà=0            prima ipotesi=1                          seconda ipotesi=2	
	        messo=1;                                                      //                   0  1  2   3        4     5  6    7       8     9       10   11  12  13     14     15 
	        controllo=aggiungiParentiIncerti(bella,indiceMin,colonna,controllo);	
		}
		else{   
		brutta[numeroSchema][indiceMin][colonna]=no+6*livHp;                                                         
		
		}       //importante: ho modificato pure la bella.                                                          
		
	}
		
	}
}
}

else if(indiceMin<2*dim){//una colonna
//printf("è una colonna %d\n",indiceMin-8);
	for(riga=0;riga<dim&&messo==0;riga++){
for(numeroSchema=0;numeroSchema<dim;numeroSchema++){

	if(brutta[numeroSchema][riga][indiceMin-dim]==0){
		if(messo==1){
			brutta[numeroSchema][riga][indiceMin-dim]=no+6*livHp;
		}
		else{                                                             //                  0  1  2   3        4     5  6    7       8     9      10    11  12  13     14     15 
			brutta[numeroSchema][riga][indiceMin-dim]=si+3+(livHp-1)*6;   //lo schema è questo: bo si no ics || origine no ics siCond noCond ics || origine no ics siCond noCond ics
	        bella[riga][indiceMin-dim]=numeroSchema+1;                    //                    realtà=0            prima ipotesi=1                          seconda ipotesi=2
			messo=1;	
			controllo=aggiungiParentiIncerti(bella,riga,indiceMin-dim,controllo);
		}       //importante: ho modificato pure la bella.                                                          
		
	}
		
	}
}

//printf("ecco la bella sotto l'ipotesi numero %d\n",livHp);

}
else{//un quadrato
printf("è una quadrato %d\n",indiceMin-17);
	for(cont=0;cont<dim&&messo==0;cont++){
for(numeroSchema=0;numeroSchema<9;numeroSchema++){

	rigaAux=((indiceMin-2*dim)/dimensione)*dimensione+cont/dimensione;
	colAux=((indiceMin-2*dim)%dimensione)+cont%dimensione;	
	if(brutta[numeroSchema][rigaAux][colAux]==0){
	if(messo==0){
		brutta[numeroSchema][rigaAux][colAux]=si+3+(livHp-1)*6;
		bella[rigaAux][colAux]=numeroSchema+1;
		messo=1;
		controllo=aggiungiParentiIncerti(bella,rigaAux,colAux,controllo);
	}	
	else{
		brutta[numeroSchema][rigaAux][colAux]=no+6*livHp;
	}
	}	

	}
}

}//a questo punto ho la lista controllo che contiene tutto ciò che potrebbe essere cambiato nel frattempo, e gli schemi sommariamente aggiornati
//scanf("%d",&completo);
if(messo==0)//se non ho inserito alcun numero, di certo ho sbagliato
livHp--;
else{
controllo=risolvi3(controllo,bella,brutta,livHp);//ho tratto tutte le conclusioni possibili

}

completo=1;
for(riga=0;riga<dim&&completo;riga++){
	for(colonna=0;colonna<dim&&completo;colonna++){
		if(bella[riga][colonna]==0)
		completo=0;                 //se in bella ci sono ancora zeri, lo annota 
	}
}

if(messo==0||completo){//se è completo.... (o necessariamente errato)
//printf("finisc\n");
	if(finitoGiusto(bella))//...e giusto
	return;//ho finito!
	else{//... e sbagliato, sono nei cazzi
	for(numeroSchema=0;numeroSchema<dim;numeroSchema++){
	
		for(riga=0;riga<dim;riga++){
			for(colonna=0;colonna<dim;colonna++){
				if(brutta[numeroSchema][riga][colonna]==si+3+6*(livHp-1)){//se è l'origine'
				brutta[numeroSchema][riga][colonna]=no+6*(livHp-1);
				bella[riga][colonna]=bo;	
				}
				else if(brutta[numeroSchema][riga][colonna]==si+6*livHp){//è un si consecutorio
				brutta[numeroSchema][riga][colonna]=bo;
				bella[riga][colonna]=bo;
				}
				else if(brutta[numeroSchema][riga][colonna]==no+6*livHp){//è un no consecutorio
					brutta[numeroSchema][riga][colonna]=bo;
					
				}
			}
		}
	}
	livHp--;//e ho tolto un piano al castello

}}
quanteMesse=0;
for(riga=0;riga<dim;riga++){
	for(colonna=0;colonna<dim;colonna++){
		if(bella[riga][colonna]!=0)
		quanteMesse++;
	}
}
//printf("attualmente ho messo %d numeri\n",quanteMesse);
}

}







int main(){
int brutta[dim][dim][dim],bella[dim][dim],riga,colonna,numSchema,numeroOutput;  //la matrice con i nove schemi dei numeri; 1 significa certo che si; 0 incerto; 2 certo che no;SEGUE quella che verrà stampata alla fine
lista controllo=NULL;
FILE *uscita;
char nomeUscita[strlen(path)+23];
printf("For your eyes only mlmlmlmlmlmml\nInserisci il numero dello schema di cui desideri la soluzione e premi INVIO.\n");
scanf("%d",&numeroOutput);
for(riga=0;riga<dim;riga++){//in questi tre cicli for annidati inizializzo a zero tutto
for (colonna=0;colonna<dim;colonna++){
bella[riga][colonna]=bo;
for(numSchema=0;numSchema<dim;numSchema++){
	brutta[numSchema][riga][colonna]=bo;
}
}
}//fine dei tre cicli

importa(numeroOutput,bella,brutta);//ora ho, su tutti gli schemi, le info di base; che cominci la soluzione!
printf("la bella come si presenta all'inizio del calacolo:\n");
for(riga=0;riga<dim;riga++){
	for(colonna=0;colonna<dim;colonna++){
		printf("%d ",bella[riga][colonna]);//<-----------------------------------------------------------------------
		if(bella[riga][colonna]==0)
		controllo=inserisci(riga,colonna,controllo);
	}
	printf("\n");
}   //ho pure inizializzato la coda
vediSchemi(brutta);

risolvi2(controllo,bella,brutta);
sprintf(nomeUscita,"%s%d_solved.txt",path,numeroOutput);
uscita=fopen(nomeUscita,"a");
fprintf(uscita,"\n*****************\n*****************\n\n");
printf("La soluzione dello schema; essa si trova anche nella stessa cartella del sudoku da risolvere.\n");
for(riga=0;riga<dim;riga++){
	for(colonna=0;colonna<dim;colonna++){
	printf("%d ",bella[riga][colonna]);
	fprintf(uscita,"%d ",bella[riga][colonna]);
	}
	printf("\n");
	fprintf(uscita,"\n");
}  
printf("1 se giusto, 0 se errato: %d\n",finitoGiusto(bella));

fclose(uscita);
//vediSchemi(brutta);
return 0;
}



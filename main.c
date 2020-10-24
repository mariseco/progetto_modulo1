//
//  ising_project.c
//  ising
//
//  Created by Maria Antonella Secondo on 24/10/2020.
//

#include <stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
int field[100][100];
int iflag;          //partenza caldo(1)/freddo(0)/precedente(altro)
int measures ;      //numero di misure
int i_decorrel;     //updating fra una misura e l'altra
float extfield;      //valore del campo magnetico esterno
float beta;           //valore di 1/(kT) = beta
int nlatt;     //grandezza reticolo
FILE *finput;
FILE *flattice;

void my_fscanf(FILE * input, int * x);
FILE * myfopen(char *name);
void inizializza_field();
void crea_lattice();

int main()
{
   //leggo file
    
    finput = fopen("input_ising.txt", "r");
    if (finput==NULL)
    {
        perror("Errore in apertura del file");
        exit(1);
    }
    
    flattice = fopen("input_lattice.txt", "r");
    if (finput==NULL)
    {
        perror("Errore in apertura del file");
        exit(1);
    }

    
    srand(time(NULL));

   //leggo gli input
    fscanf(finput, "%d\n", &nlatt);
    fscanf(finput, "%f\n", &beta);
    fscanf(finput, "%d\n", &iflag);
    fscanf(finput, "%f\n", &extfield);
    fscanf(finput, "%d\n", &measures);
    
    //printf("%d, %d, %d, %f, %f, %d",nlatt, iflag, measures, beta, extfield, rand());
    
    inizializza_field();
    
    crea_lattice();
    fclose(finput);
    fclose(flattice);
  
    return 0;
}

/*
 void inizializza_field() crea il lattice nxn in cui situiamo in ogni sito field[i][j] uno spin, 1 se è up o -1 se è down.
 L'obiettivo è inizializzarlo a uno stato iniziale.
 
 *iflag indica la tipica configurazione a determinate temperature.
 Il CASO [1] corrisponde alla temperatura nulla, in cui tutti gli spin o sono up o sono down. L'energia del mio sistema è invariante per simmetria di scambio, dunque è uguale porlo up o down, in questo caso ho scelto up.
 Il CASO [2] è tipico di una temperatura molto grande, maggiore della temperatura critica, in cui vi è disordine nel reticolo.
 Il CASO [3] 
 
 
 */
void inizializza_field()
{
    if(iflag==0)                            // CASO [1]
    {
        for(int i=0;i<nlatt;i++)
            for(int j=0;j<nlatt;j++)
                field[i][j]=1;
    }
    else if(iflag==1)                       //CASO [2]
    {
        for(int i=0;i<nlatt;i++)
            for(int j=0;j<nlatt;j++)
                field[i][j]=-1+2*(rand()%2);
    }
    else                                    //CASO [3]
    {
        for(int i=0;i<nlatt;i++)
           for(int j=0;j<nlatt;j++)
               fscanf(flattice,"%d\n" ,&field[i][j]);
    }
}


void crea_lattice()
{
    flattice = fopen("input_lattice.txt", "w");
    if (finput==NULL)
    {
        perror("Errore in apertura del file");
        exit(1);
    }
    for(int i=0;i<nlatt;i++)
       for(int j=0;j<nlatt;j++)
           fprintf(flattice,"%d\n",field[i][j]);
}







void my_fscanf(FILE * input, int * x)
{
    int lettura;
    lettura = fscanf(input, " %d\n", &lettura);
    
}

FILE * myfopen(char *name)
{
    FILE * f ;
    f = fopen("input_ising.txt", "r");
    if (f==NULL)
    {
        perror("Errore in apertura del file");
        exit(1);
    }
    return f ;
}

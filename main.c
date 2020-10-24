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
int measures       //numero di misure
int i_decorrel     //updating fra una misura e l'altra
float extfield       //valore del campo magnetico esterno
float beta           //valore di 1/(kT) = beta
int L;
float beta;


void my_fscanf(FILE * input, int * x);
FILE * myfopen(char *name);

int main()
{
   //leggo file
    FILE *finput;
    finput = fopen("input_ising.txt", "r");
    if (finput==NULL)
    {
        perror("Errore in apertura del file");
        exit(1);
    }
   //leggo gli input
    fscanf(finput, "%d\n", &L);
    fscanf(finput, "%f\n", &beta);
    fscanf(finput, "%d\n", &iflag);

    
    
    fclose(finput);
    return 0;
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

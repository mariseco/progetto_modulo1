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


void my_fscanf(FILE * input, int * x);
FILE * myfopen(char *name, char *mode);

int main()
{
    int L;
    FILE *input;
    
    input= myfopen("input_ising.txt", "r");
   
    
    
    my_fscanf(input, &L);
    
    printf("%d \n",L);
    fclose(input);
    return 0;
}


void my_fscanf(FILE * input, int * x)
{
    int lettura;
    lettura = fscanf(input, " %d\n", &lettura);
    
}

FILE * myfopen(char *name, char *mode)
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

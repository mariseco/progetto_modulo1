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

int main()
{
    int L, M;
    FILE *input;
        input=fopen("input_ising.txt", "r");
        if( input==NULL )
        {
            perror("Errore in apertura del file");
            exit(1);
        }
    
    fscanf(input, " %d\n", &L);
    printf("%d\n",L);
    fscanf(input, "%d\n", &L);
    printf("%d\n",L);

    fclose(input);
    return 0;
}

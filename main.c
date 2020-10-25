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
int field[100][100], npp[100], nmm[100];
int iflag;          //partenza caldo(1)/freddo(0)/precedente(altro)
int measures ;      //numero di step di questo run
int i_decorrel;     //updating fra una misura e l'altra
float extfield;      //valore del campo magnetico esterno
float beta;           //valore di 1/(kT) = beta
int nlatt;     //grandezza reticolo
int step_iniziale=0; //Questo è lo step iniziale già creato in un altro file (e' importante solo se iflag è diverso da 0 oppure 1
FILE *finput;
FILE *flattice;
FILE *fmisure;


/*
 Prototipi delle funzioni che saranno utilizzate nel codice
 */
void my_fscanf(FILE * input, int * x);
FILE * myfopen(char *name);
void inizializza_field();
void crea_lattice();
float magnetizzazion();
float energy();
void geometry();
void update_metropolis();

/*
 Corpo principale del progetto
 */
int main()
{
    long int time_inizio;
    time_inizio = time(NULL);
   //leggo file
    
    finput = fopen("input_ising.txt", "r");
    if (finput==NULL)
    {
        perror("Errore in apertura del file");
        exit(1);
    }
    
    flattice = fopen("input_lattice.txt", "r");
    if (flattice==NULL)
    {
        perror("Errore in apertura del file");
        exit(1);
    }

    fmisure = fopen("output.txt", "w");
    if (fmisure==NULL)
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
    fscanf(finput, "%d\n", &i_decorrel);
    //printf("%d, %d, %d, %f, %f, %d",nlatt, iflag, measures, beta, extfield, rand());
    
    inizializza_field();
    geometry();
    
    int step;
    float media_energy=0, energia, errore_energia=0;
    for(step=step_iniziale;step< step_iniziale+measures;step++)
    {
        for(int i=0;i<i_decorrel;i++)
            update_metropolis();
        
        energia= energy();
        media_energy+=energia;
        errore_energia += energia*energia;
        
        fprintf(fmisure,"%d %f %f\n",step, magnetizzazion(),energia);
    }
    
    media_energy=media_energy/step;
    errore_energia = (errore_energia/step) - media_energy*media_energy ;
    errore_energia = sqrt(errore_energia)/sqrt(step);
    printf("Questa è la media = %f \nQuesta è l'errore sbagliato della media dell'energia = %f \n", media_energy, errore_energia); //"sbagliato" perchè in realtà le varie energie sono correlate ;)
    printf("Il programma ha runnato in %ld secondi\n",time(NULL)-time_inizio);
    
    crea_lattice();
    
    
    // chiudo i file aperti
    fclose(finput);
    fclose(flattice);
    fclose(fmisure);
  
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
void inizializza_field(){
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
        fscanf(flattice,"%d\n" ,&step_iniziale);
        for(int i=0;i<nlatt;i++)
           for(int j=0;j<nlatt;j++)
               fscanf(flattice,"%d\n" ,&field[i][j]);
    }
}

float magnetizzazion(){
    float sum=0;
    for(int i=0;i<nlatt;i++)
        for(int j=0;j<nlatt;j++)
            sum+=field[i][j];
    
    return sum/(nlatt*nlatt);
}

void geometry(){
    for(int i=0; i<nlatt;i++)
    {
        npp[i]=i+1;
        nmm[i]=i-1;
    }
    npp[nlatt-1]= 0;
    nmm[0]= nlatt-1;
}

float energy(){
    int jp, jm,ip, im;
    float xene = 0, force, nvol;
    for(int i=0;i<nlatt;i++)
        for(int j=0;j<nlatt;j++)
        {
            ip = npp[i];
            im = nmm[i];
            jp = npp[j];
            jm = nmm[j];
            force= field[i][jm] + field[i][jp] + field[im][j] + field[ip][j];
            xene = xene - 0.5*force*field[i][j];
            xene = xene - extfield*field[i][j];
            
        }
    nvol = nlatt*nlatt;
    xene = (float)xene/nvol;
    return xene;
}

void crea_lattice(){
    flattice = fopen("input_lattice.txt", "w");
    if (finput==NULL)
    {
        perror("Errore in apertura del file");
        exit(1);
    }
    fprintf(flattice,"%d\n",step_iniziale+measures);
    for(int i=0;i<nlatt;i++)
       for(int j=0;j<nlatt;j++)
           fprintf(flattice,"%d\n",field[i][j]);
}


void update_metropolis(){
    int i,j;
    int jp, jm,ip, im;
    float force,p_rat, x;
    for(int ivol=0;ivol<nlatt*nlatt;ivol++)
    {
        i=rand()%nlatt;
        j=rand()%nlatt;
        
        ip = npp[i];
        im = nmm[i];
        jp = npp[j];
        jm = nmm[j];
        
        force= field[i][jm] + field[i][jp] + field[im][j] + field[ip][j];
        force=beta*(force+extfield);
        p_rat=exp(-2*field[i][j]*force);
        
        x=rand()/(RAND_MAX+1.0);
        if(x<p_rat)
            field[i][j]=-field[i][j];
    }
    
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

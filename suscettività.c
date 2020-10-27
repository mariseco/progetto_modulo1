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
float susc;
float m[100001];
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
float suscettività();
float energy();
void geometry();
void update_metropolis();
float error_suscettività();
float bootstrap(int bin);

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

    fmisure = fopen("suscettività20L.txt", "w");
    if (fmisure==NULL)
    {
        perror("Errore in apertura del file");
        exit(1);
    }
    
    srand(time(NULL));

    
   //leggo gli input
    nlatt=20;
    iflag=1;
    extfield=0;
    measures=10000;
    i_decorrel=100;
    beta=0.30;
    geometry();
    
    inizializza_field();//cancella
    susc=suscettività();
    for(int k=0;k<11;k++)
        fprintf(fmisure,"%d %f\n",k, bootstrap(pow(2,k)));
    
    //fino a qua
    
    //printf("%d, %d, %d, %f, %f, %d",nlatt, iflag, measures, beta, extfield, rand());
  /*  while(beta<=0.48)
    {
        inizializza_field();
        susc=suscettività();
        error_susc=error_suscettività();
        
        if(beta>0.40 && beta<0.47)
            beta=beta+0.0017;
        else
            beta=beta+0.008;
        
        fprintf(fmisure,"%f %f\n",beta, susc);
    }
    */
    
    
    printf("Il programma ha runnato in %ld secondi\n",time(NULL)-time_inizio);
    
    // chiudo i file aperti
    fclose(finput);
    fclose(flattice);
    fclose(fmisure);
  
    return 0;
}

float error_suscettività(){
    return 0;
}

float bootstrap(int bin){
    float media_m, media_m2, media_chi=0, media_chi2=0, temp;
    int i=0, q, zeta;
    for(int cont=0;cont<100;cont++)
    {
        media_m=0;
        media_m2=0;
        zeta=0;
        for(i=0;i<measures/bin;i++)
        {
            q=rand()%measures;
            for(int w=0;w<bin;w++)
            {
                media_m +=m[(q+w)%measures];
                media_m2 += m[(q+w)%measures]*m[(q+w)%measures];
                zeta++;
            }
            
        }
        printf("%d ma occhio che k= %d\n",measures-zeta,bin);
        media_m = media_m/zeta;
        media_m2 = media_m2/zeta;
        temp= nlatt*nlatt*(media_m2- media_m*media_m);
        media_chi += temp;
        media_chi2 += temp*temp;
    }
    media_chi=media_chi/100;
    media_chi2=media_chi2/100;
    return sqrt(media_chi2-media_chi*media_chi);
}

float suscettività(){
    int step;
    float media_magn=0, media_magn2=0, magn;
    for(step=0;step<measures;step++)
    {
        for(int i=0;i<i_decorrel;i++)
            update_metropolis();
        
        magn=magnetizzazion();
        if(magn<0)
            magn=-magn;
        m[step]=magn;
        media_magn += magn;
        media_magn2 += magn*magn;
    }
    media_magn=media_magn/measures;
    media_magn2=media_magn2/measures;
    return nlatt*nlatt*(media_magn2- media_magn*media_magn);
    
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

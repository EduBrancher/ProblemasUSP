/*  EP 2 - Sistemas Operacionais

Julia Leite
Eduardo Brancher

*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "ciclista.h"
#include <string.h>
#include <time.h>
 
typedef struct {

    int* rank;
    
    int pos;       			// colocação do prox ciclista no vetor rank
    int terminada; 			// se a volta foi encerrada
    int nao_comecaram;
    int comecaram;
    int terminaram;
    pthread_mutex_t semaforo;

}Volta;


Volta* voltas;
Volta colocacao_final;
void voltas_init ();
void voltas_free();
void end_lap (Ciclista* c);


// vetor de threads
pthread_t* threads;
void* th_ciclistas (void* aux); 

// barreira
int* arrive;
int* cont;
void init_barr ();

// matriz de mutex pra proteger as secoes criticas da pista
pthread_mutex_t * sem[10];
void sem_init ();
void sem_free ();

struct timespec t, a;
void def_interval ();
int CICLO = 60; // tempo de cada ciclo em ms

int n;                   // numero inicial de ciclistas
pthread_mutex_t n_mutex; // mutex que protege n_ciclistas
int n_ciclistas;         // numero atual de ciclistas
int d;                   // tamanho da pista em m
int tempo = 0;           // tempo atual em ms
int max_lap = 0; 
pthread_mutex_t max_mutex;

// Flags
int debug;      // se o usuario escolheu a opcao debug 

int* pista[10];
void pista_init();
void pista_free();
void pista_show();
void debug_prints();

void colocacao_parcial_show (int atual); // mostra a colocacao parcial na volta 'atual'
void colocacao_final_show ();            // mostra a colocacao final dos ciclistas 
void quebrados_show();                   // mostra os ciclistas que quebraram

// memoria
void get_memory_time();

struct timespec ini;
struct timespec tf;

int main (int argc, char* argv[]) {

    if (argc < 4) {
        printf ("Coloque a quantidade certa de argumentos!\n");
        printf ("./ep <d metros> <n ciclistas> <debug(0 ou 1)> <semente (opcional)>\n");
        exit(1);
    }

    debug = atoi(argv[3]);
    (argc == 5) ? (srand(atoi (argv[4]))) : (srand(time(NULL)));
    d = atoi(argv[1]);
    n = n_ciclistas = atoi(argv[2]);
    clock_gettime (CLOCK_MONOTONIC, &ini);

    int atual = 0; // volta a ser completada --> menor volta ativa
    int first = 0;
    int volta_90;

    // inicializamos:
    pista_init();                      // a pista
    ciclista_init(n_ciclistas, pista); // o vetor de ciclistas
    sem_init(d);                       // os semaforos
    init_barr();                       // a barreira
    voltas_init();                     // o vetor de voltas
    pthread_mutex_init(&n_mutex, NULL);//o mutex de n_ciclistas
    pthread_mutex_init(&max_mutex, NULL);
    
    // definimos o intervalo das nanosleep
    def_interval();

    // inicializamos o vetor de colocacao final
    colocacao_final.rank = malloc (n * sizeof(int));
    for (int i = 0; i < n; i++)
        colocacao_final.rank[i] = -1;
    colocacao_final.pos = n-1;
    pthread_mutex_init(&colocacao_final.semaforo, NULL);

    // criando as threads
    threads = malloc (n_ciclistas * sizeof (pthread_t));
    for (int i = 0; i < n_ciclistas; i++)
        pthread_create(&threads[i], NULL, th_ciclistas, (void *) &ciclista[i]);

    if (debug)
        pista_show();

    while (n_ciclistas > 1) {

        if (cic_90 == 1)
            CICLO = 20;

        tempo += CICLO;

        //BARREIRA 1: onde paramos as threads
        for (int i = 0; i < n; i++) {
            while (arrive[i] == 0 && !ciclista[i].quebrou && !ciclista[i].eliminado && !ciclista[i].terminou) { 
                nanosleep(&t, &a);
            }
            arrive[i] = 0;  
        }

        // volta atual acabou (pode ser mais de uma)
    	while (voltas[atual].nao_comecaram == 0 && voltas[atual].comecaram == voltas[atual].terminaram){

    		//volta concluida deve ser encerrada.
    		voltas[atual].terminada = 1;
            colocacao_parcial_show(atual);

    		if (atual % 2 == 1) { //volta PAR

                int i = 1;

                int cic_id = voltas[atual].rank[voltas[atual].terminaram - i]; //id do ultimo ciclista a terminar a volta

                while(i < voltas[atual].terminaram && (ciclista[cic_id].eliminado == 1 || ciclista[cic_id].eliminado_pendente == 1 || ciclista[cic_id].quebrou == 1)){
                    i++;
                    cic_id = voltas[atual].rank[voltas[atual].terminaram - i];
                }
                //agora cic_id e o id do ultimo ciclista que terminou a volta e ainda nao foi eliminado

    			//eliminacao
                if (ciclista[cic_id].pos == 0 && ciclista[cic_id].dt == 0){	
    			    ciclista[cic_id].eliminado = 1;
                    if (n_ciclistas > 1)
                        printf("Ciclista %d foi eliminado.\n", cic_id);
    			    n_ciclistas--;
                    for (int i = ciclista[cic_id].n_voltas; i < 2*n; i++) {
                        voltas[i].nao_comecaram--;
                    }

                }
                //se nao esta na linha de chegada, devemos esperar completar uma volta
                else {
                    ciclista[cic_id].eliminado_pendente = 1;
                    if (n_ciclistas > 1)
                        printf("Ciclista %d sera eliminado quando cruzar a linha de chegada.\n", cic_id);
                    for (int i = ciclista[cic_id].n_voltas + 1; i < 2*n; i++) {
                        voltas[i].nao_comecaram--;
                    }
                }
    	
                colocacao_final.rank[colocacao_final.pos] = cic_id;
                colocacao_final.pos--; 
    		}
    		atual++;
        }


        if (debug){
            pista_show();
        }

        // se ainda não sorteamos mas já podemos
        // se alg ja esta na volta 2n-3 ou se temos so 2 ciclista
        if (cic_90 == -1 && (max_lap >= 2*n - 3 || n_ciclistas <= 2)) {

            cic_90 = 0;
            
            for (int i = 0; i < n; i++)
                if (!ciclista[i].quebrou && !ciclista[i].eliminado && !ciclista[i].eliminado_pendente && !ciclista[i].terminou
                    && (ciclista[i].n_voltas > ciclista[first].n_voltas))
                    first = i;

            volta_90 = ciclista[first].n_voltas;

            int aux = rand() % 100;

            // alguem foi sorteado pra peladar a 90 km/h
            if (aux < 10) {

                // sorteamos qual pedalará a 90km/h
                aux = rand() % 100;

                if (aux < 50) {

                    ciclista[first].vel = 90;
                    cic_90 = 1;
                }
                
                else {
                    cic_90 = 2;
                }
            }
            
        }

        if (cic_90 == 2) {

            for (int i = 0; i < n; i++) {
                if (!ciclista[i].quebrou && !ciclista[i].eliminado && !ciclista[i].eliminado_pendente && !ciclista[i].terminou
                    && i != first && ciclista[i].n_voltas >= volta_90) {

                        ciclista[i].vel = 90;
                        cic_90 = 1;
                        break;
                    }
            }
        }

        // BARREIRA 2: deixamos as threads continuarem
        for (int i = 0; i < n; i++)
            cont[i] = 1;

    }

    //invariante: n_ciclistas <= 1
    void* status;
    for (int i = 0; i < n; i++){
        pthread_join(threads[i], &status);
        if (ciclista[i].eliminado != 1 && ciclista[i].quebrou != 1){
            //ciclista i venceu a corrida
            colocacao_final.rank[0] = ciclista[i].id;
        }
    }

    colocacao_final_show();
    quebrados_show();
    

    clock_gettime (CLOCK_MONOTONIC, &tf);

    get_memory_time();

    // Free's:
    // barreira
    free(arrive);
    free(cont);
    // threads
    free(threads);
    // vetor ciclista
    free(ciclista);
    // colocacao final
    pthread_mutex_destroy(&colocacao_final.semaforo);
    free(colocacao_final.rank);
    // vetor voltas  
    voltas_free();
    pista_free();
    sem_free();

}


// cria e inicaliza a pista
void pista_init () {

    for (int i = 0; i < 10; i++)
        pista[i] = malloc (d*sizeof(int));

    for (int i = 0; i < 10; i++)
        for (int j = 0; j < d; j++)
            pista[i][j] = -1;

}

// libera o espaco da pista e do semaforo
void pista_free() {
    for (int i = 0; i < 10; i++) 
        free(pista[i]);
}

// exibe os ciclistas na pista 
void pista_show() {

    printf ("Pista no instante %d\n\n", tempo);
    printf ("---------------------------------------------------------------------------------\n");
    printf ("|             L  A  R  G  A  D  A                                               |\n");
    printf ("---------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < d; i++) {

        printf ("|");
        for (int j = 0; j < 10; j++) {
        
            (pista[j][i] != -1 && ciclista[pista[j][i]].eliminado == 0 && ciclista[pista[j][i]].quebrou == 0) ? (printf (" %5d |", pista[j][i])) : (printf("       |"));
        
        }

        printf ("\n");

    }

    printf ("---------------------------------------------------------------------------------\n");
    printf ("|             C  H  E  G  A  D  A                                               |\n");
    printf ("---------------------------------------------------------------------------------\n\n");
}

void init_barr () {
    
    arrive = malloc (n_ciclistas * sizeof (int));
    cont = malloc (n_ciclistas * sizeof (int));

    for (int i = 0; i < n_ciclistas; i++)
        arrive[i] = cont[i] = 0;

}

void sem_init () {

    for (int i = 0; i < 10; i++) {

        sem[i] = malloc (d*sizeof(pthread_mutex_t));

        for (int j = 0; j < d; j++)
            pthread_mutex_init(&sem[i][j], NULL);
    }
}

void sem_free () {

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < d; j++){
            pthread_mutex_destroy(&sem[i][j]);
        }
        free(sem[i]);
    }

}

void def_interval () {

    t.tv_nsec = 5000;
    t.tv_sec = 0;

    a.tv_nsec = 0;
    a.tv_sec = 0;
}

void voltas_init() {

    voltas = malloc (2*n*sizeof(Volta));

    for (int i = 0; i < 2*n; i++) {

        voltas[i].pos = voltas[i].terminada = voltas[i].comecaram = voltas[i].terminaram = 0;
        voltas[i].nao_comecaram = n_ciclistas;
        voltas[i].rank = malloc((n-(i/2))*sizeof(int));

        for (int j = 0; j < n-(i/2); j++) 
            voltas[i].rank[j] = -1;

        pthread_mutex_init(&voltas[i].semaforo, NULL);
    }

    voltas[0].nao_comecaram = 0;
    voltas[0].comecaram = n;

}

void voltas_free() {
    for (int i = 0; i < 2*n; i++) {
        pthread_mutex_unlock(&voltas[i].semaforo);
        pthread_mutex_destroy(&voltas[i].semaforo);
        free(voltas[i].rank);
    }
    free(voltas);
}


void end_lap (Ciclista* c) {
        
    // atualizamos o ciclista
    update_ciclista(c->id, tempo, n_ciclistas); 

    int volta_terminada = c->n_voltas - 1;      // Ultima volta que ele terminou

    // atualiza o vetor voltas
    pthread_mutex_lock(&voltas[volta_terminada].semaforo);
    voltas[volta_terminada].terminaram++;
    voltas[volta_terminada].rank[voltas[volta_terminada].pos] = c->id;
    voltas[volta_terminada].pos++;
    pthread_mutex_unlock(&voltas[volta_terminada].semaforo);

    // se o ciclista já foi eliminado numa volta anterior, mas estava no meio da pista
    if (c->eliminado_pendente == 1){

        c->eliminado = 1;

        pthread_mutex_lock(&n_mutex);
        if (n_ciclistas > 1)
            printf("Ciclista %d foi eliminado quando cruzou a linha de chegada.\n", c->id);
        
        n_ciclistas--;
        pthread_mutex_unlock(&n_mutex);
    }

    // se o ciclista já completou as 2n-2 voltas
    if (c->n_voltas >= 2*n-2) 
        c->terminou = 1;
    
}

void * th_ciclistas (void* aux) {

    Ciclista* c;
    c = (Ciclista*) aux;

    while (!c->quebrou && c->eliminado == 0 && n_ciclistas > 1 && !c->terminou) {

        // comecou uma volta nova
        if (c->n_voltas != 0 && c->pos == 0 && c->dt == 0) {

            pthread_mutex_lock(&n_mutex);
            //vemos se quebrou
            if (c->n_voltas % 6 == 0 && n_ciclistas > 5)
                c->quebrou = broken();
            pthread_mutex_unlock(&n_mutex);

            if (c->quebrou) {
                printf ("Ciclista %d QUEBROU na volta %d :(\n", c->id, c->n_voltas); 
                
                // decrementamos o total de ciclistas ativos
                pthread_mutex_lock(&n_mutex);
                n_ciclistas--;
                pthread_mutex_unlock(&n_mutex);
                
                //Corrige o numero de ciclistas que vao comecar as proximas voltas
                for (int i = c->n_voltas; i < 2*n; i++) {
                    pthread_mutex_lock(&voltas[i].semaforo);
                    voltas[i].nao_comecaram--;
                    pthread_mutex_unlock(&voltas[i].semaforo);
                }

                break;
            }

            pthread_mutex_lock(&voltas[c->n_voltas].semaforo);
            voltas[c->n_voltas].comecaram++;
            voltas[c->n_voltas].nao_comecaram--;
            pthread_mutex_unlock(&voltas[c->n_voltas].semaforo);

            pthread_mutex_lock(&max_mutex);
            if (c->n_voltas > max_lap)
                max_lap = c->n_voltas;
            pthread_mutex_unlock(&max_mutex);
        }

        int ultrapassei = 0;
        c->dt += CICLO;

        // ele pode andar 1m
        if (c->dt >= 3600 / c->vel) {

            int prox = (c->pos + 1) % d;
            int old_pos = c->pos;

            pthread_mutex_lock(&sem[c->faixa][prox]);

            // Caso 1: não tem ninguem na frente
            if (pista[c->faixa][prox] == -1) {

                pthread_mutex_lock(&sem[c->faixa][c->pos]);
                pista[c->faixa][c->pos] = -1;
                pthread_mutex_unlock(&sem[c->faixa][c->pos]);

                c->pos = prox;
                pista[c->faixa][c->pos] = c->id;
                c->dt = 0; // ignoramos "quebrados" que podem ter ficado
                
                // se completou uma volta
                if (c->pos == 0)
                    end_lap(c);

                pthread_mutex_unlock(&sem[c->faixa][c->pos]);

            }

            // Caso 2: tem gente na frente
            else {

                pthread_mutex_unlock(&sem[c->faixa][prox]);

                // percorremos as faixas
                for (int f = c->faixa + 1; f < 10 && !ultrapassei && !c->quebrou; f++) { 

                    pthread_mutex_lock (&sem[f][old_pos]);
                    
                    // se a posição à direita está livre
                    if (pista[f][c->pos] == - 1) { 
                        
                        pthread_mutex_lock(&sem[f][prox]);

                        // ultrapassagem
                        if (pista[f][prox] == -1) {

                            pthread_mutex_lock(&sem[c->faixa][c->pos]);
                            pista[c->faixa][c->pos] = -1; 
                            pthread_mutex_unlock(&sem[c->faixa][c->pos]);
                            c->pos = prox;
                            c->faixa = f;
                            c->dt = 0;
                            pista[f][c->pos] = c->id;
                            
                            if (c->pos == 0)
                                end_lap(c);
                            
                            ultrapassei = 1;

                        }
                        pthread_mutex_unlock(&sem[f][prox]);
                    }                 
                    pthread_mutex_unlock (&sem[f][old_pos]);
                }

            }

        } //CERTO

        // barreira

        arrive[c->id] = 1;

        while (!cont[c->id]) {
            nanosleep(&t, &a);

        }
        cont[c->id] = 0;

    }

    //se saimos do while, ou quebramos ou fomos eliminados, devemos apagar nosso id da pista
    pthread_mutex_lock(&sem[c->faixa][c->pos]);
    pista[c->faixa][c->pos] = -1;
    pthread_mutex_unlock(&sem[c->faixa][c->pos]);
    c->terminou = 1;
    pthread_exit(0);

}

void debug_prints(int atual){


    printf("Valores de voltas:\n");
    printf("\nVolta atual: %d\n\n", atual);

    for (int i = atual; i < 2*n; i++){
        
        printf("Volta %d:\n", i);
        printf("Nao comecaram a volta: %d\n", voltas[i].nao_comecaram);
        printf("Comecaram a volta: %d\n", voltas[i].comecaram); 
        printf("Terminaram a volta: %d\n", voltas[i].terminaram);
        printf("Volta terminada: %d\n", voltas[i].terminada);
        if (voltas[i].terminada) {
            for (int j = 0; j < voltas[i].terminaram; j++)
                printf ("%d    ", voltas[i].rank[j]);
            printf("\n");
        }
    }



}

void get_memory_time() {

    FILE* arq;
    long long linha;
    double dt = tf.tv_sec - ini.tv_sec + (double) (tf.tv_nsec - ini.tv_nsec)/1000000000.0;

    arq = fopen("/proc/self/statm", "r"); 

    fscanf (arq, "%lld", &linha);

    fprintf (stdout, "\n\nConsumo de memoria e tempo de execucao:\n\n");
    fprintf (stdout, "Uso de memoria em pages: %lld\n", linha);
    fprintf (stdout, "Uso de memoria em kB: %lld\n", linha*4);
    fprintf (stdout, "Tempo de execução: %.3lf\n", dt);

    fclose(arq);

}

void colocacao_parcial_show (int atual) {

    printf ("\nVolta %d encerrada\n", atual);

    printf ("Colocação dos ciclistas:\n");
    for (int i = 0, c = 1; i < voltas[atual].terminaram; i++)
        if (!ciclista[voltas[atual].rank[i]].eliminado && !ciclista[voltas[atual].rank[i]].eliminado_pendente && (!ciclista[voltas[atual].rank[i]].quebrou || ciclista[voltas[atual].rank[i]].n_voltas > atual)) 
            printf ("%do : Ciclista %d\n", c++, voltas[atual].rank[i]);	 

}

void colocacao_final_show () {
    
    printf("\nColocacao final:\n\n");

    int lugar = 1; 
    int first = -1;
    for (int i = 0; i < n; i++){
        if (colocacao_final.rank[i] != -1) {

            if (colocacao_final.rank[i] != first)
                printf("%6do colocado: Ciclista %6d   -   ultima chegada(ms): %d\n", lugar++, colocacao_final.rank[i], ciclista[colocacao_final.rank[i]].t_chegada);            

            if (first == -1)
                first = colocacao_final.rank[i];
        }
    }
}

void quebrados_show () {

    printf ("\nCiclistas que quebraram:\n\n");

    for (int i = 0; i < n; i++){
        if (ciclista[i].quebrou == 1){
            printf("Ciclista %d quebrou na volta %d\n", ciclista[i].id, ciclista[i].n_voltas);            
        }
    }
}
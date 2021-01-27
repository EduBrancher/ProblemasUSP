/*  EP 2 - Sistemas Operacionais

Julia Leite
Eduardo Brancher

*/

#ifndef _CICLISTA_H_
#define _CICLISTA_H_

typedef struct cyclist {

    int vel; 
    int faixa;
    int pos;      // posicao na pista
    int n_voltas; // voltas COMPLETAS
    int quebrou;  // quebrou indica que quebrou agora
    int dt;
    int id;
    int t_chegada; // instante que o ciclista passou pela linha de chegada pela ultima vez
    int eliminado_pendente; //sera eliminado na proxima vez que cruzar a linha de chegada.
    int eliminado; // se quebrou ou foi eliminado ele e eliminado
    int terminou; // se ja correu 2n voltas

} Ciclista;

Ciclista* ciclista;

// cria os ciclistas e sortea a posicao de largada de cada um
void ciclista_init (int n, int* pista[]);

// verifica se o ciclista quebrou (5% de prob)
int broken ();

// sortea a nova velocidade
// recebe a velocidade anterior
int velocity (int ant);

// sorteia a nova velocidade do ciclista 'ind'
// e se ele quebrou ou nao 
// att o numero de voltas
// guarda o instante
void update_ciclista (int ind, int t, int n);

// FLAG
int cic_90 = -1; 
// cic_90 = 2: se o segundo ciclista foi sorteado pra peladar a 90 km/h
// cic_90 = 1: se tem alguem pedalando a 90 km/h
// cic_90 = 0: se ninguem foi sorteado pra pedalar a 90 km/h
// cic_90 = -1: nenhum ciclista completou 2n-3 voltas e tem mais de 2 ciclistas na pista

#endif

#include <stdio.h>
#include <stdlib.h>

void ciclista_init (int n, int* pista[]) {

    ciclista = malloc(n*sizeof(Ciclista));

    for (int i = 0; i < n; i++) {
    
        ciclista[i].vel = 30;
        ciclista[i].quebrou = 0;
        ciclista[i].eliminado = 0;
        ciclista[i].n_voltas = 0;
        ciclista[i].faixa = ciclista[i].pos = -1;
        ciclista[i].dt = 0;
        ciclista[i].id = i;
        ciclista[i].t_chegada = 0;
        ciclista[i].eliminado_pendente = 0;
        ciclista[i].terminou = 0;
    }

    // agora sorteamos a posicao de largada de cada ciclista

    int ind = 0;

    // posição na pista
    for (int p = 0; p < (n/5 + n % 5); p++)
        
        for (int f = 0 + 5*(p % 2); f < 5*(1 + p % 2); f++) {
            
            if (ind == n)
                break;

            int aux = rand() % n;

            while (ciclista[aux].pos != -1) {
                aux = rand() % n;
            }

            pista[f][p] = aux;
            ciclista[aux].pos = p;
            ciclista[aux].faixa = f;
            ind++;
            
        }

}

int broken () {

    int aux = rand() % 100;
    return (aux < 5) ? (1) : (0);
}

int velocity (int ant) {

    int aux = rand() % 100;

    if (ant == 30)
        return (aux < 80) ? (60) : (30);

    return (aux < 40) ? (30) : (60);

}

// n: numero de ciclistas ainda na corrida
// t atual
void update_ciclista (int ind, int t, int n) {

    ciclista[ind].n_voltas++;
    ciclista[ind].t_chegada = t;

    if (ciclista[ind].vel != 90)
        ciclista[ind].vel = velocity(ciclista[ind].vel);
    
}
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdbool.h>
#include "inicializar.h"
#include "entidades.h"
#include "estadoJogo.h"
#include "animacao.h"

void atualizarEstado(bool keys[4], bool noChao, bool interacao, struct personagem *persona){
    if(interacao) persona->est = ESCALANDO;
    else if(!noChao) persona->est = PULANDO;
    else if(keys[1]) persona->est = AGACHADO;
    else if(keys[2]) persona->est = ANDANDO_ESQ;
    else if(keys[3]) persona->est = ANDANDO_DIR;
    else persona->est = PARADO;
}

void atualizarAnimacao(struct animacao *a){
    a->cont++;
    if(a->cont >= a->atraso){
        a->cont = 0;
        a->frameAtual++;
        if(a->frameAtual >= a->totalFrames)
            a->frameAtual = 0;
    }
}

void atualizarPersonagem(struct personagem *persona){
    switch(persona->est){
        case PARADO:
            persona->anim.frameAtual = 0;
            persona->anim.cont = 0;
            break;
        case ANDANDO_ESQ:
            persona->direcao = 0;
            atualizarAnimacao(&persona->anim);
            break;
        case ANDANDO_DIR:
            persona->direcao = 1;
            atualizarAnimacao(&persona->anim);
            break;
        case PULANDO:
            atualizarAnimacao(&persona->anim);
            break;
        case ESCALANDO:
            atualizarAnimacao(&persona->anim);
            break;
        case AGACHADO:
            persona->anim.frameAtual = 0;
            break;
    }
}
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdbool.h>
#include "entidades.h"
#include "inicializar.h"

void inicializarPersonagem(struct personagem *persona){
    persona->velocidade = 5;
    persona->vida = 3;
    persona->posX = 20; //ver isso
    persona->posY = ALTURA_TELA - 100;
}

void desenharPersonagem(struct personagem persona){
    al_draw_filled_rectangle(persona.posX, persona.posY, persona.posX + 20, persona.posY + 20, al_map_rgb(255, 0, 255));
}

void moverCima(struct personagem *persona){
    persona->posY -= persona->velocidade;
}

void moverBaixo(struct personagem *persona){
    persona->posY += persona->velocidade;
}

void moverEsquerda(struct personagem *persona){
    persona->posX -= persona->velocidade;
    if(persona->posX < 0)
        persona->posX = 0;
}

void moverDireita(struct personagem *persona){
    persona->posX += persona->velocidade;
}
//desenhar personagem
//inicializar personagem
//mover para cima
//mover para baixo
//mover para os lados
//o mover para baixo no futuro vai ser inutil, mas por ora para testar a movimentaçao deixa ai
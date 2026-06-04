#ifndef ENTIDADES
#define ENTIDADES

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdbool.h>

struct hitbox{
    int pontoX;
    int pontoY;
    int largura;
    int altura;
};

struct personagem{
    int vida;
    int velocidade;
    int posX;
    int posY;
    struct hitbox hitboxPersonagem;
    float velocidadeY;
    //allegro bitmap no futuro
    //nome?
};

struct tile{
    int hitboxINTEIRA;
    int hitboxEMCIMA;
    int spriteID;
};

static const struct tile TILE[] = {
    [0] = {0, 0, 0},
    [1] = {1, 0, 1},
};

void inicializarPersonagem(struct personagem *persona);
void desenharPersonagem(struct personagem persona);
void moverCima(struct personagem *persona);
void moverBaixo(struct personagem *persona);
void moverEsquerda(struct personagem *persona);
void moverDireita(struct personagem *persona);
bool verificarHitbox(struct personagem *persona, int mapa[15][20]);
void inicializarMapa(int mapa[15][20]);
void desenharMapa(int mapa[15][20]);

#endif
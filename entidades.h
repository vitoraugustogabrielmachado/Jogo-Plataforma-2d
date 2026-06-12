#ifndef ENTIDADES
#define ENTIDADES

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdbool.h>

#define LINHAS 15
#define COLUNAS 25
#define ALTURAPULO -8.0
#define DECAIMENTOPULO 0.4

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

struct camera{
    int posX;
    int posY;
};

struct tile{
    int hitboxINTEIRA;
    int hitboxEMCIMA;
    int spriteID;
};

static const struct tile TILE[] = {
    [0] = {0, 0, 0},
    [1] = {0, 1, 1},
    [2] = {1, 0, 1},
};


void moverCima(struct personagem *persona);
void moverBaixo(struct personagem *persona);
void moverEsquerda(struct personagem *persona);
void moverDireita(struct personagem *persona);
void inicializarMapa(int mapa[LINHAS][COLUNAS]);
void inicializarPersonagem(struct personagem *persona);
void inicializarCamera(struct camera *camera, struct personagem persona);
void desenharMapa(int mapa[LINHAS][COLUNAS], struct camera *camera);
void desenharPersonagem(struct personagem persona, struct camera *camera);
void colisaoHorizontal(struct personagem *persona, int mapa[LINHAS][COLUNAS]);
bool colisaoVertical(struct personagem *persona, int mapa[LINHAS][COLUNAS]);

#endif
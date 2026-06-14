#ifndef ENTIDADES
#define ENTIDADES

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdbool.h>

#define LINHAS 30
#define COLUNAS 100
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
    int dano;
};

struct background{
    float x;
    float y;
    float velx;
    int dirx;
    int diry;

    int largura;
    int altura;

    ALLEGRO_BITMAP *bg;
};

static const struct tile TILE[] = {
    [0] = {0, 0, 0, 0},
    [1] = {0, 1, 1, 0},
    [2] = {1, 0, 1, 0},
    [3] = {1, 0, 1, 1}
};


void moverCima(struct personagem *persona);
void moverBaixo(struct personagem *persona);
void moverEsquerda(struct personagem *persona);
void moverDireita(struct personagem *persona);
void inicializarMapa(int mapa[LINHAS][COLUNAS]);
void inicializarPersonagem(struct personagem *persona);
void inicializarInimigo(struct personagem *inimigo);
void inicializarCamera(struct camera *camera, struct personagem persona);
void inicializarBackground(struct background *bg, float x, float y, float velx, int dirx, int diry, int largura, int altura, ALLEGRO_BITMAP *fundo);
void desenharMapa(int mapa[LINHAS][COLUNAS], struct camera *camera, ALLEGRO_BITMAP *chao);
void desenharPersonagem(struct personagem persona, struct camera *camera);
void desenharInimigo(struct personagem inimigo, struct camera *camera);
void desenharBackground(struct background *bg);
void colisaoHorizontal(struct personagem *persona, int mapa[LINHAS][COLUNAS]);
bool colisaoVertical(struct personagem *persona, int mapa[LINHAS][COLUNAS]);
void atualizaBackground(struct background *bg, struct camera *camera);
void atualizarInimigo(struct personagem *inimigo, bool *bateuEsq);


#endif
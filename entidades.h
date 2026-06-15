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
#define TAMANHOTILE 16

typedef enum{
    PARADO, 
    ANDANDO_ESQ, 
    ANDANDO_DIR, 
    PULANDO, 
    AGACHADO, 
    ESCALANDO
}estado;

struct allegro{
    ALLEGRO_DISPLAY *janela;
    ALLEGRO_EVENT_QUEUE *fila_eventos;
    ALLEGRO_BITMAP *fundo;
    ALLEGRO_BITMAP *personagem;
    ALLEGRO_FONT *fonte;
    ALLEGRO_TIMER *timer;
};

struct tipoTiles{
    ALLEGRO_BITMAP *chao;
    ALLEGRO_BITMAP *paredeEsq;
    ALLEGRO_BITMAP *paredeDir;
    ALLEGRO_BITMAP *escada;
    ALLEGRO_BITMAP *meioParede;
    ALLEGRO_BITMAP *perigo1;
};

struct animacao{
    int frameAtual;
    int totalFrames;
    int cont;
    int atraso;
};

struct personagem{
    int vida;
    int velocidade; // tenho q colocar float
    int posX;
    int posY;
    int largura;
    int altura;
    float velocidadeY;

    estado est;
    struct animacao anim;

    ALLEGRO_BITMAP *andar[2];
    ALLEGRO_BITMAP *pular[2];
    ALLEGRO_BITMAP *escalar;
    ALLEGRO_BITMAP *agachar;
    ALLEGRO_BITMAP *parado;
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
    int dano;
    int interacao;
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
    [0] = {0, 0, 0, 0}, //nada
    [1] = {0, 1, 0, 0}, //chao 
    [2] = {1, 0, 0, 0}, //paredeEsq
    [3] = {1, 0, 0, 0}, //meiodaparede
    [4] = {1, 0, 0, 0}, //paredeDir
    [5] = {1, 0, 0, 1}, //escada
    [6] = {1, 0, 1, 0},
};


void moverCima(struct personagem *persona);
void moverBaixo(struct personagem *persona);
void moverEsquerda(struct personagem *persona);
void moverDireita(struct personagem *persona);

void desenharMapa(int mapa[LINHAS][COLUNAS], struct camera *camera, struct tipoTiles *desenhos);
void desenharPersonagem(struct personagem persona, struct camera *camera);
void desenharInimigo(struct personagem inimigo, struct camera *camera);
void desenharBackground(struct background *bg);

bool colisaoHorizontal(struct personagem *persona, int mapa[LINHAS][COLUNAS]);
bool colisaoVertical(struct personagem *persona, int mapa[LINHAS][COLUNAS]);
void colisaoInimigo(struct personagem *persona, struct personagem *inimigo);

void atualizarBackground(struct background *bg, struct camera *camera);
void atualizarInimigo(struct personagem *inimigo, bool *bateuEsq);


#endif
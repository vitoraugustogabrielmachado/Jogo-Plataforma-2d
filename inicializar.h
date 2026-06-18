#ifndef INICIALIZAR
#define INICIALIZAR

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <stdio.h>
#include <stdbool.h>
#include "entidades.h"

#define LARGURA_TELA 640
#define ALTURA_TELA 480
#define FPS 60

bool inicializar(struct allegro *elementos);
void inicializarMapa(int mapa[LINHAS][COLUNAS]);
void inicializarPersonagem(struct personagem *persona, struct allegro elementos);
void inicializarInimigo(struct personagem *inimigo, struct allegro elementos);
void inicializarCamera(struct camera *camera, struct personagem persona);
void inicializarBackground(struct background *bg, float x, float y, float velx, int dirx, int diry, int largura, int altura, ALLEGRO_BITMAP *fundo);
void inicializarElementos(struct allegro *elementos);
void inicializarDesenhos(struct tipoTiles *desenhos);
void destruirJogo(struct personagem *persona, struct personagem *inimigo, struct allegro *elementos, struct tipoTiles *desenhos);

#endif
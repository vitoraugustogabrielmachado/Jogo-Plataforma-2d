#ifndef MENU_H
#define MENU_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdbool.h>
#include "inicializar.h"
#include "entidades.h"


void desenharMenu(estadoJogo *estado, ALLEGRO_EVENT ev, struct allegro elementos);
void desenharMorte(estadoJogo *estado, ALLEGRO_EVENT ev, struct allegro elementos, struct personagem *persona);
void desenharVitoria(estadoJogo *estado, ALLEGRO_EVENT ev, struct allegro elementos);
void jogo(int mapa[LINHAS][COLUNAS], struct personagem *persona, struct personagem *inimigo, struct allegro elementos, struct camera *camera, struct background *bg, estadoJogo *estado, ALLEGRO_EVENT ev, struct tipoTiles desenhos);
#endif
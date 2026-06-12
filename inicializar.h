#ifndef INICIALIZAR
#define INICIALIZAR

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <stdio.h>
#include <stdbool.h>

#define LARGURA_TELA 640
#define ALTURA_TELA 480
#define FPS 60

bool inicializar(ALLEGRO_DISPLAY **janela, ALLEGRO_EVENT_QUEUE **fila_eventos, ALLEGRO_FONT **fonte, ALLEGRO_TIMER **timer, ALLEGRO_BITMAP **fundo);

#endif
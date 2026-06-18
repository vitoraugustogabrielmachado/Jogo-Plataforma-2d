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

int main(){
    struct allegro elementos;
    inicializarElementos(&elementos);

    int mapa[LINHAS][COLUNAS];
    inicializarMapa(mapa);

    if(!inicializar(&elementos))
        return 0;
    al_start_timer(elementos.timer);

    struct personagem persona;
    inicializarPersonagem(&persona, elementos);

    struct personagem inimigo;
    inicializarInimigo(&inimigo, elementos);

    struct camera camera;
    inicializarCamera(&camera, persona);

    struct background bg;
    inicializarBackground(&bg, 0, 0, 1, -1, 1, LARGURA_TELA, ALTURA_TELA, elementos.fundo);

    struct tipoTiles desenhos;
    inicializarDesenhos(&desenhos);

    estadoJogo estado = MENU;
    bool sair = false;
    while(!sair){
        ALLEGRO_EVENT ev;
        al_wait_for_event(elementos.fila_eventos, &ev);
        switch(estado){
            case PAUSAR: desenharMenu(&estado, ev, elementos); break;
            case MENU: desenharMenu(&estado, ev, elementos); break;
            case JOGANDO: jogo(mapa, &persona, &inimigo, elementos, &camera, &bg, &estado, ev, desenhos); break;
            case MORTE: desenharMorte(&estado, ev, elementos, &persona); break;
            case VITORIA: desenharVitoria(&estado, ev, elementos); break;
            case SAIR: sair = true; break;
        }
    
    }
    destruirJogo(&persona, &inimigo, &elementos, &desenhos);
    return 1;
}
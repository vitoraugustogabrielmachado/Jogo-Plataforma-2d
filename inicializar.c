#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdbool.h>
#include "inicializar.h"

bool inicializar(struct allegro *elementos){
    if (!al_init()){
      fprintf(stderr, "Falha ao inicializar a Allegro.\n");
      return false;
    }
  
    if (!al_init_ttf_addon()){
      fprintf(stderr, "Falha ao inicializar add-on allegro_ttf.\n");
      return false;
    }
  
    if (!al_init_image_addon()){
      fprintf(stderr, "Falha ao inicializar add-on allegro_image.\n");
      return false;
    }
  
    if (!al_install_keyboard()){
      fprintf(stderr, "Falha ao inicializar o teclado.\n");
      return false;
    }
  
    elementos->janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!elementos->janela){
      fprintf(stderr, "Falha ao criar janela.\n");
      return false;
    }
  
    al_set_window_title(elementos->janela, "Utilizando o Teclado");
  
    al_init_font_addon();
    elementos->fonte = al_load_font("comic.ttf", 72, 0);
    if (!elementos->fonte){
      fprintf(stderr, "Falha ao carregar \"fonte comic.ttf\".\n");
      al_destroy_display(elementos->janela);
      return false;
    }
  
    elementos->fila_eventos = al_create_event_queue();
    if (!elementos->fila_eventos){
      fprintf(stderr, "Falha ao criar fila de eventos.\n");
      al_destroy_display(elementos->janela);
      return false;
    }
  
    al_init_primitives_addon();
    elementos->fundo = al_load_bitmap("fundo4.png");
    if (!elementos->fundo){
      fprintf(stderr, "Falha ao carregar imagem de fundo.\n");
      al_destroy_display(elementos->janela);
      al_destroy_event_queue(elementos->fila_eventos);
      return false;
    }
    /*personagem = al_load_bitmap("images.jpg");
    if (!personagem){
      fprintf(stderr, "Falha ao carregar imagem.\n");
      al_destroy_display(janela);
      al_destroy_event_queue(fila_eventos);
      return false;
    }*/
  
    al_register_event_source(elementos->fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(elementos->fila_eventos, al_get_display_event_source(elementos->janela));

    elementos->timer = al_create_timer(1.0 / FPS);
    if (!elementos->timer){
      fprintf(stderr, "Falha ao criar timer.\n");
      al_destroy_display(elementos->janela);
      al_destroy_event_queue(elementos->fila_eventos);
      return false;
    }
    al_register_event_source(elementos->fila_eventos, al_get_timer_event_source(elementos->timer));
    return true;
}

void inicializarInimigo(struct personagem *inimigo){
    inimigo->velocidade = 1;
    inimigo->vida = 0;
    inimigo->posX = 500; //ver isso
    inimigo->posY = (LINHAS - 1) * 16 - 20;
    inimigo->altura = 20;
    inimigo->largura = 20;
    inimigo->velocidadeY = 0;
}

void inicializarCamera(struct camera *camera, struct personagem persona){
    camera->posX = persona.posX - LARGURA_TELA/2;
}

void inicializarBackground(struct background *bg, float x, float y, float velx, int dirx, int diry, int largura, int altura, ALLEGRO_BITMAP *fundo){
    bg->x = x;
    bg->y = y;
    bg->velx = velx;
    bg->dirx = dirx;
    bg->diry = diry;
    bg->largura = largura;
    bg->altura = altura;
    bg->bg = fundo;
}

void inicializarMapa(int mapa[LINHAS][COLUNAS]) {
    FILE *f = fopen("mapa.txt", "r");
    if(!f){
        printf("Erro ao abrir arquivo\n");
        return;
    }
    char c;
    for (int lin = 0; lin < LINHAS; lin++)
        for (int col = 0; col < COLUNAS; col++){
            fscanf(f, " %c", &c);
            mapa[lin][col] = c - '0';
        }
    fclose(f);
            
}

void inicializarPersonagem(struct personagem *persona){
    persona->velocidade = 5;
    persona->vida = 3000;
    persona->posX = 0; //ver isso
    persona->posY = (LINHAS - 2) * 16 - 20;
    persona->altura = 20;
    persona->largura = 20;
    persona->velocidadeY = 0;
}

void inicializarElementos(struct allegro *elementos){
    elementos->janela = NULL;
    elementos->fila_eventos = NULL;
    elementos->fonte = NULL;
    elementos->fundo = NULL;
    elementos->timer = NULL;
}

void inicializarDesenhos(struct tipoTiles *desenhos){
    desenhos->chao = al_load_bitmap("sprites/tile_0000.png");
    desenhos->paredeDir = al_load_bitmap("sprites/tile_0024.png");
    desenhos->paredeEsq = al_load_bitmap("sprites/tile_0025.png");
    desenhos->escada = al_load_bitmap("sprites/tile_0071.png");
    desenhos->meioParede = al_load_bitmap("sprites/tile_0104.png");
}
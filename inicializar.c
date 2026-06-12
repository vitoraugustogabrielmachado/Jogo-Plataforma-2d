#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdbool.h>
#include "inicializar.h"

bool inicializar(ALLEGRO_DISPLAY **janela, ALLEGRO_EVENT_QUEUE **fila_eventos, ALLEGRO_FONT **fonte, ALLEGRO_TIMER **timer, ALLEGRO_BITMAP **fundo){
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
 
  *janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
  if (!*janela){
    fprintf(stderr, "Falha ao criar janela.\n");
    return false;
  }
 
  al_set_window_title(*janela, "Utilizando o Teclado");
 
  al_init_font_addon();
  *fonte = al_load_font("comic.ttf", 72, 0);
  if (!*fonte){
    fprintf(stderr, "Falha ao carregar \"fonte comic.ttf\".\n");
    al_destroy_display(*janela);
    return false;
  }
 
  *fila_eventos = al_create_event_queue();
  if (!*fila_eventos){
    fprintf(stderr, "Falha ao criar fila de eventos.\n");
    al_destroy_display(*janela);
    return false;
  }
 
  al_init_primitives_addon();
  *fundo = al_load_bitmap("fundo.png");
  if (!*fundo){
    fprintf(stderr, "Falha ao carregar imagem de fundo.\n");
    al_destroy_display(*janela);
    al_destroy_event_queue(*fila_eventos);
    return false;
  }
  /*personagem = al_load_bitmap("images.jpg");
  if (!personagem){
    fprintf(stderr, "Falha ao carregar imagem.\n");
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
    return false;
  }*/
 
  al_register_event_source(*fila_eventos, al_get_keyboard_event_source());
  al_register_event_source(*fila_eventos, al_get_display_event_source(*janela));

  *timer = al_create_timer(1.0 / FPS);
  if (!*timer){
    fprintf(stderr, "Falha ao criar timer.\n");
    al_destroy_display(*janela);
    al_destroy_event_queue(*fila_eventos);
    return false;
  }
  al_register_event_source(*fila_eventos, al_get_timer_event_source(*timer));
  return true;
}
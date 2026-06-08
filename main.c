#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdbool.h>
#include "inicializar.h"
#include "entidades.h"

void loop(int mapa[LINHAS][COLUNAS], struct personagem *persona, ALLEGRO_EVENT_QUEUE *fila_eventos){
    bool sair = false, redraw = false;
    bool keys[4] = {false, false, false, false};

    int cameraX = persona->posX - LARGURA_TELA/2;

    //printf("%d\n", cameraX);

    while(!sair){
        ALLEGRO_EVENT ev;
        al_wait_for_event(fila_eventos, &ev);
        //printf("%d\n", cameraX);
        //printf("%d\n", persona.posY);
        if(ev.type == ALLEGRO_EVENT_TIMER){
            redraw = true;
            
            persona->velocidadeY += 0.4f;
            persona->posY += persona->velocidadeY;


            if(keys[2])
                moverEsquerda(persona, &cameraX);
                //mover personagem para esquerda
            if(keys[3])
                moverDireita(persona, &cameraX);
                //mover personagem para direita
            verificarHitbox(persona, mapa, cameraX);
            cameraX = persona->posX - LARGURA_TELA/2;
            if(cameraX <= 0)
                cameraX = 0;
        }
        else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            sair = true;
        else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
            switch (ev.keyboard.keycode){
                case ALLEGRO_KEY_UP:
                    if(persona->posY >= 429)
                        persona->velocidadeY = -8.0f;
                    break;
                
                case ALLEGRO_KEY_DOWN:
                    keys[1] = true;
                    break;

                case ALLEGRO_KEY_LEFT:
                    keys[2] = true;
                    break;

                case ALLEGRO_KEY_RIGHT:
                    keys[3] = true;
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    sair = true;
                    break;
            }
        else if(ev.type == ALLEGRO_EVENT_KEY_UP)
            switch(ev.keyboard.keycode){
             
                
                case ALLEGRO_KEY_DOWN:
                    keys[1] = false;
                    break;

                case ALLEGRO_KEY_LEFT:
                    keys[2] = false;
                    break;

                case ALLEGRO_KEY_RIGHT:
                    keys[3] = false;
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    sair = true;
                    break;
            }
        if(redraw && al_event_queue_is_empty(fila_eventos)){
            redraw = false;                         
            al_clear_to_color(al_map_rgb(255, 255, 255)); 
            desenharMapa(mapa, &cameraX);                      
            desenharPersonagem(*persona, cameraX);             
            al_flip_display();                       
        }
    }
}

int main(){
    ALLEGRO_DISPLAY *janela = NULL;
    ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
    /*ALLEGRO_BITMAP *fundo = NULL;
    ALLEGRO_BITMAP *personagem = NULL;*/
    ALLEGRO_FONT *fonte = NULL;
    ALLEGRO_TIMER *timer = NULL;//20 15

    int mapa[LINHAS][COLUNAS];
    inicializarMapa(mapa);
    
    
    if(!inicializar(&janela, &fila_eventos, &fonte, &timer))
        return (0);

    al_start_timer(timer);

    struct personagem persona;
    inicializarPersonagem(&persona);
    
    //inicializar personagem
    loop(mapa, &persona, fila_eventos);

    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
 
    return 1;
}
 

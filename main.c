#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdbool.h>
#include "inicializar.h"
#include "entidades.h"
 
int main(){
    ALLEGRO_DISPLAY *janela = NULL;
    ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
    /*ALLEGRO_BITMAP *fundo = NULL;
    ALLEGRO_BITMAP *personagem = NULL;*/
    ALLEGRO_FONT *fonte = NULL;
    ALLEGRO_TIMER *timer = NULL;
    
    bool sair = false, redraw = false;
    bool keys[4] = {false, false, false, false};
    bool jump = false;
    
    if(!inicializar(&janela, &fila_eventos, &fonte, &timer))
        return (0);
    al_start_timer(timer);

    struct personagem persona;
    inicializarPersonagem(&persona);
    // initial draw
    al_clear_to_color(al_map_rgb(0,0,0));
    desenharPersonagem(persona);
    //inicializar personagem
    //desenhar personagem

    while(!sair){
        ALLEGRO_EVENT ev;
        al_wait_for_event(fila_eventos, &ev);
        if(ev.type == ALLEGRO_EVENT_TIMER){
            redraw = true;
            if(keys[0]){
                moverCima(&persona);
                //jump = true;
                //mover personagem para cima
            }
            if(keys[1])
                moverBaixo(&persona);
                //mover personagem para baixo
            if(keys[2])
                moverEsquerda(&persona);
                //mover personagem para esquerda
            if(keys[3])
                moverDireita(&persona);
            /*if(jump)
                persona.posY += persona.velocidade; 
                //mover personagem para direita*/
        }
        else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            sair = true;
        else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
            switch (ev.keyboard.keycode){
                case ALLEGRO_KEY_UP:
                    keys[0] = true;
                    //jump = false;
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
                case ALLEGRO_KEY_UP:
                    keys[0] = false;
                    //jump = false;
                    break;
                
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
            //desenhar personagem
            desenharPersonagem(persona);
            al_flip_display();
            al_clear_to_color(al_map_rgb(0, 0, 0));
        }
    }
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
 
    return 1;
}
 

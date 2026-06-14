#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdbool.h>
#include "inicializar.h"
#include "entidades.h"

void loop(int mapa[LINHAS][COLUNAS], struct personagem *persona, struct personagem *inimigo, ALLEGRO_EVENT_QUEUE *fila_eventos, ALLEGRO_BITMAP *fundo, struct camera *camera, struct background *bg){
    bool sair = false, redraw = false, noChao = false, agachado = false, bateuEsq = false, interacao = false;
    bool keys[4] = {false, false, false, false};
    ALLEGRO_BITMAP *chao;
    chao = al_load_bitmap("tile_0000.png"); // colocar isso numa funcao


    while(!sair){
        ALLEGRO_EVENT ev;
        al_wait_for_event(fila_eventos, &ev);
        //printf("%d\n", persona->posY);
        if(ev.type == ALLEGRO_EVENT_TIMER){
            redraw = true;
            if(!agachado){
                if(keys[2])
                    moverEsquerda(persona);
                if(keys[3])
                    moverDireita(persona);
                interacao = colisaoHorizontal(persona, mapa);
                //printf("%d", interacao);
                if(!interacao){
                    colisaoInimigo(persona, inimigo);
                
                    persona->velocidadeY += DECAIMENTOPULO;
                    persona->posY += persona->velocidadeY;
                    noChao = colisaoVertical(persona, mapa);
                    if(keys[0] && noChao)       
                        persona->velocidadeY = ALTURAPULO;
                    if(persona->vida == 0){
                        persona->posX = 0; //ver isso
                        persona->posY = ALTURA_TELA - 100;
                    }
                }
                else{
                    if(keys[0]){
                        persona->posY -= 2;
                    }
                    if(keys[1]){
                        if(persona->posY < (LINHAS - 2.3) * 16)
                            persona->posY += 2;
                    }
                }
                camera->posX = persona->posX - LARGURA_TELA/2;
                if(camera->posX <= 0)
                    camera->posX = 0;
            }
        }
        else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            sair = true;
        else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
            switch (ev.keyboard.keycode){
                case ALLEGRO_KEY_UP:
                    keys[0] = true;
                    break;
                
                case ALLEGRO_KEY_DOWN:
                    keys[1] = true;
                    if(!interacao && noChao)
                        agachado = true;
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
                    break;
                
                case ALLEGRO_KEY_DOWN:
                    keys[1] = false;
                    if(!interacao && noChao)
                        agachado = false;
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
        atualizaBackground(bg, camera);
        atualizarInimigo(inimigo, &bateuEsq);
        if(redraw && al_event_queue_is_empty(fila_eventos)){
            redraw = false;                         
            al_clear_to_color(al_map_rgb(255, 255, 255)); 
            desenharBackground(bg);
            desenharMapa(mapa, camera, chao);                      
            desenharPersonagem(*persona, camera);   
            desenharInimigo(*inimigo, camera);          
            al_flip_display();                       
        }
    }
}

int main(){
    ALLEGRO_DISPLAY *janela = NULL;
    ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
    ALLEGRO_BITMAP *fundo = NULL;
    //ALLEGRO_BITMAP *personagem = NULL;*/
    ALLEGRO_FONT *fonte = NULL;
    ALLEGRO_TIMER *timer = NULL;

    int mapa[LINHAS][COLUNAS];
    inicializarMapa(mapa);
    if(!inicializar(&janela, &fila_eventos, &fonte, &timer, &fundo))
        return (0);
    al_start_timer(timer);

    struct personagem persona;
    inicializarPersonagem(&persona);

    struct personagem inimigo;
    inicializarInimigo(&inimigo);


    struct camera camera;
    inicializarCamera(&camera, persona);

    struct background bg;
    inicializarBackground(&bg, 0, 0, 1, -1, 1, LARGURA_TELA, ALTURA_TELA, fundo);

    loop(mapa, &persona, &inimigo, fila_eventos, fundo, &camera, &bg);

    al_destroy_bitmap(fundo);
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
 
    return 1;
}
 

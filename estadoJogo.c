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

#define MINIMOVITORIA 1530

void desenharMenu(estadoJogo *estado, ALLEGRO_EVENT ev, struct allegro elementos){
    if(ev.type == ALLEGRO_EVENT_TIMER){
        if(*estado == MENU){
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_text(elementos.fonte, al_map_rgb(255,0,0), 320, 200, ALLEGRO_ALIGN_CENTRE, "MENU INICIAL");
            al_draw_text(elementos.fonte, al_map_rgb(255,255,255), 320, 240, ALLEGRO_ALIGN_CENTRE, "SPACE - COMEÇAR  ESC - SAIR");
        }else{
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_text(elementos.fonte, al_map_rgb(255,0,0), 320, 200, ALLEGRO_ALIGN_CENTRE, "PAUSADO");
            al_draw_text(elementos.fonte, al_map_rgb(255,255,255), 320, 240, ALLEGRO_ALIGN_CENTRE, "SPACE - VOLTAR  ESC - SAIR");
        }
        al_flip_display();
    }
    if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
        if(ev.keyboard.keycode == ALLEGRO_KEY_SPACE)
            *estado = JOGANDO;
        else if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE || ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            *estado = SAIR;
    }
}   

void desenharMorte(estadoJogo *estado, ALLEGRO_EVENT ev, struct allegro elementos, struct personagem *persona){
    if(ev.type == ALLEGRO_EVENT_TIMER){
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_text(elementos.fonte, al_map_rgb(255,0,0), 320, 200, ALLEGRO_ALIGN_CENTRE, "VOCE MORREU");
        al_draw_text(elementos.fonte, al_map_rgb(255,255,255), 320, 240, ALLEGRO_ALIGN_CENTRE, "ESC PARA SAIR");
        al_flip_display();
    }
    if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
        if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE || ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            *estado = SAIR;
    }
}

void desenharVitoria(estadoJogo *estado, ALLEGRO_EVENT ev, struct allegro elementos){
    if(ev.type == ALLEGRO_EVENT_TIMER){
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_text(elementos.fonte, al_map_rgb(255,0,0), 320, 200, ALLEGRO_ALIGN_CENTRE, "VOCE VENCEU!");
        al_draw_text(elementos.fonte, al_map_rgb(255,255,255), 320, 240, ALLEGRO_ALIGN_CENTRE, "ESC PARA SAIR");
        al_flip_display();
    }
    if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
        if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE || ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            *estado = SAIR;
    }
}

void jogo(int mapa[LINHAS][COLUNAS], struct personagem *persona, struct personagem *inimigo, struct allegro elementos, struct camera *camera, struct background *bg, estadoJogo *estado, ALLEGRO_EVENT ev, struct tipoTiles desenhos){
    static bool redraw = false, noChao = false, bateuEsq = false, interacao = false;
    static bool keys[4] = {false, false, false, false};
    
    if(ev.type == ALLEGRO_EVENT_TIMER){
        redraw = true;
        if(persona->est != AGACHADO){
            if(keys[2]) moverEsquerda(persona);
            if(keys[3]) moverDireita(persona);

            colisaoInimigo(persona, inimigo);
            interacao = colisaoHorizontal(persona, mapa);

            if(!interacao){
                persona->velocidadeY += DECAIMENTOPULO;
                persona->posY += persona->velocidadeY;
                colisaoVertical(persona, mapa);        
                noChao = estaNoChao(persona, mapa);    

                if(keys[0] && noChao) persona->velocidadeY = ALTURAPULO;

                if(persona->vida == 0) *estado = MORTE;
                
            } else {
                noChao = true;
                if(keys[0]) persona->posY -= 2.0;
                if(keys[1])
                    if(persona->posY < (float)((LINHAS - 2.3f) * 16))
                        persona->posY += 2.0;
            }
            if((int)persona->posX >= MINIMOVITORIA)
                *estado = VITORIA;
            camera->posX = persona->posX - (float)(LARGURA_TELA / 2);
            if(camera->posX <= 0.0)
                camera->posX = 0.0;
        }

        atualizarEstado(keys, noChao, interacao, persona);
        atualizarPersonagem(persona);
        atualizarAnimacao(&inimigo->anim);
    }
    else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        *estado = MENU;
    else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
        switch(ev.keyboard.keycode){
            case ALLEGRO_KEY_UP:    keys[0] = true; break;
            case ALLEGRO_KEY_DOWN:  keys[1] = true; break;
            case ALLEGRO_KEY_LEFT:  keys[2] = true; break;
            case ALLEGRO_KEY_RIGHT: keys[3] = true; break;
            case ALLEGRO_KEY_ESCAPE: *estado = PAUSAR;   break;
        }
    else if(ev.type == ALLEGRO_EVENT_KEY_UP)
        switch(ev.keyboard.keycode){
            case ALLEGRO_KEY_UP:    keys[0] = false; break;
            case ALLEGRO_KEY_DOWN:  keys[1] = false; break;
            case ALLEGRO_KEY_LEFT:  keys[2] = false; break;
            case ALLEGRO_KEY_RIGHT: keys[3] = false; break;
            case ALLEGRO_KEY_ESCAPE: *estado = PAUSAR; break;
        }

    atualizarBackground(bg, camera);
    atualizarInimigo(inimigo, &bateuEsq);

    if(redraw && al_event_queue_is_empty(elementos.fila_eventos)){
        redraw = false;
        al_clear_to_color(al_map_rgb(255, 255, 255));
        desenharBackground(bg);
        desenharMapa(mapa, camera, &desenhos);
        desenharPersonagem(*persona, camera);
        desenharInimigo(*inimigo, camera);
        al_flip_display();
    }
}
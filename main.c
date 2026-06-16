#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdbool.h>
#include "inicializar.h"
#include "entidades.h"

bool estaNoChao(struct personagem *persona, int mapa[LINHAS][COLUNAS]) {
    int inicioX = (int)persona->posX / TAMANHOTILE;
    int fimX = ((int)persona->posX + persona->largura - 1) / TAMANHOTILE;
    int lin    = ((int)persona->posY + persona->altura) / TAMANHOTILE;

    if(lin >= LINHAS) 
        return(false);
    if(lin < 0)       
        return(false);

    if(inicioX < 0)      
        inicioX = 0;
    if(fimX >= COLUNAS) 
        fimX = COLUNAS - 1;

    for(int col = inicioX; col <= fimX; col++){
        struct tile temp = TILE[mapa[lin][col]];
        if(temp.hitboxEMCIMA || temp.hitboxINTEIRA)
            return true;
    }
    return(false);
}

void atualizarEstado(bool keys[4], bool noChao, bool interacao, struct personagem *persona){
    if(interacao) persona->est = ESCALANDO;
    else if(!noChao) persona->est = PULANDO;
    else if(keys[1]) persona->est = AGACHADO;
    else if(keys[2]) persona->est = ANDANDO_ESQ;
    else if(keys[3]) persona->est = ANDANDO_DIR;
    else persona->est = PARADO;
}

void atualizarAnimacao(struct animacao *a){
    a->cont++;
    if(a->cont >= a->atraso){
        a->cont = 0;
        a->frameAtual++;
        if(a->frameAtual >= a->totalFrames)
            a->frameAtual = 0;
    }
}

void atualizarPersonagem(struct personagem *persona){
    switch(persona->est){
        case PARADO:
            persona->anim.frameAtual = 0;
            persona->anim.cont = 0;
            break;
        case ANDANDO_ESQ:
            persona->direcao = 0;
            atualizarAnimacao(&persona->anim);
            break;
        case ANDANDO_DIR:
            persona->direcao = 1;
            atualizarAnimacao(&persona->anim);
            break;
        case PULANDO:
            atualizarAnimacao(&persona->anim);
            break;
        case ESCALANDO:
            atualizarAnimacao(&persona->anim);
            break;
        case AGACHADO:
            persona->anim.frameAtual = 0;
            break;
    }
}

void loop(int mapa[LINHAS][COLUNAS], struct personagem *persona, struct personagem *inimigo, struct allegro elementos, struct camera *camera, struct background *bg){
    bool sair = false, redraw = false, noChao = false, bateuEsq = false, interacao = false;
    bool keys[4] = {false, false, false, false};
    struct tipoTiles desenhos;
    inicializarDesenhos(&desenhos);

    while(!sair){
        ALLEGRO_EVENT ev;
        al_wait_for_event(elementos.fila_eventos, &ev);

        if(ev.type == ALLEGRO_EVENT_TIMER){
            redraw = true;
            if(persona->est != AGACHADO){
                if(keys[2])
                    moverEsquerda(persona);
                if(keys[3])
                    moverDireita(persona);

                colisaoInimigo(persona, inimigo);
                interacao = colisaoHorizontal(persona, mapa);

                if(!interacao){
                    persona->velocidadeY += DECAIMENTOPULO;
                    persona->posY += persona->velocidadeY;
                    colisaoVertical(persona, mapa);        
                    noChao = estaNoChao(persona, mapa);    

                    if(keys[0] && noChao)
                        persona->velocidadeY = ALTURAPULO;

                    if(persona->vida == 0){
                        persona->posX = 0.0;
                        persona->posY = (float)(ALTURA_TELA - 100);
                    }
                } else {
                    noChao = true;
                    if(keys[0])
                        persona->posY -= 2.0;
                    if(keys[1]){
                        if(persona->posY < (float)((LINHAS - 2.3f) * 16))
                            persona->posY += 2.0;
                    }
                }

                camera->posX = persona->posX - (float)(LARGURA_TELA / 2);
                if(camera->posX <= 0.0)
                    camera->posX = 0.0;
            }

            atualizarEstado(keys, noChao, interacao, persona);
            atualizarPersonagem(persona);
            //atualizarPersonagem(inimigo);
            atualizarAnimacao(&inimigo->anim);
        }
        else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            sair = true;
        else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
            switch(ev.keyboard.keycode){
                case ALLEGRO_KEY_UP:    keys[0] = true; break;
                case ALLEGRO_KEY_DOWN:  keys[1] = true; break;
                case ALLEGRO_KEY_LEFT:  keys[2] = true; break;
                case ALLEGRO_KEY_RIGHT: keys[3] = true; break;
                case ALLEGRO_KEY_ESCAPE: sair = true;   break;
            }
        else if(ev.type == ALLEGRO_EVENT_KEY_UP)
            switch(ev.keyboard.keycode){
                case ALLEGRO_KEY_UP:    keys[0] = false; break;
                case ALLEGRO_KEY_DOWN:  keys[1] = false; break;
                case ALLEGRO_KEY_LEFT:  keys[2] = false; break;
                case ALLEGRO_KEY_RIGHT: keys[3] = false; break;
                case ALLEGRO_KEY_ESCAPE: sair = true;    break;
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
}

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
    loop(mapa, &persona, &inimigo, elementos, &camera, &bg);

    al_destroy_bitmap(elementos.fundo);
    al_destroy_display(elementos.janela);
    al_destroy_event_queue(elementos.fila_eventos);

    return 1;
}
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdbool.h>
#include "entidades.h"
#include "inicializar.h"

void moverCima(struct personagem *persona){
    persona->posY -= 4 * persona->velocidade;
}

void moverBaixo(struct personagem *persona){
    persona->posY += persona->velocidade;
}

void moverEsquerda(struct personagem *persona){
    persona->posX -= persona->velocidade;
    if(persona->posX < 0)
        persona->posX = 0;
}

void moverDireita(struct personagem *persona){
    persona->posX += persona->velocidade;
}

void colisaoVertical(struct personagem *persona, int mapa[LINHAS][COLUNAS]) {
    int iniciox = (int)persona->posX / TAMANHOTILE;
    int fimx    = ((int)persona->posX + persona->largura - 1) / TAMANHOTILE;
    int inicioy = (int)persona->posY / TAMANHOTILE;
    int fimy    = ((int)persona->posY + persona->altura - 1) / TAMANHOTILE;

    if(iniciox < 0)       iniciox = 0;
    if(fimx >= COLUNAS)   fimx = COLUNAS - 1;
    if(inicioy < 0)       inicioy = 0;
    if(fimy >= LINHAS)    fimy = LINHAS - 1;

    for(int lin = inicioy; lin <= fimy; lin++){
        for(int col = iniciox; col <= fimx; col++){
            int tipo = mapa[lin][col];
            struct tile temp = TILE[tipo];
            if(temp.hitboxEMCIMA || temp.hitboxINTEIRA){

                float c = (float)(col * TAMANHOTILE);
                float l = (float)(lin * TAMANHOTILE);
                float chao      = (persona->posY + (float)persona->altura) - l;
                float bateuCabeca = (l + (float)TAMANHOTILE) - persona->posY;

                if(persona->posX + (float)persona->largura > c && persona->posX < c + (float)TAMANHOTILE && persona->posY + (float)persona->altura >= l && persona->posY < l + (float)TAMANHOTILE){

                    if(chao < bateuCabeca){
                        persona->posY = l - (float)persona->altura;
                        persona->velocidadeY = 0;
                    } else {
                        persona->posY = l + (float)TAMANHOTILE;
                        persona->velocidadeY = 0;
                    }
                    if(temp.dano){
                        persona->vida--;
                        if(chao < bateuCabeca){
                            persona->posY -= 30.0;
                            persona->posX -= 50.0;
                        } else {
                            persona->posY += 30.0;
                        }
                    }
                }
            }
        }
    }
}

bool colisaoHorizontal(struct personagem *persona, int mapa[LINHAS][COLUNAS]) {
    int iniciox = (int)persona->posX / TAMANHOTILE;
    int fimx    = ((int)persona->posX + persona->largura - 1) / TAMANHOTILE;
    int inicioy = (int)persona->posY / TAMANHOTILE;
    int fimy    = ((int)persona->posY + persona->altura - 1) / TAMANHOTILE;

    if(iniciox < 0)       iniciox = 0;
    if(fimx >= COLUNAS)   fimx = COLUNAS - 1;
    if(inicioy < 0)       inicioy = 0;
    if(fimy >= LINHAS)    fimy = LINHAS - 1;

    for(int lin = inicioy; lin <= fimy; lin++){
        for(int col = iniciox; col <= fimx; col++){
            int tipo = mapa[lin][col];
            struct tile temp = TILE[tipo];
            float c = (float)(col * TAMANHOTILE);
            float l = (float)(lin * TAMANHOTILE);

            if(temp.hitboxEMCIMA || temp.hitboxINTEIRA){
                float bateuEsquerda = (persona->posX + (float)persona->largura) - c;
                float bateuDireita  = (c + (float)TAMANHOTILE) - persona->posX;

                if(persona->posX + (float)persona->largura > c &&persona->posX < c + (float)TAMANHOTILE && persona->posY + (float)persona->altura > l && persona->posY < l + (float)TAMANHOTILE){

                    if(temp.interacao)
                        return true;
                    if(bateuEsquerda < bateuDireita)
                        persona->posX = c - (float)persona->largura;
                    else
                        persona->posX = c + (float)TAMANHOTILE;
                }
                if(temp.dano){
                    persona->vida--;
                    if(bateuEsquerda < bateuDireita)
                        persona->posX -= 30.0;
                    else
                        persona->posX += 30.0;
                }
            }
        }
    }
    return false;
}

void colisaoInimigo(struct personagem *persona, struct personagem *inimigo){
    float bateuEsquerda = (persona->posX + (float)persona->largura) - (float)inimigo->posX;
    float bateuDireita  = ((float)inimigo->posX + (float)inimigo->largura) - persona->posX;
    float chao          = (persona->posY + (float)persona->altura) - (float)inimigo->posY;
    float bateuCabeca   = ((float)inimigo->posY + (float)inimigo->altura) - persona->posY;

    if(persona->posX + (float)persona->largura > (float)inimigo->posX && persona->posX < (float)inimigo->posX + (float)inimigo->largura && persona->posY + (float)persona->altura > (float)inimigo->posY && persona->posY < (float)inimigo->posY + (float)inimigo->altura){

        if((bateuEsquerda < chao && bateuEsquerda < bateuCabeca) || (bateuDireita  < chao && bateuDireita  < bateuCabeca)){
            if(bateuEsquerda < bateuDireita)
                persona->posX -= 30.0;
            else
                persona->posX += 30.0;
        } else {
            if(chao < bateuCabeca){
                persona->posY -= 30.0;
                persona->posX -= 50.0;
            } else {
                persona->posY += 30.0;
            }
        }
        persona->vida--;
    }
}

void desenharMapa(int mapa[LINHAS][COLUNAS], struct camera *camera, struct tipoTiles *desenhos){
    for(int lin = 0; lin < LINHAS; lin++){
        for(int col = 0; col < COLUNAS; col++){
            int tipo = mapa[lin][col];
            float telaX = (float)(col * TAMANHOTILE) - camera->posX;
            if(tipo != 0 && telaX > -(float)TAMANHOTILE && telaX < 680.0f){
                switch(tipo){
                    case 1: al_draw_bitmap(desenhos->chao, telaX, (float)(lin * TAMANHOTILE), 0); break;
                    case 2: al_draw_bitmap(desenhos->paredeEsq, telaX, (float)(lin * TAMANHOTILE), 0); break;
                    case 3: al_draw_bitmap(desenhos->meioParede, telaX, (float)(lin * TAMANHOTILE), 0); break;
                    case 4: al_draw_bitmap(desenhos->paredeDir, telaX, (float)(lin * TAMANHOTILE), 0); break;
                    case 5: al_draw_bitmap(desenhos->escada, telaX, (float)(lin * TAMANHOTILE), 0); break;
                    case 6: al_draw_bitmap(desenhos->perigo1, telaX, (float)(lin * TAMANHOTILE), 0); break;
                    case 7: al_draw_bitmap(desenhos->perigo2, telaX, (float)(lin * TAMANHOTILE), 0); break;
                    case 8: al_draw_bitmap(desenhos->perigo3, telaX, (float)(lin * TAMANHOTILE), 0); break;
                    case 9: al_draw_bitmap(desenhos->perigo4, telaX, (float)(lin * TAMANHOTILE), 0); break;
                    case 10: al_draw_bitmap(desenhos->perigo5, telaX, (float)(lin * TAMANHOTILE), 0); break;
                }
            }
        }
    }
}

void desenharPersonagem(struct personagem persona, struct camera *camera){
    int flag = persona.direcao ? ALLEGRO_FLIP_HORIZONTAL : 0;
    int flagEscalando = (persona.anim.frameAtual == 1) ? ALLEGRO_FLIP_HORIZONTAL : 0;
    float telaX = persona.posX - camera->posX;
    float telaY = persona.posY;
    
    switch(persona.est){
        case PARADO: al_draw_bitmap(persona.parado, telaX, telaY, flag); break;
        case AGACHADO: al_draw_bitmap(persona.agachar, telaX, telaY, flag); break;
        case ANDANDO_DIR: al_draw_bitmap(persona.andar[persona.anim.frameAtual], telaX, telaY, flag); break;
        case ANDANDO_ESQ: al_draw_bitmap(persona.andar[persona.anim.frameAtual], telaX, telaY, flag); break;
        case ESCALANDO: al_draw_bitmap(persona.escalar[persona.anim.frameAtual], telaX, telaY, flagEscalando); break;
        case PULANDO: al_draw_bitmap(persona.pular, telaX, telaY, flag); break;
    }
}

void desenharBackground(struct background *bg){
    al_draw_bitmap(bg->bg, (float)bg->x, (float)bg->y, 0);
    if(bg->x + bg->largura < LARGURA_TELA)
        al_draw_bitmap(bg->bg, (float)(bg->x + bg->largura), (float)bg->y, 0);
}

void desenharInimigo(struct personagem inimigo, struct camera *camera){
    al_draw_bitmap(inimigo.andar[inimigo.anim.frameAtual], inimigo.posX - camera->posX, inimigo.posY, 0);
}

void atualizarBackground(struct background *bg, struct camera *camera){
    bg->x = (int)(-(camera->posX * 0.5));
    if(bg->x + bg->largura < 0)
        bg->x += bg->largura;
    if(bg->x > 0)
        bg->x -= bg->largura;
}

void atualizarInimigo(struct personagem *inimigo, bool *bateuEsq){
    if(inimigo->posX <= 450.0)
        *bateuEsq = true;
    else if(inimigo->posX >= 550.0)
        *bateuEsq = false;

    if(!*bateuEsq)
        inimigo->posX -= inimigo->velocidade;
    else
        inimigo->posX += inimigo->velocidade;
}
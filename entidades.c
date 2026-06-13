#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdbool.h>
#include "entidades.h"
#include "inicializar.h"

void inicializarPersonagem(struct personagem *persona){
    persona->velocidade = 5;
    persona->vida = 3;
    persona->posX = 0; //ver isso
    persona->posY = ALTURA_TELA - 100;
    persona->velocidadeY = 0;
}

void desenharPersonagem(struct personagem persona, struct camera *camera){
    al_draw_filled_rectangle(persona.posX - camera->posX , persona.posY, persona.posX + 20 - camera->posX, persona.posY + 20, al_map_rgb(255, 0, 255));
}

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

bool colisaoVertical(struct personagem *persona, int mapa[LINHAS][COLUNAS]) {
    int iniciox = persona->posX / 32;
    int fimx    = (persona->posX + 19) / 32;
    int inicioy = persona->posY / 32;
    int fimy    = (persona->posY + 19) / 32;

    if(iniciox < 0)       
        iniciox = 0;
    if(fimx >= COLUNAS)   
        fimx = COLUNAS - 1;
    if(inicioy < 0)       
        inicioy = 0;
    if(fimy >= LINHAS)    
        fimy = LINHAS - 1;

    bool noChao = false;

    for(int lin = inicioy; lin <= fimy; lin++){
        for(int col = iniciox; col <= fimx; col++){
            int tipo = mapa[lin][col];
            struct tile temp = TILE[tipo];
            if(temp.hitboxEMCIMA || temp.hitboxINTEIRA){

                int c = col * 32;
                int l = lin * 32;

                if(persona->posX + 20 > c && persona->posX < c + 32 && persona->posY + 20 > l && persona->posY < l + 32){

                    int chao = (persona->posY + 20) - l;  
                    int bateuCabeca = (l + 32) - persona->posY;  

                    if(chao < bateuCabeca){
                        persona->posY = l - 20;  
                        persona->velocidadeY = 0;
                        noChao = true;
                    } else {
                        persona->posY = l + 32;  
                        persona->velocidadeY = 0;
                    }
                    if(temp.dano){
                        persona->vida--;
                        persona->posX -= 20;
                    }
                }
            }
        }
    }
    return noChao;
}

void colisaoHorizontal(struct personagem *persona, int mapa[LINHAS][COLUNAS]) {
    int iniciox = persona->posX / 32;
    int fimx    = (persona->posX + 19) / 32;
    int inicioy = persona->posY / 32;
    int fimy    = (persona->posY + 19) / 32;

    if(iniciox < 0)       
        iniciox = 0;
    if(fimx >= COLUNAS)   
        fimx = COLUNAS - 1;
    if(inicioy < 0)       
        inicioy = 0;
    if(fimy >= LINHAS)    
        fimy = LINHAS - 1;


    for(int lin = inicioy; lin <= fimy; lin++){
        for(int col = iniciox; col <= fimx; col++){
            int tipo = mapa[lin][col];
            struct tile temp = TILE[tipo];
            if(temp.hitboxEMCIMA || temp.hitboxINTEIRA){
                int c = col * 32;
                int l = lin * 32;

                if(persona->posX + 20 > c && persona->posX < c + 32 && persona->posY + 20 > l && persona->posY < l + 32){
                    int bateuEsquerda  = (persona->posX + 20) - c;  
                    int bateuDireita = (c + 32) - persona->posX;  

                    if(bateuEsquerda < bateuDireita)
                        persona->posX = c - 20;  
                    else
                        persona->posX = c + 32; 
                }
                if(temp.dano){
                    persona->vida--;
                    persona->posX -= 30;
                }
            }
        }
    }
}

void inicializarMapa(int mapa[LINHAS][COLUNAS]) {
    int temp[LINHAS][COLUNAS] = {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,2},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    };
    for (int lin = 0; lin < LINHAS; lin++)
        for (int col = 0; col < COLUNAS; col++)
            mapa[lin][col] = temp[lin][col];
}

void desenharMapa(int mapa[LINHAS][COLUNAS], struct camera *camera){
    ALLEGRO_BITMAP *chao;
    chao = al_load_bitmap("tile_0000.png"); // colocar isso numa funcao
    for(int lin = 0; lin < LINHAS; lin++){
        for(int col = 0; col < COLUNAS; col++){
            int tipo = mapa[lin][col];
            int telaX = (col * 32) - camera->posX;
            if(tipo != 0 && telaX < 680){ //  && telax > -32 && 
                if(tipo == 1)
                    al_draw_bitmap(chao, telaX, lin * 32, 0);
                else
                    al_draw_filled_rectangle(telaX, lin * 32, telaX + 32, lin * 32 + 32, al_map_rgb(255, 0, 255));
            }
        }
    }
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

void atualizaBackground(struct background *bg, struct camera *camera){
    bg->x = -camera->posX + bg->dirx;
    if(bg->x + bg->largura <= 0)
        bg->x = 0;
}

void desenharBackground(struct background *bg){
    al_draw_bitmap(bg->bg, bg->x, bg->y, 0);

    if(bg->x + bg->largura < LARGURA_TELA)
        al_draw_bitmap(bg->bg, bg->x + bg->largura, bg->y, 0);
        

}
//desenhar personagem
//inicializar personagem
//mover para cima
//mover para baixo
//mover para os lados
//o mover para baixo no futuro vai ser inutil, mas por ora para testar a movimentaçao deixa ai
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
    persona->posX = 20; //ver isso
    persona->posY = ALTURA_TELA - 100;
    persona->velocidadeY = 0;
}

void desenharPersonagem(struct personagem persona){
    al_draw_filled_rectangle(persona.posX, persona.posY, persona.posX + 20, persona.posY + 20, al_map_rgb(255, 0, 255));
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

bool verificarHitbox(struct personagem *persona, int mapa[15][20]){
    int iniciox = (persona->posX) / 32;
    int fimx = (persona->posX + 20) / 32;
    int inicioy = (persona->posY) / 32;
    int fimy = (persona->posY + 20) / 32;

    if(iniciox < 0)  
        iniciox = 0;
    if(fimx >= 20)  
        fimx = 19;
    if(inicioy < 0)  
        inicioy = 0;
    if(fimy >= 15)   
        fimy = 14;

    for(int col = iniciox; col <= fimx; col++){
        for(int lin = inicioy; lin <= fimy; lin++){
            int tipo = mapa[lin][col];
            struct tile temp = TILE[tipo];
            if(temp.hitboxEMCIMA || temp.hitboxINTEIRA){
                int c = col * 32;
                int l = lin * 32;
                if((persona->posX < c && persona->posX + 20 > c) || (persona->posY < l && persona->posY + 20 > l)){
                    if(persona->posY < l && persona->posY + 20 > l){
                        persona->posY = l - 19;
                        persona->velocidadeY = 0;
                    }
                    if(persona->posX < c && persona->posX + 20 > c)
                        persona->posX = c - 19;
                    return(true);
                }
                    
            }
        }
    }
    return(false);
}

void inicializarMapa(int mapa[15][20]) {
    int temp[15][20] = {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    };
    for (int lin = 0; lin < 15; lin++)
        for (int col = 0; col < 20; col++)
            mapa[lin][col] = temp[lin][col];
}

void desenharMapa(int mapa[15][20]){
    for(int lin = 0; lin < 15; lin++){
        for(int col = 0; col < 20; col++){
            int tipo = mapa[lin][col];
            if(tipo != 0)
                al_draw_filled_rectangle(col * 32, lin * 32, col * 32 + 32, lin * 32 + 32, al_map_rgb(255, 0, 255));
        }
    }
}
//desenhar personagem
//inicializar personagem
//mover para cima
//mover para baixo
//mover para os lados
//o mover para baixo no futuro vai ser inutil, mas por ora para testar a movimentaçao deixa ai
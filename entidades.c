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

bool colisaoVertical(struct personagem *persona, int mapa[LINHAS][COLUNAS]) {
    int iniciox = persona->posX / TAMANHOTILE;
    int fimx    = (persona->posX + persona->largura - 1) / TAMANHOTILE;
    int inicioy = persona->posY / TAMANHOTILE;
    int fimy    = (persona->posY + persona->altura - 1) / TAMANHOTILE;

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

                int c = col * TAMANHOTILE;
                int l = lin * TAMANHOTILE;
                int chao = (persona->posY + persona->altura) - l;  
                int bateuCabeca = (l + TAMANHOTILE) - persona->posY;   

                if(persona->posX + persona->largura > c && persona->posX < c + TAMANHOTILE && persona->posY + persona->altura > l && persona->posY < l + TAMANHOTILE){


                    if(chao < bateuCabeca){
                        persona->posY = l - persona->altura;  
                        persona->velocidadeY = 0;
                        noChao = true;
                    } else {
                        persona->posY = l + TAMANHOTILE;  
                        persona->velocidadeY = 0;
                    }
                    if(temp.dano){
                        persona->vida--;
                        if(chao < bateuCabeca){
                            persona->posY -= 30;  
                            persona->posX -= 50;
                        }
                        else
                            persona->posY += 30; 
                }
                }
            }
        }
    }
    return noChao;
}

bool colisaoHorizontal(struct personagem *persona, int mapa[LINHAS][COLUNAS]) {
    int iniciox = persona->posX / TAMANHOTILE;
    int fimx    = (persona->posX + persona->largura - 1) / TAMANHOTILE;
    int inicioy = persona->posY / TAMANHOTILE;
    int fimy    = (persona->posY + persona->altura - 1) / TAMANHOTILE;

    if(iniciox < 0)       
        iniciox = 0;
    if(fimx >= COLUNAS)   
        fimx = COLUNAS - 1;
    if(inicioy < 0)       
        inicioy = 0;
    if(fimy >= LINHAS)    
        fimy = LINHAS - 1;
    //bool interacao;

    for(int lin = inicioy; lin <= fimy; lin++){
        for(int col = iniciox; col <= fimx; col++){
            int tipo = mapa[lin][col];
            struct tile temp = TILE[tipo];
            int c = col * TAMANHOTILE;
            int l = lin * TAMANHOTILE;
            if(temp.hitboxEMCIMA || temp.hitboxINTEIRA){
                int bateuEsquerda  = (persona->posX + persona->largura) - c;  
                int bateuDireita = (c + TAMANHOTILE) - persona->posX;  
                if(persona->posX + persona->largura > c && persona->posX < c + TAMANHOTILE && persona->posY + persona->altura > l && persona->posY < l + TAMANHOTILE){
                    if(temp.interacao)
                        return(true);
                    if(bateuEsquerda < bateuDireita)
                        persona->posX = c - persona->largura;  
                    else
                        persona->posX = c + TAMANHOTILE; 
                }
                if(temp.dano){
                    persona->vida--;
                    if(bateuEsquerda < bateuDireita)
                        persona->posX -= 30;  
                    else
                        persona->posX += 30; 
                }
            }
        }
    }
    return(false);
}
void colisaoInimigo(struct personagem *persona, struct personagem *inimigo){
    int bateuEsquerda  = (persona->posX + persona->largura) - inimigo->posX;  
    int bateuDireita = (inimigo->posX + inimigo->largura) - persona->posX;  
    int chao = (persona->posY + persona->altura) - inimigo->posY;  
    int bateuCabeca = (inimigo->posY + inimigo->altura) - persona->posY;  

    if(persona->posX + persona->largura > inimigo->posX && persona->posX < inimigo->posX + inimigo->largura && persona->posY + persona->altura > inimigo->posY && persona->posY < inimigo->posY + inimigo->altura){
        if((bateuEsquerda < chao && bateuEsquerda < bateuCabeca) || (bateuDireita < chao && bateuDireita < bateuCabeca)){
            if(bateuEsquerda < bateuDireita)
                persona->posX -= 30;  
            else
                persona->posX += 30; 
        }else{
            if(chao < bateuCabeca){
                persona->posY -= 30;
                persona->posX -= 50;   
            }
            else
                persona->posY += 30; 
        }
        persona->vida--;
    }
}

void desenharMapa(int mapa[LINHAS][COLUNAS], struct camera *camera, struct tipoTiles *desenhos){
    for(int lin = 0; lin < LINHAS; lin++){
        for(int col = 0; col < COLUNAS; col++){
            int tipo = mapa[lin][col];
            int telaX = (col * TAMANHOTILE) - camera->posX;
            if(tipo != 0 && telaX > -TAMANHOTILE && telaX < 680){ //  && telax > -32 && 
                switch(tipo){
                    case 1:
                        al_draw_bitmap(desenhos->chao, telaX, lin * TAMANHOTILE, 0);
                        break;
                    case 2:
                        al_draw_bitmap(desenhos->paredeEsq, telaX, lin * TAMANHOTILE, 0);
                        break;
                    case 3:
                        al_draw_bitmap(desenhos->meioParede, telaX, lin * TAMANHOTILE, 0);
                        break;
                    case 4:
                        al_draw_bitmap(desenhos->paredeDir, telaX, lin * TAMANHOTILE, 0);
                        break;
                    case 5:
                        al_draw_bitmap(desenhos->escada, telaX, lin * TAMANHOTILE, 0);
                        break;
                }
            }
        }
    }
}

void desenharPersonagem(struct personagem persona, struct camera *camera){
    al_draw_filled_rectangle(persona.posX - camera->posX , persona.posY, persona.posX + persona.largura - camera->posX, persona.posY + persona.altura, al_map_rgb(255, 0, 255));
}

void desenharBackground(struct background *bg){
    al_draw_bitmap(bg->bg, bg->x, bg->y, 0);

    if(bg->x + bg->largura < LARGURA_TELA)
        al_draw_bitmap(bg->bg, bg->x + bg->largura, bg->y, 0);
}

void desenharInimigo(struct personagem inimigo, struct camera *camera){
    al_draw_filled_rectangle(inimigo.posX - camera->posX , inimigo.posY, inimigo.posX + inimigo.largura - camera->posX, inimigo.posY + inimigo.altura, al_map_rgb(120, 0, 120));
}

void atualizarBackground(struct background *bg, struct camera *camera){
    bg->x = -(camera->posX * 0.5);
    
    if(bg->x + bg->largura < 0) // quando ele sai completamente ele reseta 
        bg->x += bg->largura;
    if(bg->x > 0)
        bg->x -= bg->largura;
}

void atualizarInimigo(struct personagem *inimigo, bool *bateuEsq){
    //preciso definir pontos X limite para ele, quando atingir algum, tem q começar a voltar
    if(inimigo->posX <= 450){
        *bateuEsq = true;
    }else if(inimigo->posX >= 550){
        *bateuEsq = false; 
    }
    if(!*bateuEsq)
        inimigo->posX -= inimigo->velocidade;
    else
        inimigo->posX += inimigo->velocidade;
}
  
//desenhar personagem
//inicializar personagem
//mover para cima
//mover para baixo
//mover para os lados
//o mover para baixo no futuro vai ser inutil, mas por ora para testar a movimentaçao deixa ai
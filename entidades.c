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
    persona->posY = (LINHAS - 2) * 16 - 20;
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
    int iniciox = persona->posX / 16;
    int fimx    = (persona->posX + 19) / 16;
    int inicioy = persona->posY / 16;
    int fimy    = (persona->posY + 19) / 16;

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

                int c = col * 16;
                int l = lin * 16;
                int chao = (persona->posY + 20) - l;  
                int bateuCabeca = (l + 16) - persona->posY;   

                if(persona->posX + 20 > c && persona->posX < c + 16 && persona->posY + 20 > l && persona->posY < l + 16){


                    if(chao < bateuCabeca){
                        persona->posY = l - 20;  
                        persona->velocidadeY = 0;
                        noChao = true;
                    } else {
                        persona->posY = l + 16;  
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

void colisaoHorizontal(struct personagem *persona, int mapa[LINHAS][COLUNAS]) {
    int iniciox = persona->posX / 16;
    int fimx    = (persona->posX + 19) / 16;
    int inicioy = persona->posY / 16;
    int fimy    = (persona->posY + 19) / 16;

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
                int c = col * 16;
                int l = lin * 16;
                int bateuEsquerda  = (persona->posX + 20) - c;  
                int bateuDireita = (c + 16) - persona->posX;  
                if(persona->posX + 20 > c && persona->posX < c + 16 && persona->posY + 20 > l && persona->posY < l + 16){
                    if(bateuEsquerda < bateuDireita)
                        persona->posX = c - 20;  
                    else
                        persona->posX = c + 16; 
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
}

void inicializarMapa(int mapa[LINHAS][COLUNAS]) {
    FILE *f = fopen("mapa.txt", "r");
    if(!f){
        printf("Erro ao abrir arquivo\n");
        return;
    }
    char c;
    for (int lin = 0; lin < LINHAS; lin++)
        for (int col = 0; col < COLUNAS; col++){
            fscanf(f, " %c", &c);
            mapa[lin][col] = c - '0';
        }
    fclose(f);
            
}

void desenharMapa(int mapa[LINHAS][COLUNAS], struct camera *camera, ALLEGRO_BITMAP *chao){
    for(int lin = 0; lin < LINHAS; lin++){
        for(int col = 0; col < COLUNAS; col++){
            int tipo = mapa[lin][col];
            int telaX = (col * 16) - camera->posX;
            if(tipo != 0 && telaX > -16 && telaX < 680){ //  && telax > -32 && 
                if(tipo == 1)
                    al_draw_bitmap(chao, telaX, lin * 16, 0);
                else
                    al_draw_filled_rectangle(telaX, lin * 16, telaX + 16, lin * 16 + 16, al_map_rgb(255, 0, 255));
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
    bg->x = -(camera->posX * 0.5);
    
    if(bg->x + bg->largura < 0) // quando ele sai completamente ele reseta 
        bg->x += bg->largura;
    if(bg->x > 0)
        bg->x -= bg->largura;
}

void desenharBackground(struct background *bg){
    al_draw_bitmap(bg->bg, bg->x, bg->y, 0);

    if(bg->x + bg->largura < LARGURA_TELA)
        al_draw_bitmap(bg->bg, bg->x + bg->largura, bg->y, 0);
}

void inicializarInimigo(struct personagem *inimigo){
    inimigo->velocidade = 1;
    inimigo->vida = 0;
    inimigo->posX = 500; //ver isso
    inimigo->posY = (LINHAS - 1) * 16 - 20;
    inimigo->velocidadeY = 0;
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

void desenharInimigo(struct personagem inimigo, struct camera *camera){
    al_draw_filled_rectangle(inimigo.posX - camera->posX , inimigo.posY, inimigo.posX + 20 - camera->posX, inimigo.posY + 20, al_map_rgb(120, 0, 120));
}

void colisaoInimigo(struct personagem *persona, struct personagem *inimigo){
    int bateuEsquerda  = (persona->posX + 20) - inimigo->posX;  
    int bateuDireita = (inimigo->posX + 20) - persona->posX;  
    int chao = (persona->posY + 20) - inimigo->posY;  
    int bateuCabeca = (inimigo->posY + 20) - persona->posY;  
    if(persona->posX + 20 > inimigo->posX && persona->posX < inimigo->posX + 20 && persona->posY + 20 > inimigo->posY && persona->posY < inimigo->posY + 20){
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
  
//desenhar personagem
//inicializar personagem
//mover para cima
//mover para baixo
//mover para os lados
//o mover para baixo no futuro vai ser inutil, mas por ora para testar a movimentaçao deixa ai
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdbool.h>
#include "inicializar.h"
#include "entidades.h"


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
                    if(persona->posY < l && persona->posY + 20 > l)
                        persona->posY = l - 19;
                    if(persona->posX < c && persona->posX + 20 > c)
                        persona->posX = c - 19;
                    return(true);
                }
                    
            }
        }
    }
    return(false);
}

int main(){
    ALLEGRO_DISPLAY *janela = NULL;
    ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
    /*ALLEGRO_BITMAP *fundo = NULL;
    ALLEGRO_BITMAP *personagem = NULL;*/
    ALLEGRO_FONT *fonte = NULL;
    ALLEGRO_TIMER *timer = NULL;//20 15
    int mapa[15][20];
    int gravidade = 3;
    inicializarMapa(mapa);
    
    bool sair = false, redraw = false;
    bool keys[4] = {false, false, false, false};
    bool jump = false;
    
    if(!inicializar(&janela, &fila_eventos, &fonte, &timer))
        return (0);
    al_start_timer(timer);

    struct personagem persona;
    inicializarPersonagem(&persona);
    
    //inicializar personagem

    while(!sair){
        ALLEGRO_EVENT ev;
        al_wait_for_event(fila_eventos, &ev);
        printf("%d\n", persona.posY);
        if(ev.type == ALLEGRO_EVENT_TIMER){
            redraw = true;
            if(persona.posY == 429)
                    jump = false;
                if(jump)
                    persona.posY += gravidade; 
                if(!jump && keys[0]){
                    moverCima(&persona);
                    jump = true;
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
                    //mover personagem para direita
            verificarHitbox(&persona, mapa);
        }
        else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            sair = true;
        else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
            switch (ev.keyboard.keycode){
                case ALLEGRO_KEY_UP:
                    if(!jump){
                        keys[0] = true;
                        jump = true;
                    }
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
                   if(!jump){
                        keys[0] = false;
                        jump = false;
                   }
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
            redraw = false;                         
            al_clear_to_color(al_map_rgb(0, 0, 0)); 
            desenharMapa(mapa);                      
            desenharPersonagem(persona);             
            al_flip_display();                       
        }
    }
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
 
    return 1;
}
 

#ifndef ANIMACAO_H
#define ANIMACAO_H

void atualizarEstado(bool keys[4], bool noChao, bool interacao, struct personagem *persona);
void atualizarAnimacao(struct animacao *a);
void atualizarPersonagem(struct personagem *persona);

#endif
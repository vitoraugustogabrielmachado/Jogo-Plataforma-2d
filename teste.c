// Programa que exemplica como eh feito o tratamento de eventos na Allegro.
// Quando executado, eh aberta uma janela com uma estrela desenhada no seu
// centro. Atraves das teclas de seta eh possivel deslocar esta estrela para
// direita, para esquerda, para cima e para baixo. Clicando com um botao do
// mouse eh possivel intercalar a cor da estrela entre azul e amarela.
//
// Curso de Especializacao em Desenvolvimento de Jogos Digitais
// Disciplina: Programacao 2D
// Professores: Isabel H. Manssour e Marcio S. Pinho


#include <allegro5/allegro.h>
#include <allegro5/allegro_opengl.h>

// Variaveis que guardam a translacao da estrela.
int txEstrela = 0;
int tyEstrela = 0;

// Variavel que identifica a cor que a estrela sera desenhada.
bool corAmarela = true;

// Variavel que guarda a fila de eventos.
ALLEGRO_EVENT_QUEUE *event_queue;


// Funcao que desenha um triangulo centralizado na origem e
// pintado de amarelo ou azul.
void draw_triangle(void)
{
	if (corAmarela)
		glColor3f(1, 1, 0);
	else
		glColor3f(0, 0, 1);
	glBegin(GL_TRIANGLES);
		glVertex2f(-40, -40);
		glVertex2f(  0,  40);
		glVertex2f( 40, -40);
	glEnd();
}

// Funcao que desenha uma estrela centralizada na origem e
// formada por dois triangulos.
void draw_star(void)
{
	glTranslatef(0,15,0);
	draw_triangle();
	glTranslatef(0,-30,0);
	glScalef(1,-1,0);
	draw_triangle();
}

// Funcao que contem os comandos OpenGL para fazer um desenho na janela.
void draw_opengl(void)
{
	// Especifica a cor de fundo da janela
	glClearColor(0, 0, 0, 1);
	// Limpa a janela
	glClear(GL_COLOR_BUFFER_BIT);

	// Inicializa sistema de coordenadas de projecao
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Estabelece volume de visualizacao (xMin, xMax, yMin, yMax, zMin, zMax)
	glOrtho(-200, 200, -200, 200, -1, 1);

	// Inicializa sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Translada e desenha uma estrela
	glTranslatef(txEstrela,tyEstrela,0);
	draw_star();
}


// Funcao que estabelece o loop principal do programa. Aqui se fica a espera
// dos eventos (de qualquer fonte de eventos) para fazer o tratamento de cada
// um deles. Enquanto não houver evento, o programa fica "parado", consumindo
// pouco tempo de CPU.
static void main_loop(void)
{
	ALLEGRO_EVENT event;

	while (true) {
		// Verifica se a fila de eventos esta vazia para fazer o "refresh" da tela
		if (al_event_queue_is_empty(event_queue)) {
			draw_opengl();
			al_flip_display();
		}

		// Espera ate que a fila de eventos nao esteja vazia. O primeiro evento
		// da fila será copiado em "event" e removido da fila.
		al_wait_for_event(event_queue, &event);


		// Verifica qual foi o evento gerado e faz o tratamento adequado.
		// ALLEGRO_EVENT eh uma union, e a interpretacao de seu conteudo
		// depende do tipo de evento, que eh definido no campo "type".
		// Cada evento vem de uma fonte (mouse, joystick ou teclado,
		// por exemplo), que pode ser acessada por "event.any.source".
		switch (event.type)
		{
			// Indica que o botao de fechar da janela foi pressionado
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
					return; // Encerra a execucao do programa
					break;

			// Indica que um botao do mouse foi pressionado.
			case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
					corAmarela = !corAmarela;
					break;

			// Indica que uma tecla foi pressionada.
			// O campo "keycode" do evento "keyboard" contem uma constante que
			// representa a tecla.
			case ALLEGRO_EVENT_KEY_DOWN:
					if (event.keyboard.keycode == ALLEGRO_KEY_UP)
						tyEstrela+=3;
					else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN)
						tyEstrela-=3;
					else if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)
						txEstrela-=3;
					else if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
						txEstrela+=3;
					else if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
						return; // Encerra a execucao do programa
					break;

			// Indica que uma tecla foi pressionada por um tempo longo o suficiente
			// para "repetir" a acao. Este evento eh util quando se trabalhando
			// com uma interacao que requer uma entrada constante.
			// O campo "keycode" do evento "keyboard" contem uma constante que
			// representa a tecla.
			
	}
}
}


// Funcao principal.
int main(void)
{
    // Funcao de inicializacao da Allegro
	al_init();

	// Chamada de funcao para forcar o uso de OpenGL ao inves de DirectX (DirectX eh o default)
	al_set_new_display_flags(ALLEGRO_OPENGL);

	// Funcao para criar uma janela com as dimensoes passadas como parametro, que torna-se
	// automaticamente ativa, com o backbuffer selecionado para desenho. Os parametros
	// da janela sao determinados pelas ultimas chamadas a "al_set_new_display_*".
    ALLEGRO_DISPLAY *display = al_create_display(600, 600);

	// Funcao usada para especificar o titulo da janela.
	al_set_window_title(display, "Teste da Allegro com OpenGL e Eventos de Teclado");

	// Funcao que instala um driver de mouse. Se der problema, retorna false, senao retorna true.
	if ( !al_install_mouse() ){
		return 1; // encerra se deu problema
	}

	// Funcao que instala um driver de teclado. Se der problema, retorna false, senao retorna true.
	if ( !al_install_keyboard() ) {
		return 1; // encerra se deu problema
	}

	// Funcao que cria uma fila de eventos nova e vazia, retornando o ponteiro para o objeto criado,
	// ou NULL se ocorreu um erro.
	event_queue = al_create_event_queue();
	if (event_queue==NULL) {
		return 1; // encerra se deu problema
	}

	// al_register_event_source(): registra a fonte de eventos com a fila de eventos especificada.
	// al_get_mouse_event_source(): recupera a fonte de eventos do mouse.
	// al_get_keyboard_event_source(): recupera a fonte de eventos do teclado.
	// al_get_display_event_source(): recupera a fonte de evento associada a janela.
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(display));

	// Funcao que estabelece o loop do programa, que fica a espera de eventos
	main_loop();

    return 0;
}

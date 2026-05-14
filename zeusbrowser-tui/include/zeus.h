#ifndef ZEUS_H
#define ZEUS_H

#include <ncurses.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

/* --- CONFIGURAÇÕES DE NÚCLEO --- */
#define MAX_ITEMS      1024     
#define MAX_QUERY      512      
#define COMMAND_BUFFER 8192     
#define SPECTRUM_SIZE  256      

/* --- VARIÁVEIS GLOBAIS (Sinal para o Main) --- */
extern int spectrum_data[SPECTRUM_SIZE];
extern int peak_data[SPECTRUM_SIZE];

/* --- ESTADOS DO SISTEMA --- */
typedef enum { 
    MODO_LOCAL, 
    MODO_QUERY 
} ZeusState;

/* --- ESTRUTURA DE NAVEGAÇÃO --- */
typedef struct {
    char name[1024];           
    int is_dir;
} BrowseItem;

/* --- MOTOR VISUAL (THEMES & HUD) --- */
void init_player_colors();
void draw_archon_spectrum(int max_y, int max_x, Mix_Music *music);
void draw_tactical_load(int y, int x, const char *label);

/* --- MOTOR DE DADOS E EXECUÇÃO (ORQUESTRADOR) --- */
int  scan_folder(BrowseItem *items);
const char* detect_browser();  

/* 
   zeus_leap: Lançamento assíncrono via Duplo Fork.
   Usado para 'feh' (imagens) e o player de mídia ('nsix'),
   liberando o ZeusBrowser imediatamente sem gerar processos zumbis.
*/
void zeus_leap(const char *cmd);

/* 
   zeus_portal: Lançamento síncrono via Fork simples + waitpid.
   Usado exclusivamente para editores interativos como o 'nano',
   permitindo o controle temporário do terminal atual.
*/
void zeus_portal(const char *cmd); 

/* --- MOTOR DE ÁUDIO (SDL2_MIXER) --- */
void audio_callback(void *udata, Uint8 *stream, int len);

#endif /* ZEUS_H */

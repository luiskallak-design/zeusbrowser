#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <locale.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <limits.h>
#include <strings.h>
#include "zeus.h"

/* --- VARIÁVEIS DE ESTADO --- */
int selection = 0, offset = 0, total_files = 0, esquema_player = 0;
BrowseItem items[MAX_ITEMS];

/* Mapeamento de variáveis globais declaradas no .h */
int spectrum_data[SPECTRUM_SIZE];
int peak_data[SPECTRUM_SIZE];

/* --- MOTOR DE DADOS --- */
int scan_folder(BrowseItem *items_ptr) {
    DIR *dir = opendir("."); 
    if (!dir) return 0;
    struct dirent *entry; 
    int count = 0;
    while ((entry = readdir(dir)) != NULL && count < MAX_ITEMS) {
        if (strcmp(entry->d_name, ".") == 0) continue;
        snprintf(items_ptr[count].name, sizeof(items_ptr[count].name), "%s", entry->d_name);
        items_ptr[count].is_dir = (entry->d_type == DT_DIR);
        count++;
    }
    closedir(dir); 
    return count;
}

/* --- MOTOR DE DETECÇÃO DE BROWSER --- */
const char* detect_browser() {
    if (access("/usr/bin/chromium", X_OK) == 0) return "chromium";
    if (access("/usr/bin/brave", X_OK) == 0) return "brave";
    return "xdg-open";
}

/* --- ORQUESTRADOR DE PROCESSOS (BAIXO NÍVEL) --- */
void zeus_leap(const char *cmd) { 
    pid_t pid = fork();
    if (pid < 0) return;
    
    if (pid == 0) {
        setsid(); 
        pid_t grandson = fork();
        if (grandson < 0) _exit(1);
        
        if (grandson == 0) {
            int fd = open("/dev/null", O_RDWR);
            if (fd >= 0) {
                dup2(fd, 0); dup2(fd, 1); dup2(fd, 2); 
                close(fd);
            }
            execlp("sh", "sh", "-c", cmd, (char *)NULL);
            _exit(1);
        }
        _exit(0); 
    }
    waitpid(pid, NULL, 0); 
}

void zeus_portal(const char *cmd) {
    def_prog_mode();     
    endwin();            
    
    pid_t pid = fork();
    if (pid == 0) {
        execlp("sh", "sh", "-c", cmd, (char *)NULL);
        _exit(1);
    }
    if (pid > 0) {
        waitpid(pid, NULL, 0); 
    }
    
    reset_prog_mode();   
    refresh();           
}

/* --- MOTOR VISUAL --- */
void init_player_colors() {
    start_color(); use_default_colors();
    if (esquema_player == 0) { // EMERALD
        init_pair(1, COLOR_GREEN, -1); init_pair(2, COLOR_BLACK, COLOR_GREEN); init_pair(3, COLOR_CYAN, -1);
    } else if (esquema_player == 1) { // BLOOD
        init_pair(1, COLOR_RED, -1);   init_pair(2, COLOR_BLACK, COLOR_WHITE);  init_pair(3, COLOR_RED, -1);
    } else { // CYBER
        init_pair(1, COLOR_BLUE, -1);  init_pair(2, COLOR_BLACK, COLOR_MAGENTA); init_pair(3, COLOR_CYAN, -1);
    }
}

/* --- ORQUESTRADOR PRINCIPAL --- */
int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "");
    initscr(); init_player_colors(); noecho(); curs_set(0); keypad(stdscr, TRUE);

    total_files = scan_folder(items);
    const char *browser = detect_browser();

    while(1) {
        int my, mx; getmaxyx(stdscr, my, mx);
        erase();
        attron(COLOR_PAIR(3) | A_BOLD);
        char *cwd = getcwd(NULL, 0);
        mvprintw(0, 2, " ⌬ ZEUS_ORACLE // %s ", cwd); free(cwd);
        mvhline(1, 0, ACS_HLINE, mx);
        attroff(A_BOLD | COLOR_PAIR(3));

        int max_v = my - 4;
        if (selection < offset) offset = selection;
        if (selection >= offset + max_v) offset = selection - max_v + 1;

        for (int i = 0; i < max_v && (i + offset) < total_files; i++) {
            int idx = i + offset;
            if (idx == selection) attron(COLOR_PAIR(2));
            else attron(COLOR_PAIR(items[idx].is_dir ? 3 : 1));
            mvprintw(i + 2, 2, " %s %s ", (items[idx].is_dir ? "" : ""), items[idx].name);
            attroff(COLOR_PAIR(1) | COLOR_PAIR(2) | COLOR_PAIR(3));
        }

        // Rodapé reestruturado para impressão limpa baseada no layout da sua imagem
        int p = 0;
        attron(COLOR_PAIR(1) | A_REVERSE);
        mvprintw(my-1, p, " F1:HOME "); p += 9;
        mvprintw(my-1, p, " F2:GGLE "); p += 9;
        mvprintw(my-1, p, " F3:DUCK "); p += 9;
        mvprintw(my-1, p, " F4:YTUB "); p += 9;
        mvprintw(my-1, p, " F5:THEME "); p += 10;
        mvprintw(my-1, p, " Q:EXIT "); p += 8;
        mvprintw(my-1, p, " ENTER:INVOKE "); p += 14;
        mvhline(my-1, p, ' ', mx - p);
        attroff(A_REVERSE);
        refresh();

        int ch = getch();
        if (ch == 'q' || ch == 'Q') break; 
        if (ch == KEY_UP && selection > 0) selection--;
        if (ch == KEY_DOWN && selection < total_files - 1) selection++;

        if (ch == KEY_F(1)) { chdir(getenv("HOME")); total_files = scan_folder(items); selection = 0; offset = 0; }
        if (ch == KEY_F(2)) { char c[1024]; snprintf(c, 1024, "%s --app=https://google.com", browser); zeus_leap(c); }
        if (ch == KEY_F(3)) { char c[1024]; snprintf(c, 1024, "%s --app=https://duckduckgo.com", browser); zeus_leap(c); }
        if (ch == KEY_F(4)) { char c[1024]; snprintf(c, 1024, "%s --app=https://youtube.com", browser); zeus_leap(c); }
        if (ch == KEY_F(5)) { esquema_player = (esquema_player + 1) % 3; init_player_colors(); }

        if (ch == 10) { // ENTER
            if (items[selection].is_dir) {
                chdir(items[selection].name); total_files = scan_folder(items); selection = 0; offset = 0;
            } else {
                /* CORREÇÃO DAS ARRAYS: Buffers dimensionados corretamente para evitar corrupção */
                char cmd[8192], abs_f[PATH_MAX];
                if (realpath(items[selection].name, abs_f) != NULL) {
                    char *ext = strrchr(abs_f, '.');

                    if (ext && (strcasecmp(ext, ".jpg") == 0 || strcasecmp(ext, ".png") == 0 || strcasecmp(ext, ".jpeg") == 0)) {
                        snprintf(cmd, sizeof(cmd), "feh -. -g 800x600+auto '%s'", abs_f);
                        zeus_leap(cmd);
                    } 
                    else if (ext && (strcasecmp(ext, ".txt") == 0 || strcasecmp(ext, ".md") == 0 || strcasecmp(ext, ".cfg") == 0)) {
                        snprintf(cmd, sizeof(cmd), "nano '%s'", abs_f);
                        zeus_portal(cmd);
                        total_files = scan_folder(items);
                    } 
                    else {
                        snprintf(cmd, sizeof(cmd), "kitty --class ZEUS_PLAYER -o show_tab_bar=no sh -c \"/opt/archia/projects/archonplayer/bin/archonplayer '%s' -c %d\"", abs_f, esquema_player);
                        zeus_leap(cmd);
                    }
                }
            }
        }
    }
    endwin();
    return 0;
}

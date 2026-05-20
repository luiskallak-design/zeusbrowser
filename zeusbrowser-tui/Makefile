# --- ZEUS-BROWSER TUI: O RADAR DO OLIMPO ---
TARGET = bin/zeusbrowser
CC     = gcc

# Repositório correto da dependência essencial
ARCHON_GIT = https://github.com/luiskallak-design/archonplayer-tui

# Cores (Estética Azul/Divina)
BLUE    = \033[0;34m
CYAN    = \033[0;36m
WHITE   = \033[1;37m
BOLD    = \033[1m
RESET   = \033[0m

# FLAGS (Unificadas e Protegidas contra Undefined Reference)
CFLAGS  = -Iinclude -Wall -O3 $(shell sdl2-config --cflags)
LIBS    = -lncursesw $(shell sdl2-config --libs) -lSDL2 -lSDL2_mixer

# ARQUIVOS
SRC     = src/main.c
OBJ     = obj/main.o

# --- OPERAÇÕES ---

.PHONY: all setup clean install archon_dep

all: setup archon_dep $(TARGET)
	@echo -e "\n$(BLUE)$(BOLD)⌬ [ZEUS] RADAR DE BUSCA SINCRONIZADO!$(RESET)"
	@echo -e "$(CYAN)❱❱ Navegação TUI pronta em: $(TARGET)$(RESET)\n"

# Prepara os diretórios
setup:
	@mkdir -p bin obj
	@echo -e "$(CYAN)⌬ ESTABELECENDO CONEXÃO COM O ÉTER...$(RESET)"

# Verifica e instala o ArchonPlayer se não estiver presente no sistema
archon_dep:
	@if ! command -v archonplayer >/dev/null 2>&1; then \
		echo -e "$(WHITE)$(BOLD)⌬ [SINAL]$(RESET) $(BLUE)ArchonPlayer não detectado. Iniciando forja automática...$(RESET)"; \
		git clone $(ARCHON_GIT) /tmp/archonplayer-tui && \
		cd /tmp/archonplayer-tui && \
		make && \
		sudo make install && \
		rm -rf /tmp/archonplayer-tui; \
		echo -e "$(CYAN)⌬ [ECOSISTEMA] ArchonPlayer forjado e linkado com sucesso!$(RESET)\n"; \
	else \
		echo -e "$(BLUE)⌬ [ECOSISTEMA] ArchonPlayer já integrado ao ecossistema. Sincronizado.$(RESET)"; \
	fi

# Linkagem Final
$(TARGET): $(OBJ)
	@echo -e "$(BLUE)⌬ CONSOLIDANDO ÓPTICA: $(RESET)$@"
	@$(CC) $(OBJ) -o $(TARGET) $(LIBS)

# Compilação do DNA do Zeus
obj/%.o: src/%.c
	@echo -e "$(WHITE)⌬ PROCESSANDO SINAL: $(RESET)$<"
	@$(CC) -c $< -o $@ $(CFLAGS)

# Limpeza
clean:
	@rm -rf obj bin
	@echo -e "$(BLUE)⌬ [ZEUS]: SISTEMA DESLIGADO. RASTROS LIMPOS.$(RESET)"

# Instalação com o "Papo Reto" Final
install: all
	@echo -e "$(CYAN)⌬ INSTALANDO RADAR NO SISTEMA...$(RESET)"
	@sudo cp $(TARGET) /usr/local/bin/zeusbrowser
	@echo -e "\n$(WHITE)$(BOLD)⌬ [SISTEMA]$(RESET) Zeus-Browser está online em $(CYAN)/usr/local/bin/$(RESET)"
	@echo -e "$(BLUE)⌬ DICA FINAL:$(RESET) Agora que você tem o Zeus, o $(BOLD)CARONTE-CTRL$(RESET) vai ser seu melhor guia."


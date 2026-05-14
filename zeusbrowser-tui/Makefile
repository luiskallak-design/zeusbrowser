# --- ZEUS-BROWSER TUI: O RADAR DO OLIMPO ---
TARGET = bin/zeusbrowser
CC     = gcc

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

.PHONY: all setup clean install

all: setup $(TARGET)
	@echo -e "\n$(BLUE)$(BOLD)⌬ [ZEUS] RADAR DE BUSCA SINCRONIZADO!$(RESET)"
	@echo -e "$(CYAN)❱❱ Navegação TUI pronta em: $(TARGET)$(RESET)\n"

# Prepara os diretórios
setup:
	@mkdir -p bin obj
	@echo -e "$(CYAN)⌬ ESTABELECENDO CONEXÃO COM O ÉTER...$(RESET)"

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

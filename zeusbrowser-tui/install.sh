#!/usr/bin/env bash
# --- ZEUS-BROWSER: INSTALADOR UNIVERSAL MULTI-DISTRO ---

BOLD='\033[1m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
GREEN='\033[0;32m'
RED='\033[0;31m'
RESET='\033[0m'

echo -e "${BLUE}${BOLD}⌬ [ZEUS] INICIANDO MAPEAMENTO DO SISTEMA OPERACIONAL...${RESET}"

# 1. DETECÇÃO DA DISTRIBUIÇÃO NATIVA
if [ -f /etc/os-release ]; then
    . /etc/os-release
    DISTRO=$ID
else
    echo -e "${RED}❌ Erro: Não foi possível identificar a assinatura da distribuição.${RESET}"
    exit 1
fi

echo -e "${CYAN}⌬ Distro detectada: ${BOLD}${DISTRO}${RESET}"
echo -e "${CYAN}⌬ Instalando dependências essenciais do núcleo de áudio e vídeo...${RESET}"

# 2. INSTALAÇÃO DE DEPENDÊNCIAS VIA GERENCIADOR NATIVO (SEM PPAs)
case "$\${DISTRO}" in
    arch|manjaro|artix)
        sudo pacman -Sy --needed --noconfirm base-devel ncurses sdl2 sdl2_mixer feh nano kitty
        ;;
    ubuntu|debian|pop|mint)
        sudo apt-get update
        sudo apt-get install -y build-essential libncursesw5-dev libsdl2-dev libsdl2-mixer-dev feh nano kitty
        ;;
    fedora|nobara)
        sudo dnf groupinstall -y "Development Tools"
        sudo dnf install -y ncurses-devel SDL2-devel SDL2_mixer-devel feh nano kitty
        ;;
    opensuse*|suse)
        sudo zypper install -t pattern devel_basis
        sudo zypper install -y ncurses-devel libSDL2-devel libSDL2_mixer-devel feh nano kitty
        ;;
    *)
        echo -e "${RED}⚠️ Alerta: Distribuição não mapeada formalmente.${RESET}"
        echo -e "${CYAN}Certifique-se de ter GCC, Make, Ncurses, SDL2, SDL2_mixer, Feh e Nano instalados.${RESET}"
        ;;
esac

# 3. COMPILAÇÃO DO DNA DO ZEUS
echo -e "\n${BLUE}${BOLD}⌬ [ZEUS] INICIANDO COMPILAÇÃO DO CÓDIGO FONTE...${RESET}"
if [ -f Makefile ]; then
    make clean
    make
else
    echo -e "${RED}❌ Erro: Makefile não encontrado no diretório atual.${RESET}"
    exit 1
fi

# 4. INSTALAÇÃO DO BINÁRIO NO CORE DO SISTEMA
if [ -f bin/zeusbrowser ]; then
    echo -e "\n${CYAN}⌬ Movendo binário consolidado para o path global (/usr/local/bin)...${RESET}"
    sudo cp bin/zeusbrowser /usr/local/bin/zeusbrowser
    echo -e "${GREEN}${BOLD}⌬ [SUCESSO] ZeusBrowser está online e pronto para execução!${RESET}"
    echo -e "${CYAN}Digite ${BOLD}zeusbrowser${RESET}${CYAN} em qualquer terminal para invocar o Radar.${RESET}\n"
else
    echo -e "${RED}❌ Erro: Falha na compilação do binário.${RESET}"
    exit 1
fi

# Nome do executável
EXECUTAVEL = main

# Arquivos com fonte
FONTES = main.c

# Flags do compilador
CFLAGS = -Wall -g -I./SDL2/include/SDL2/ -I./SDL2_ttf/include/SDL2/

# Bibliotecas
LIBS = -L./SDL2/lib -L./SDL2_ttf/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf

# Regra padrão
all: $(EXECUTAVEL)

# Regra para compilar o executável
$(EXECUTAVEL): $(FONTES)
	gcc $(CFLAGS) $(FONTES) -o $(EXECUTAVEL) $(LIBS)

# Regra para limpar os arquivos gerados
clean:
	rm -f $(EXECUTAVEL)

# Regra para executar o programa
run: all
	./$(EXECUTAVEL)
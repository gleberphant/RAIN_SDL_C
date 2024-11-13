# Nome do executável
EXECUTAVEL = main

# Arquivos com fonte
FONTES = main.c

# Flags do compilador
CFLAGS = -Wall -g -I./SDL3/include/

# Bibliotecas
LIBS = -lmingw32 -lSDL3 -lSDL3_ttf -lSDL3_image
LIBS_PATH = -L./SDL3/lib

# Regra padrão
all: $(EXECUTAVEL)

# Regra para compilar o executável
$(EXECUTAVEL): $(FONTES)
	gcc $(CFLAGS) $(FONTES) -o $(EXECUTAVEL) $(LIBS_PATH) $(LIBS)

# Regra para limpar os arquivos gerados
clean:
	rm -f $(EXECUTAVEL)

# Regra para executar o programa
run: all
	./$(EXECUTAVEL)
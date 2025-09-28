# Makefile pour le projet OceanDepth

# Variables
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
TARGET = oceandepth
SRCDIR = src
SOURCES = $(wildcard $(SRCDIR)/*.c) $(wildcard $(SRCDIR)/*/*.c)
OBJECTS = $(SOURCES:.c=.o)
HEADERS = $(wildcard $(SRCDIR)/*.h) $(wildcard $(SRCDIR)/*/*.h)

# Règle par défaut
all: $(TARGET)

# Compilation de l'exécutable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

# Compilation des fichiers objets
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers générés
clean:
	rm -f $(OBJECTS) $(TARGET)

# Nettoyage complet (inclut les fichiers de sauvegarde)
distclean: clean
	rm -f *~ $(SRCDIR)/*~ $(SRCDIR)/*/*~

# Reconstruction complète
rebuild: clean all
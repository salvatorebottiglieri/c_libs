# Nome della libreria (senza "lib" o estensione)
LIB_NAME = sep_error

# Compilatore e flag
CC = gcc
CFLAGS = -Wall -Wextra -fPIC -Iinclude

# Percorsi di installazione standard
INSTALL_PATH_LIB = /usr/local/lib
INSTALL_PATH_INCLUDE = /usr/local/include

# Trova automaticamente tutti i file .c nella cartella src
SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:.c=.o)

# Target di default: crea sia la libreria statica che quella condivisa
all: lib$(LIB_NAME).a lib$(LIB_NAME).so

# Regola per creare la libreria condivisa (.so)
lib$(LIB_NAME).so: $(OBJS)
	$(CC) -shared -o $@ $^

# Regola per creare la libreria statica (.a)
lib$(LIB_NAME).a: $(OBJS)
	ar rcs $@ $^

# Regola generica per compilare i file .c in .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Target per l'installazione
install: all
	@echo "Installando libreria e header in /usr/local..."
	# Copia le librerie
	sudo cp lib$(LIB_NAME).so $(INSTALL_PATH_LIB)
	sudo cp lib$(LIB_NAME).a $(INSTALL_PATH_LIB)
	# Copia gli header
	sudo cp include/*.h $(INSTALL_PATH_INCLUDE)
	# Aggiorna la cache del linker per la libreria condivisa
	sudo ldconfig
	@echo "Installazione completata."

# Target per la disinstallazione
uninstall:
	@echo "Disinstallando libreria e header da /usr/local..."
	sudo rm -f $(INSTALL_PATH_LIB)/lib$(LIB_NAME).so
	sudo rm -f $(INSTALL_PATH_LIB)/lib$(LIB_NAME).a
	sudo rm -f $(INSTALL_PATH_INCLUDE)/$(LIB_NAME).h
	sudo ldconfig
	@echo "Disinstallazione completata."

# Pulisce i file generati
clean:
	rm -f *.o *.a *.so
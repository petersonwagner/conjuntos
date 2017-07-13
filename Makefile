CFLAGS = -Wall -g

all: trab verificador

trab: trab.c
	gcc $(CFLAGS) trab.c -g -o trab

verificador: verificador.c
	gcc $(CFLAGS) verificador.c -o verificador

clean:
	rm -f trab verificador
	rm -rf testes

#!/bin/bash
echo rm -rf log_file.txt
rm -rf log_file.txt
echo rm -rf testes
rm -rf testes
echo mkdir testes
mkdir testes
echo make all
make all

echo -e "\n\ngerando testes:"

COUNTER=0
while [  $COUNTER -lt 10 ]; do
	echo -e "\n\n========$COUNTER========\n"

	for i in 1 2 4 8;
	do
		echo "./gerador.py -n ${i}000 -x ${i}00 -b ${i}00 -s ${i}00 -p ${i}00 -k 1000 -e 10 -o testes/entrada_${i}k_$COUNTER.txt -r testes/saida_${i}k_$COUNTER.txt"
		./gerador.py -n ${i}000 -x ${i}00 -b ${i}00 -s ${i}00 -p ${i}00 -k 1000 -e 10 -o testes/entrada_${i}k_$COUNTER.txt -r testes/saida_${i}k_$COUNTER.txt;
	done;

	echo -e "\n\nexecutando programa:\n"
	 
	for i in 1 2 4 8;
	do
		echo "./trab < testes/entrada_${i}k_$COUNTER.txt > testes/prog_${i}k_$COUNTER.txt"
		./trab < testes/entrada_${i}k_$COUNTER.txt > testes/prog_${i}k_$COUNTER.txt;
	done;

	echo -e "\n\nverificando resultados:\n"
	 
	for i in 1 2 4 8;
	do
		echo "./verificador testes/saida_${i}k_$COUNTER.txt testes/prog_${i}k_$COUNTER.txt"
		./verificador testes/saida_${i}k_$COUNTER.txt testes/prog_${i}k_$COUNTER.txt;
	done;

	let COUNTER=COUNTER+1
done;
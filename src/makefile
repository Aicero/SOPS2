compile :
	gcc fryzjer.c -o compiled/fryzjer -pthread
	clear

ct :
	make compile
	clear
	make test

test :
	compiled/fryzjer
	
params :
	compiled/fryzjer 10 -zmienne -debug

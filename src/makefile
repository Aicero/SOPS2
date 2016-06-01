compile :
	rm -rf compiled
	mkdir compiled
	gcc fryzjer.c -o compiled/fryzjer -pthread

ct :
	make compile
	clear
	make test

tm :
	compiled/fryzjer 5
	
tz :
	compiled/fryzjer 10 -zmienne

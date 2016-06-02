compile :
	rm -rf compiled
	mkdir compiled
	gcc fryzjer.c -o compiled/fryzjer -pthread

ctm :
	make compile
	clear
	make tm

tm :
	compiled/fryzjer 7
	
tz :
	compiled/fryzjer 10 -zmienne

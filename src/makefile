compile :
	rm -rf compiled
	mkdir compiled
	gcc fryzjer.c -o compiled/fryzjer -pthread

ctm :
	make compile
	clear
	make tm

tm :
	compiled/fryzjer 5
	
tmd :
	compiled/fryzjer 2 -debug
	
tz :
	compiled/fryzjer 10 -zmienne

tzd :
	compiled/fryzjer 2 -debug -zmienne
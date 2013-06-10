all:
	cd src; qmake; make

clean:
	cd src; qmake; make clean
	rm src/Makefile
	rm -f bin/CQIllustrator

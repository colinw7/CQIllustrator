all:
	cd src; qmake; make

clean:
	cd src; qmake; make clean
	rm -f src/Makefile
	rm -f bin/CQIllustrator

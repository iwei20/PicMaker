all: run

run: picmaker.out
	./picmaker.out
	-display picmaker.ppm
	echo picmaker.ppm

picmaker.out: picmaker.cpp
	g++ -o picmaker.out picmaker.cpp

clean:
	rm picmaker.out

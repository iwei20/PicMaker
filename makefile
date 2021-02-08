picmake:
	g++ -o picmaker.out picmaker.cpp
	./picmaker.out
	-display picmaker.ppm
	echo picmaker.ppm;

test: test.o canny_filter.o
	clang++ -g test.o canny_filter.o -o test

test.o: test.cpp canny_filter.cpp
	clang++ -g -c test.cpp matrix.h canny_filter.cpp canny_filter.h -std=c++11 -lstdc++
all: test
clean:
	rm *.gch *.o
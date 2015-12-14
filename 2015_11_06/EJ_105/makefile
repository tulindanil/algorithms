all:
	g++ main.cpp -O2 -o test

test:
	g++ test.cc -O2 -o test && ./test && rm test && echo 'tests passed' || echo 'tests is not passed'
	g++ main.cpp -O2 -o test

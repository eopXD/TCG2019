all:
	g++ -std=c++11 -D CONSERVATIVE src/baseline.cpp -o conservative
	g++ -std=c++11 -D GREEDY src/baseline.cpp -o greedy
	g++ -std=c++11 -D RANDOM src/baseline.cpp -o random

conservative:
	g++ -std=c++11 -D CONSERVATIVE src/baseline.cpp -o conservative

random:
	g++ -std=c++11 -D RANDOM src/baseline.cpp -o random

greedy:
	g++ -std=c++11 -D GREEDY src/baseline.cpp -o greedy

clean:
	rm -rf greedy
	rm -rf conservative
	rm -rf random
	rm -rf .log.*

#!/bin/bash
i=0
g++ -std=c++11 main.cpp -o hey
g++ -std=c++11 inputgenerator.cpp -o inputwali 

while ( (($i < 30))  ); do 	
	g++ -std=c++11 inputgenerator.cpp -o inputwali 
	./inputwali inputwala.txt
	./hey < inputwala.txt 
	i=$[$i +1]
done
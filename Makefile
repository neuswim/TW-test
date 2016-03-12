CXX = g++ -g 

test/test.exe : test/test.o src/item.o lib/json.o lib/configfile.o
	$(CXX) -o test/test.exe test.o item.o json.o configfile.o
	rm *.o
test/test.o : test/test.cpp
	$(CXX) -c test/test.cpp -I include/ -I lib/
src/item.o : src/item.cpp
	$(CXX) -c src/item.cpp -I include/ -I lib/
lib/json.o : lib/json.cpp
	$(CXX) -c -std=c++0x lib/json.cpp -I lib/
lib/configfile.o : lib/configfile.cpp
	$(CXX) -c lib/configfile.cpp -I lib/

clean :
	if [ -f *.o ]; then rm *.o; fi;
	if [ -f test/test.exe ]; then rm test/test.exe; fi;

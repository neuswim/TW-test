CXX = g++ -g 

test/test.exe : test/test.o src/item.o json/json.o
	$(CXX) -o test/test.exe test.o item.o json.o
	rm *.o
test/test.o : test/test.cpp
	$(CXX) -c test/test.cpp -I include/
src/item.o : src/item.cpp
	$(CXX) -c src/item.cpp -I include/ -I json/
json/json.o : json/json.cpp
	$(CXX) -c -std=c++0x json/json.cpp -I json/

clean :
	rm *.o
	rm test/test.exe

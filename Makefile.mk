hashtagcounter: hashtag.o
	g++ --std=c++11 hashtag.o -o hashtagcounter

hashtag.o: hashtag.cpp
	g++ -c --std=c++11 hashtag.cpp

run: ./hashtagcounter
	./hashtagcounter

clean:
	rm *.o hashtagcounter
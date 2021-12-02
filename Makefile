all:  webcrawler
webcrawler: webcrawler.cpp openhttp.cpp SimpleHTMLParser.cpp PunctuationParser.h HashTableTemplate.h
	g++  -g  webcrawler.cpp openhttp.cpp SimpleHTMLParser.cpp PunctuationParser.h -lcurl

test-hashTable: test_hash_table.cc HashTableTemplate.h
	g++ -g --std=c++17 -Wall -o hashTableTest test_hash_table.cc HashTableTemplate.h
clean:
	rm -f *.o gethttp core webcrawler

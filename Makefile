all:  git-commit gethttp webcrawler

gethttp: gethttp.cpp openhttp.cpp SimpleHTMLParser.cpp
	g++ -o gethttp -g gethttp.cpp openhttp.cpp SimpleHTMLParser.cpp -lcurl

webcrawler: webcrawler.cpp openhttp.cpp SimpleHTMLParser.cpp PunctuationParser.h HashTableTemplate.h
	g++  -g  webcrawler.cpp openhttp.cpp SimpleHTMLParser.cpp PunctuationParser.h -lcurl

test-hashTable: test_hash_table.cc HashTableTemplate.h
	g++ -g --std=c++17 -Wall -o hashTableTest test_hash_table.cc HashTableTemplate.h

git-commit:
	git add *.cpp *.h Makefile >> .local.git.out  || echo
	git commit -a -m 'Commit' >> .local.git.out || echo
	git push


clean:
	rm -f *.o gethttp core webcrawler

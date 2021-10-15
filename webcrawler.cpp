#include <string.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <iterator>

#include "webcrawler.h"
#include "SimpleHTMLParser.h"
#include "openhttp.h"
#include "HashTableTemplate.h"
#include "PunctuationParser.h"

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define NC "\033[0m"

// Add your implementation here




using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::list;


//TODO problem is with the _headURL iteration check that


int main(int argc, char const *argv[])
{

    int urlListSize = 0;
	int urlListMaxSize = 0;
	//const char ** urlRoots = (const char **)malloc((argc - 1)*sizeof(char *));
	if(argc == 1){
        cout<<RED<<"Please provide at least 1 url for crawling."<<NC<<endl;
    }
    else if(argc >= 2){
        if(!strcmp(argv[1], "-u")){
            if(argc == 2){
                cout<<RED<<"Please Enter at least One starting url!"<<NC<<endl;
                exit(1);
            }
            urlListMaxSize = atoi(argv[2]);

			//std::cout<<urlListMaxSize<<std::endl;
            if(!urlListMaxSize && argc <= 3){
                cout<<RED<<"Please Enter a valid List Number!"<<NC<<endl;
				EXIT_FAILURE;
            }
            else{
				const char* urlRoots[argc - 3];
				//std::cout<<"Here 1"<<std::endl;
				for(int i = 3;i < argc;i++){
					urlRoots[i - 3] = strdup(argv[i]);
				}
				//std::cout<<"Here 2"<<std::endl;
				WebCrawler crawler(urlListMaxSize, argc - 3, urlRoots);
				crawler.crawl();
            }
        }
		else{
			const char* urlRoots[argc - 1];
			urlListMaxSize = 1000;
			for(int i = 1;i < argc;i++){
				urlRoots[i-1] = strdup(argv[i]);
			}
			WebCrawler crawler(urlListMaxSize, argc - 1, urlRoots);
            crawler.crawl();
		}
        
        
    }else{
        cout<<RED<<"Wrong format<<endl";
    }
    
    return 0;
}

class HTMLParserContent: public SimpleHTMLParser{
    public:
        void onContentFound();
};


// the constructor only puts initial urls into URLRecords and insert into the _urlArray; does not contain descriptions for the urls
// the descriptions will be added the traversing while loop
WebCrawler::WebCrawler(int maxUrls, int nurlRoots, const char ** urlRoots){
    _maxUrls = maxUrls;
    _urlArray = (URLRecord *)malloc(_maxUrls*sizeof(URLRecord));
    _headURL = 0; 
    _tailURL = 0;
	_urlToUrlRecord = new HashTable<int>();
	_wordToURLRecordList = new HashTable<list<int> >();
	//std::cout<<*urlRoots<<std::endl;
    int indexofURLArray = 0; // index for inserting the URLRecord Object into their array corresponding to the index of the url in the _urlArray
    int n = 0; //length of the url content
    int wordCount = 0; // word count for the description
    // Enqueuing initial urls given in the argument of the program
    char * urlContent;
    


    for (int i = 0; i < nurlRoots;i++){
    	urlContent = fetchHTML(strdup(urlRoots[i]), &n);
        if(urlContent != NULL && strlen(urlContent) != 0){
            URLRecord newRecord;
            newRecord._url = strdup(urlRoots[i]);
			// the string as a key to check if the hash table already contains the url key
			
			string urlCheck = urlRoots[i];
			//std::cout<<urlCheck<<endl;
			int urlIndex = 0;
			if(!_urlToUrlRecord->find(urlCheck, &urlIndex)){
				// if the find table function does not find the key in the hash table add the URLRecord into the hash table
				_urlToUrlRecord->insertItem(urlCheck, indexofURLArray);
				_urlArray[indexofURLArray] = newRecord; // add the URLRecord into the _urlArray
				indexofURLArray++; // increment index
			}
        }
    }
    _tailURL = indexofURLArray;
    if(_tailURL == 0){
        cout<<RED<<"All Input URL(s) are not valid!"<<endl;
        cout<<RED<<"No Files Created"<<endl;
        cout<<NC;
        EXIT_FAILURE;
        exit(1);
    }
	//std::cout<<"Tail URL: "<<_tailURL<<std::endl;
	//
}



void
WebCrawler::crawl(){ 
	
	// a C string for the content retrieved from the urls
	//char * urlContent = NULL;
	
	// the number of char of the content fetched by fetchHTML
	int nForFetchHTML;

	// the HTML doc parser for parsing the links and the contents
	HTMLParserContent multiParser;
	
	PunctuationParser puncParser(" ");

	// the string for urlDescriptions that go into the _description of an URLRecord struct that is in the _urlArray limited to 500 chars not counting space
	string urlDescription = "";

	// the string for the entire contents parsed regardless of number of chars; this is for storing words into the word Hash Table
	string urlContentWithoutTags = "";

	// the links retrieved from urls in _urlArray new urls are inserted into _urlArray and the Hash Table mapping urls to index in _urlArray
	vector<string> urlListFromLink;
	vector<string> wordList;
	list<int> wordIndexList;

	// the int for the find function for url Hash Table; if url is found in Hash Table then fetch the index of the url in _urlArray and assign it to index
	int index = 0;

	//the while loop traversing the urls in _urlArray; adding urls to _urlArray
	while(_headURL < _tailURL){

		//reinitialze the url description and other object
		urlDescription = "";
		urlContentWithoutTags = "";
		urlListFromLink.clear();
		wordList.clear();
        wordIndexList.clear();
		//urlContent = NULL;

		//fetch the document from the url stored in _urlArray using the index _headURL
        std::cout<<"---------------------------------------------"<<std::endl;
        std::cout<<"Number of Urls in the URL Hash Table: "<<_urlToUrlRecord->nElement<<std::endl;
        std::cout<<std::endl<<"Current URL: "<<_urlArray[_headURL]._url<<std::endl<<std::endl;
	std::cout<<std::endl<<"Current Index Number in the _urlArray: "<<_headURL<<std::endl;
		char * urlContent = fetchHTML(strdup(_urlArray[_headURL]._url), &nForFetchHTML);
		if(urlContent==NULL || strlen(urlContent) == 0){
			std::cout<<"Invalid URL!!!!"<<std::endl;
			// if the content retrieved by fetchHTML isn't valid continue on to the next item in _urlArray and remove it from the HashTable and _urlArray	
			_urlToUrlRecord->removeElement(_urlArray[_headURL]._url);
            std::cout<<"Removed Url: "<<_urlArray[_headURL]._url<<std::endl;
			for(int i = _headURL; i < _tailURL; i++){
				_urlArray[i] = _urlArray[i+1];
			}
			_tailURL--;
            std::cout<<"Number of Urls in the URL Hash Table after removal: "<<_urlToUrlRecord->nElement<<std::endl;
			continue;
		}
        
		//parsing the HTML web page fetched from the url
		multiParser.parse(urlContent, nForFetchHTML, strdup(_urlArray[_headURL]._url));

		// Add title to the url description


		//urlDescription = "Title: \n" + multiParser.getSiteTitle() + "\n";


		// Add main body of description to the url
		string siteDescription = multiParser.getSiteDescription();
		if(siteDescription.empty()){
			siteDescription = "There is no description available for this site";
		}
		urlDescription =  siteDescription + "...";

		// update the description of the URLRecord object of the url;
		_urlArray[_headURL]._description = strdup(urlDescription.c_str());

		//retrieve the url list parsed from the current url
		urlListFromLink = multiParser.getSiteUrlList();

		//traverse the list
		//
		if(_tailURL < _maxUrls){
				
			for(std::vector<string>::iterator itr = urlListFromLink.begin(); itr != urlListFromLink.end(); itr++){
				string newUrl = *itr;
				std::cout<<"URLS from the link: "<<newUrl<<std::endl;
                
				//
				if(!_urlToUrlRecord->find(newUrl, &index)){
					std::cout<<"Inserting URL .... "<<std::endl;	
					_urlToUrlRecord->insertItem(newUrl, _tailURL);  
					 std::cout<<"Number of Urls in the URL Hash Table after insertion: "<<_urlToUrlRecord->nElement<<std::endl;
					URLRecord newRecord;
					newRecord._url = strdup(newUrl.c_str());
					newRecord._description =  strdup("");
					_urlArray[_tailURL] = newRecord;
					_tailURL++;

				}
                else{
                    std::cout<<"Link already recorded!"<<endl;
                }
				
                //exit for loop adding links if _urlArray is full
				if(_tailURL == _maxUrls){
					break;
				}
			}// end of for loop for adding urls to both the hashtable and _urlArray
            
		}// end if for checking size of _urlArray



		//std::cout<<"Getting Through the insertion of links"<<std::endl;
		
        //Parsing the webpage content strip out text and punctuations
		urlContentWithoutTags = multiParser.getSiteContent(); 
		puncParser.setParseContent(urlContentWithoutTags);
		puncParser.parsePunctuation();
		wordList = puncParser.getParsedContent();
			
        

		//std::cout<<"Word From the page: "<<std::endl;
		for(std::vector<string>::iterator itrW = wordList.begin(); itrW != wordList.end(); itrW++){
			string newWord = *itrW;
            wordIndexList.clear();
			
            
            _wordToURLRecordList->find(newWord, &wordIndexList);
            
            //prevent from adding duplicate index when a word appears in a webpage multiple times
            if(!wordIndexList.empty() && wordIndexList.back() == _headURL){
                continue;
            }
            wordIndexList.push_back(_headURL);
            _wordToURLRecordList->insertItem(newWord, wordIndexList);
		} // end of for loop for adding words 



		_headURL++;

	}// end of while loop for iterating through _urlArray


			
	int loopIndex = 0;
	HashTableIterator<int> urlListsitr(*_urlToUrlRecord);
	string demoKey = "";
	int demoIndex = 0;

    for(int i = 0;i < _urlToUrlRecord->getTableSize(); i++){
        if(_urlToUrlRecord->_buckets[i].empty()){
            continue;
        }
        for(vector<HashTableEntry<int> >::iterator itr = _urlToUrlRecord->_buckets[i].begin(); itr != _urlToUrlRecord->_buckets[i].end(); itr++){
            cout<<endl<<YELLOW<<"Index: "<<demoIndex<<endl;
            cout<<GREEN<<"Url: "<<(*itr)._key<<NC<<endl;
            cout<<BLUE<<"Index in _urlArray: "<<(*itr)._data<<NC<<endl;
            demoIndex++;
        }
        
    }
    
    cout<<endl<<endl<<endl;
    
    for(int i = 0;i < _wordToURLRecordList->getTableSize(); i++){
        if(_wordToURLRecordList->_buckets[i].empty()){
            continue;
        }
        for(vector<HashTableEntry<list<int> > >::iterator itr = _wordToURLRecordList->_buckets[i].begin(); itr != _wordToURLRecordList->_buckets[i].end(); itr++){
            cout<<endl<<YELLOW<<"Index: "<<demoIndex<<endl;
            cout<<GREEN<<"Word: "<<(*itr)._key<<NC<<endl;
            cout<<BLUE<<"Indexs of url in _urlArray the word is in: ";
            for(list<int>::iterator wordIndexItr = (*itr)._data.begin(); wordIndexItr != (*itr)._data.end(); wordIndexItr++){
                if((++wordIndexItr)-- == (*itr)._data.end()){
                    cout<<BLUE<<*wordIndexItr<<".";
                }
                else{
                    cout<<BLUE<<*wordIndexItr<<", ";
                }
                
            }
            cout<<endl;
            demoIndex++;
        }
        
    }//
    
    cout<<NC;
	cout<<endl<<endl<<GREEN<<"End of Crawling!"<<endl<<endl;
        
    cout<<endl<<endl<<GREEN<<"Start Outputing Crawled Results..."<<endl;

    cout<<NC;
    this->writeUrlFile("url.txt");
    this->writeWordFile("word.txt");
    
}// end of crawl function
   

// this function is for writing urls and their index in the _urlArray into a text file given the file name
void
WebCrawler::writeUrlFile(string urlFileName){
    std::ofstream urlOutput (urlFileName.c_str());
    string url;
    string description;
    
    if(urlOutput.is_open()){
        //urlOutput<<"URLs Crawled along with their Description!\n";
        for(int i = 0; i < _tailURL; i++){
            url = _urlArray[i]._url;
            description = _urlArray[i]._description;
            urlOutput<<i<<" "<<url<<"\n";
            urlOutput<<description<<"\n";
            urlOutput<<"\n";
        }
        
        cout<<GREEN<<"Successfully Saved URL Crawl Result! Please Check Folder for File"<<endl<<endl<<endl;
        cout<<NC;
    }
    urlOutput.close();
}


// this function is for writing words found in the crawled pages and the the index of the urls where the words were found in the _urlArray 
void
WebCrawler::writeWordFile(string wordFileName){
    std::ofstream wordOutput (wordFileName.c_str());
    string word;
    string index;
    
    if(wordOutput.is_open()){
        
	//wordOutput<<"Words Contained in Crawled Pages\n";
        
	list<int> wordList(_maxUrls/2);


        for(int i = 0;i < _wordToURLRecordList->getTableSize(); i++){
            if(_wordToURLRecordList->_buckets[i].empty()){
                continue;
            }
            for(vector<HashTableEntry<list<int> > >::iterator itr = _wordToURLRecordList->_buckets[i].begin(); itr != _wordToURLRecordList->_buckets[i].end(); itr++){
                wordOutput<<(*itr)._key<<" ";
                //wordOutput<<"Indices of url in _urlArray the word is in: \n";
                for(list<int>::iterator wordIndexItr = (*itr)._data.begin(); wordIndexItr != (*itr)._data.end(); wordIndexItr++){
                   wordOutput<<(* wordIndexItr)<<" "; 
                }
		wordOutput<<"\n";
            }
            
        }
        cout<<GREEN<<"Successfully Saved WORD Crawl Result! Please Check Folder for File"<<endl<<endl<<endl;
        cout<<NC;
    }
    wordOutput.close();
}


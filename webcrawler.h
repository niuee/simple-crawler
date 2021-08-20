
// CS251 Definitions for web-crawler


#include <vector>
#include <string>
#include <list>

#include "HashTableTemplate.h"


using std::list;
using std::vector;
using std::string;



// Stores a URL and the first 100 words
struct URLRecord {
  char * _url;          // URL of the document.
  char * _description;  // First 100 words of the document without tags
};



class WebCrawler {
  // The url array stores all the URLs that have been scanned
    int _maxUrls;            // Max size of the URL array
    URLRecord * _urlArray;   // Array of URLs
    int _headURL;            // Points to the next URL to scan by the web crawler
    int _tailURL;            // Next position unused in the urlArray

    HashTable<int> * _urlToUrlRecord;         //  maps a URL to its index in the URLArray
  
    HashTable<list<int> > *_wordToURLRecordList; // maps a word to a list of URLs
    

public:
  // Create a web crawler object with the initial urls
  WebCrawler(int maxUrls, int nurlRoots, const char ** urlRoots);
  
  // crawl the web
  void crawl();

  // Write array of URLs and descriptions to file
  void writeUrlFile(string urlFileName);

  // Write list of words with their urls to file
  void writeWordFile(string wordFileName);

  // Add any other objects you may need


};



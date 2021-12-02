
#ifndef SIMPLEHTMLPARSER_H
#define SIMPLEHTMLPARSER_H

#include "openhttp.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

class SimpleHTMLParser {
    // URL read in anchor
protected:
    int urlAnchorLength;
    char urlAnchor[MaxURLLength];
    
    vector<string> siteUrlList;
    string siteDescription;
    int descriptionCharCount;

    string siteHeader;
    string siteContent;

    bool containSiteHeader;
    bool containSiteDescription;
    
    bool match(char **b, const char *m);
 public:
    SimpleHTMLParser();
    
    bool parse(char * buffer, int n, char * urlCurrent);
    // Calbacks
    virtual void onContentFound(char c);
    virtual void onAnchorFound(char * url);

    string getSiteDescription();
    vector<string> getSiteUrlList();
    string getSiteTitle();
    string getSiteContent();
    
};

#endif


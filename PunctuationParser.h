//
//  main.cpp
//  Testing
//
//  Created by Vincent Chang on 7/4/18.
//  Copyright © 2018 Vincent Chang. All rights reserved.
//


#include <string>
#include <vector>
#include <iostream>
#include <string.h>
using std::string;




class PunctuationParser{
    std::vector<string> _parsedContent;
    int _urlContentlength;
    string _urlContent;
    
    
public:
    PunctuationParser(string urlContent);
    std::vector<string> getParsedContent(){return _parsedContent;};
    bool setParseContent(string urlContent);
    void parsePunctuation();
protected:
    bool matchContent(char ** contentToMatch, const char * puncTuation);
    
};

PunctuationParser::PunctuationParser(string urlContent){
    _urlContent = urlContent;
}

bool
PunctuationParser::setParseContent(std::string urlContent){
    if(urlContent.empty()){
        return false;
    }
    _urlContent = urlContent;
    return true;
}

bool
PunctuationParser::matchContent(char **contentToMatch, const char * punctuation){
    int len = (int) strlen(punctuation);
    if(!strncasecmp(*contentToMatch, punctuation, len)){
        (*contentToMatch) += len;
        return true;
    }
    return false;
}

void
PunctuationParser::parsePunctuation(){
    
    enum{START, PUNCTUATION, SPACE, WEBADDRESS}state;
    state = START;
    string word = "";
    string punC = "";
    char * parsingContent = strdup(_urlContent.c_str());
    char * contentEnd = parsingContent + _urlContent.length();
    _parsedContent.clear();
    bool lastCharSpace = false;
    while(parsingContent < contentEnd){
        switch(state){
            case START:
                if(matchContent(&parsingContent, ",")){
                    state = PUNCTUATION;
                    punC = ",";
                }
                else if(matchContent(&parsingContent, ";")){
                    state = PUNCTUATION;
                    punC = ";";
                }
                else if(matchContent(&parsingContent, "/")){
                    state = PUNCTUATION;
                    punC = "/";
                }
                else if(matchContent(&parsingContent, ":")){
                    state = PUNCTUATION;
                    punC = ":";
                }
                else if(matchContent(&parsingContent, "\"")){
                    state = PUNCTUATION;
                    punC = "\"";
                }
                else if(matchContent(&parsingContent, "!")){
                    state = PUNCTUATION;
                    punC = "!";
                }
                else if(matchContent(&parsingContent, "(")){
                    state = PUNCTUATION;
                    punC = "(";
                }
                else if(matchContent(&parsingContent, ")")){
                    state = PUNCTUATION;
                    punC = ")";
                }
                else if(matchContent(&parsingContent, "? ")){
                    state = PUNCTUATION;
                    punC = "?";
                }
                else if(matchContent(&parsingContent, ". ") || (parsingContent == contentEnd - 1 && matchContent(&parsingContent, "."))){
                    state = PUNCTUATION;
                    punC = ".";
                }
                else if(matchContent(&parsingContent, "...")){
                    state = PUNCTUATION;
                    punC = "...";
                }
                else if(matchContent(&parsingContent, " ")){
                    state = SPACE;
                }
                else if(matchContent(&parsingContent, "http://")){
                    word = word + "http://";
                    state = WEBADDRESS;
                }
                else if(matchContent(&parsingContent, "https://")){
                    word = word + "https://";
                    state = WEBADDRESS;
                }
                else if(matchContent(&parsingContent, "www.")){
                    word = word + "www.";
                    state = WEBADDRESS;
                }
                else{
                    state = START;
                    word = word + *parsingContent;
                    //if(parsingContent == contentEnd){
                    //_parsedContent.push_back(word);
                    //}
                    parsingContent++;
                    lastCharSpace = false;
                }
                if(parsingContent == contentEnd){ // check if the content is reaching the end after parsing in case the last char is a punctuation that would make the parsingContent to reach the content end before making into the next switch round, thus a word and a punc would be left out
                    if(!word.empty()){ // make sure there is no more words left out
                        _parsedContent.push_back(word);
                        word= "";
                    }
                    if(!punC.empty()){ // make sure there is no more punctuations left out
                        _parsedContent.push_back(punC);
                        punC = "";
                    }
                }
                break;
            case PUNCTUATION:
                if(!word.empty()){ // in case the punctuation is trailing a char
                    _parsedContent.push_back(word);
                    word = "";
                }
                _parsedContent.push_back(punC);
                punC = "";
                state = START;
                lastCharSpace = false;
                break; // end of punctuation cases
            case SPACE:
                if(lastCharSpace){ // double space guard
                    state = START;
                    lastCharSpace = true;
                    break;
                }
                if(!word.empty()){
                    _parsedContent.push_back(word);
                    word = "";
                }
                state = START;
                lastCharSpace = true;
                break; // end of space case
            case WEBADDRESS:
                if(*parsingContent != ' '){
                    word = word + *parsingContent;
                    parsingContent++;
                    state = WEBADDRESS;
                }
                else{
                    state = START;
                    lastCharSpace = false;
                }
                break; // end of case webaddress
            default:;
        }//end of switch cases
        
    }//end of while loop
}//end of parsePunctuation

#include "SimpleHTMLParser.h"
#include "openhttp.h"
#include <string.h>
#include <string>
#include <iostream>
//#include <boost/algorithm/string/predicate.hpp>

using std::string;

SimpleHTMLParser::SimpleHTMLParser()
{
	siteDescription = "";
	siteHeader = "";
	siteContent = "";
	descriptionCharCount = 0;
	containSiteDescription = false;
	containSiteHeader = false;

}

bool
SimpleHTMLParser::match(char **b, const char *m) {
	int len = strlen(m);
	if ( !strncasecmp(*b, m, len)) {
		(*b)+=len;
		return true;
	}
	return false;
}

bool
SimpleHTMLParser::parse(char * buffer, int n, char * urlCurrent)
{
	enum { START, TAG, SCRIPT, ANCHOR, HREF,
	       COMMENT, FRAME, SRC, STYLE, ESCAPE, META, METACONTENT, METANAME, BODY, HEADER} state;

	state = START;
	string metaContent = "";
	string metaTag = "";
	string urlStrip = "";
	string urlParsing = urlCurrent;
	char * bufferEnd = buffer + n;
	char * b = buffer;
	bool lastCharSpace = true;

	int urlBackSlashPos = 0;

	siteContent = "";
	siteDescription = "";
	siteHeader = "";
	descriptionCharCount = 0;
	containSiteDescription = false;
	containSiteHeader = false;
	siteUrlList.clear();
	
	bool startHeaderRecord = false;
    if(urlParsing.empty()){
        return false;
    }
	//  make sure the root url does not contain invalid strings such as /index.html
	if(!urlParsing.compare(urlParsing.length() - 4, 4, "html")|| !urlParsing.compare(urlParsing.length() - 3, 3, "php")){
		for(int i = urlParsing.length() - 1;i >=0; i-- ){
			if(urlParsing[i] == '/'){
				urlBackSlashPos = i;
				break;
			}
		}
		urlParsing = urlParsing.substr(0, urlBackSlashPos);
	}
    if(!urlParsing.compare(urlParsing.length() - 1, 1, "/")){
        urlParsing = urlParsing;
    }
    else{
        urlParsing = urlParsing + "/";
    }
	
    //std::cout<<"URL Parsing in HTML Parser: "<<urlParsing<<std::endl;



	while (b < bufferEnd) {
		//printf("<%c,%d,%d>", *b, *b,state);
		switch (state) {
		case START: {
			if (match(&b,"<SCRIPT")) {
				state = SCRIPT;
			}
			else if (match(&b, "<STYLE")){
				state = STYLE;
			}
			else if (match(&b,"<!--")) {
				state = COMMENT;
			}
			else if (match(&b,"<A ")) {
				state = ANCHOR;
			}
			else if (match(&b,"<IFRAME ") || match(&b, "<FRAME ")) {
				state = FRAME;
			}
			else if (match(&b, "<META")){
				if(!containSiteDescription){	
					state = META;
				}
				else{
					state = TAG;
				}
			}
            else if (match(&b, "</a>")){
                siteContent = siteContent + " ";
            }
			else if (match(&b, "<BODY")){
				state = BODY;
			}
			else if (match(&b, "<TITLE")){
				if(containSiteHeader){
					state = START;
                    b++;
					break;
				}
				startHeaderRecord = false;
				state = HEADER;
			}
			else if	(match(&b,"<")) {
				state = TAG;
			}
			else if (match(&b, "&")){
				state = ESCAPE;
			}
			else {
				if(!containSiteDescription){	
					char c = *b;
					//Substitute one or more blank chars with a single space
					if (c=='\n'||c=='\r'||c=='\t'||c==' ') {
						// when a space or new line is found
						if (!lastCharSpace) {
							// the space is right after a char not another space
							siteDescription = siteDescription + " ";
							siteContent = siteContent + " ";
							onContentFound(' ');
						}
						lastCharSpace = true;
					}
					else {
						if(descriptionCharCount >= 500){
							containSiteDescription = true;
							break;
						}
						siteDescription = siteDescription + c;
						siteContent = siteContent + c;
						descriptionCharCount++;
						onContentFound(c);
						lastCharSpace = false;
					}	
					
				}
				else{
					char c = *b;
					//Substitute one or more blank chars with a single space
					if (c=='\n'||c=='\r'||c=='\t'||c==' ') {
						// when a space or new line is found
						if (!lastCharSpace) {
							// the space is right after a char not another space
							siteContent = siteContent + " ";
							//onContentFound(' ');
						}
						lastCharSpace = true;
					}
					else {
						siteContent = siteContent + c;
						//onContentFound(c);
						lastCharSpace = false;
					}			
				}
				b++;
			}
			break;
		}
		case ANCHOR: {
			if (match(&b,"href=\"")) {
				state = HREF;
				urlAnchorLength=0;
				//printf("href=");
			}
			else if (match(&b,">")) {
				// End script
				state = START;
			}
			else {
				b++;
			}
			break;
				
		}
		case HREF: {
			if (match(&b,"\"")) {
				// Found ending "
				state = ANCHOR;
				urlAnchor[urlAnchorLength] = '\0';
				urlStrip = urlAnchor;
                //std::cout<<"URL in Tag: "<<urlStrip<<std::endl;
                if(!urlStrip.compare(0,1,"#")){
                    urlStrip = "";
                }
				else if(!urlStrip.compare(0, 3, "../")){
				    urlStrip = urlStrip.substr(3, urlStrip.length() - 3);
                    urlStrip = urlParsing + urlStrip;
				}
				else if(!urlStrip.compare(0, 4, "/../") ){
				    urlStrip = urlStrip.substr(4, urlStrip.length() - 4);
                    urlStrip = urlParsing + urlStrip;
				}
				else if(!urlStrip.compare(0, 1, "/")){
					urlStrip = urlStrip.substr(1, urlStrip.length() - 1);
                    urlStrip = urlParsing + urlStrip;
				}
                else if(!urlStrip.compare(0,7,"http://")){
                    urlStrip = urlStrip.substr(7, urlStrip.length()-6);
                    urlStrip = "https://" + urlStrip;
                }
                else if(urlStrip.compare(0, 5, "https") && urlStrip.compare(0, 6, "//www.") && urlStrip.compare(0, 3, "ftp") && urlStrip.compare(0, 4, "/www")){
                    urlStrip = urlParsing + urlStrip;
                }
                else if(urlStrip.length() < 2){
                    urlStrip = "";
                }
                
                if(urlStrip.empty()){
                    urlStrip = "";
                    break;
                }
                else if(urlStrip.compare(urlStrip.length() - 4, 4, ".pdf") && urlStrip.compare(urlStrip.length() - 4, 4, ".png") && urlStrip.compare(urlStrip.length() - 4, 4, ".jpg") && urlStrip.compare(urlStrip.length() - 4, 4, ".PDF")){
					//guard the url from being a file that's not text 
					//fetchHTML will retrieve files that are not text and turn them into char * which the file is not i.e. image files and pdf files
					//Possible adding other none text file in the future
                    //std::cout<<"URL pushed: "<<urlStrip<<std::endl;
					siteUrlList.push_back(urlStrip);
				}
				//onAnchorFound(urlAnchor);
				//printf("\n");
			}
			else {
				if ( urlAnchorLength < MaxURLLength-1) {
					urlAnchor[urlAnchorLength] = *b;
					urlAnchorLength++;
				}
				//printf("%c", *b, *b);
				b++;
			}
			break;
		}
		case FRAME: {
			if (match(&b,"src=\"")) {
				state = SRC;
				urlAnchorLength=0;
				//printf("href=");
			}
			else if (match(&b,">")) {
				// End script
				state = START;
			}
			else {
				b++;
			}
			break;
				
		}
		case SRC: {
			if (match(&b,"\"")) {
				// Found ending "
				state = FRAME;
				urlAnchor[urlAnchorLength] = '\0';
				urlStrip = urlAnchor;
				if(urlStrip.compare(0, 4, "http") && urlStrip.compare(0, 4, "www.") && urlStrip.compare(0, 6, "//www.") && urlStrip.compare(0, 3, "ftp")){
					urlStrip.insert(0, urlParsing); 
				}
				siteUrlList.push_back(urlStrip);
				//onAnchorFound(urlAnchor);
				//printf("\n");
			}
			else {
				if ( urlAnchorLength < MaxURLLength-1) {
					urlAnchor[urlAnchorLength] = *b;
					urlAnchorLength++;
				}
				//printf("%c", *b, *b);
				b++;
			}
			break;
		}
		case SCRIPT: {
			if (match(&b,"/SCRIPT>")) {
				// End script
				state = START;
			}
			else {
				b++;
			}
			break;
		}
		case COMMENT: {
			if (match(&b,"-->")) {
				// End comments
				state = START;
			}
			else {
				b++;
			}
			break;
		}
		case TAG: {
			if (match(&b, ">")) {
				state = START;
			}
			else {
				b++;
			}
			break;
		}
		case STYLE: {
		    if(match(&b, "/STYLE>")){
                state = START;
		    }
		    else{
                b++;
		    }
		    break;
		}
		case ESCAPE: {
			if(match(&b, ";")){
				state = START;
			}
			else{
				b++;
			}
			break;
		}
		case META:{
			if(match(&b, "content=\"")){
				state = METACONTENT;
			}
			else if(match(&b, "name=\"")){
				state = METANAME;
			}
			else if(match(&b, ">")){
				if(!metaTag.compare("description") || !metaTag.compare("Description") || !metaTag.compare("DESCRIPTION")){
					int metaIndex = 0;
					int metaCharCount = 0;
                    siteContent = siteContent + metaContent;
					while(metaCharCount < 500 && metaIndex < metaContent.length()){
						if(metaContent[metaIndex] != '\r' && metaContent[metaIndex] != '\n' && metaContent[metaIndex] != ' ' && metaContent[metaIndex] != '\t'){
							metaCharCount++;
						}
						else{
							metaContent[metaIndex] = ' ';
						}
						siteDescription = siteDescription + metaContent[metaIndex];
						metaIndex++;
					}
					metaIndex = 0;
					metaCharCount = 0;
					containSiteDescription = true;
					metaTag = "";
					metaContent = "";
				}
				else{
					metaTag = "";
					metaContent = "";
				}
				state = START;
			}
			else{
				b++;
			}
			break;
		}
		case METACONTENT:{
            
			if(match(&b, "\"")){
				//end of meta content
				state = META;
			}
			else{
				metaContent = metaContent + *b;
				b++;
			}
			break;
		}
		case METANAME:{
			if(match(&b, "\"")){
				//end of meta name tag
				state = META;
			}
			else{
				metaTag = metaTag + *b;
				b++;
			}
			break;
		}
		case BODY:{
			if(siteDescription.empty()){
				containSiteDescription = false;
			}
			if(match(&b, ">")){
				state = START;
			}
            else{
                b++;
            }

			break;
		}
		case HEADER:{
			if(match(&b, "</TITLE>")){
				//end of header 
				containSiteHeader = true;
				state = START;
				break;
			}
			if(startHeaderRecord){
				if(match(&b, "&amp;")){
					siteHeader = siteHeader + "&";
				}
				else if(match(&b, "&apos;")){
					siteHeader = siteHeader + "'";
				}
				else if(match(&b, "&lt;")){
					siteHeader = siteHeader + "<";
				}
				else if(match(&b, "&gt;")){
					siteHeader = siteHeader + ">";
				}
				else{
                    char c = *b;
                    //Substitute one or more blank chars with a single space
                    if (c=='\n'||c=='\r'||c=='\t'||c==' ') {
                        siteHeader = siteHeader + " ";
                    }
                    else {
                        siteHeader = siteHeader + *b;
                    }
					b++;
				}
				break;
			}
			if(match(&b, ">")){
				startHeaderRecord = true;
				break;
			}
			b++;
			break;
		}
		default:;
		}
		
	}
}

void
SimpleHTMLParser::onContentFound(char c) {
}

void
SimpleHTMLParser::onAnchorFound(char * url) {
}


string
SimpleHTMLParser::getSiteDescription(){
	return siteDescription;
}

string 
SimpleHTMLParser::getSiteTitle(){
	if(!siteHeader.compare("")){
		return "No title specified for this page!";
	}
	return siteHeader;
}

vector<string>
SimpleHTMLParser::getSiteUrlList(){
	return siteUrlList;
}

string
SimpleHTMLParser::getSiteContent(){
	return siteContent;
}

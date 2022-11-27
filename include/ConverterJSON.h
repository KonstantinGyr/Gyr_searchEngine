#include<string>
#include <vector>


#ifndef SEARCHENGINE_CONVERTERJSON_H
#define SEARCHENGINE_CONVERTERJSON_H
#include "SearchServer.h"




class ConverterJSON {
    std::string mainPath;
    std::string name;
    std::string version;
    int maxResponses ;
public:
    ConverterJSON() = default;
    ConverterJSON(std::string _path):mainPath(_path) {} ;

    std::string nameSearchEngine();

    std::vector <std::string> GetTextDocuments();

    int GetResponsesLimit() ;

    std::vector <std::string> GetRequests();

    void putAnswers(std::vector<std::vector<RelativeIndex>>answers);

    void printAnswers();
};

#endif //SEARCHENGINE_CONVERTERJSON_H


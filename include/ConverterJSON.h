#ifndef SEARCHENGINE_CONVERTERJSON_H
#define SEARCHENGINE_CONVERTERJSON_H
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "SearchServer.h"
#include "nlohmann/json.hpp"

class ConverterJSON {
    const std::string mainPath;
    std::string name;
    std::string version;
    int maxResponses ;
public:
    ConverterJSON() = default;
    ConverterJSON(std::string _path):mainPath(_path) {} ;

    void updateConfig(int n);

    std::string nameSearchEngine() ;

    std::vector<std::string>GetTextDocuments()const;

    int GetResponsesLimit()const;

    std::vector<std::string> GetRequests()const;

    void putAnswers(const std::vector<std::vector<RelativeIndex>>&answers)const;

    void printAnswers()const;

    void SetRequest(std::vector<std::string> inVec);

    std::vector<std::string> getNameDocuments ()const;
};

#endif //SEARCHENGINE_CONVERTERJSON_H


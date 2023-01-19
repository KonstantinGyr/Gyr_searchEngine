#ifndef SEARCHENGINE_CONVERTERJSON_H
#define SEARCHENGINE_CONVERTERJSON_H
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include "SearchServer.h"
#include "nlohmann/json.hpp"

class ConverterJSON {
    const std::filesystem::path mainPath;
    std::string name;
    std::string version;
    int maxResponses;
public:
    ConverterJSON() = default;
    explicit ConverterJSON(std::filesystem::path _path):mainPath(std::move(_path)) {} ;

    std::string updateConfig() ;

    std::vector<std::string>GetTextDocuments()const;

    int GetResponsesLimit()const;

    std::vector<std::string> GetRequests()const;

    void putAnswers(const std::vector<std::vector<RelativeIndex>>&answers)const;

    void printAnswers()const;
};

#endif //SEARCHENGINE_CONVERTERJSON_H


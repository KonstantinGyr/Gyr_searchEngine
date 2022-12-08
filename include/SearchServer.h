#ifndef SEARCHENGINE_SEARCHSERVER_H
#define SEARCHENGINE_SEARCHSERVER_H

#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include "InvertedIndex.h"
//#include "ConverterJSON.h"

struct RelativeIndex{
    size_t doc_id;
    float rank;
    bool operator ==(const RelativeIndex& other) const {
        return (doc_id == other.doc_id && rank == other.rank);
    }
};

class SearchServer {
private:
    InvertedIndex _index;
    std::vector<std::vector<RelativeIndex>> vecRelativeIndex;
    std::vector<std::vector<std::string>>vecRequests;
public:
/**
* @param idx в конструктор класса передаётся ссылка на класс
InvertedIndex,
* чтобы SearchServer мог узнать частоту слов встречаемых в
запросе
*/
    SearchServer() = default;

    explicit SearchServer(InvertedIndex &idx) : _index(idx) {};

    size_t counter(const std::string& word);

    size_t countInDoc(const std::string& word, size_t docId);

    void processingRequest( std::vector<std::string>&request);

    auto creatVec(const std::vector<size_t> &listDocs,const std::vector<std::string> &request);

    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string> &queries_input,int resp);
};

#endif //SEARCHENGINE_SEARCHSERVER_H

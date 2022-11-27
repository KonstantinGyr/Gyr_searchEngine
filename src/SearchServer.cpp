#include <iostream>
#include <algorithm>
#include <filesystem>
#include <cassert>
#include "SearchServer.h"
#include "ConverterJSON.h"

//счетчик слова из запроса во всех документах
int SearchServer::counter(std::string word){
    int sum =0;
    for(auto &item:_index.GetWordCount(word)){
        sum += item.count;
    }
    return sum;
}

//счетчик слова из запроса в одном документе
int SearchServer::countInDoc( std::string word,size_t docId){
    for(auto &item:_index.GetWordCount(word)){
        if(item.doc_id == docId)return item.count;
    }
    return 0;
}
std::vector<std::string>parsingRequest(std::string str) {
    std::vector<std::string> vecOut;
    std::string word;
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == ' ') {
            vecOut.emplace_back(word);
            word.clear();
            continue;
        }
        word.push_back(str[i]);
        if (i == str.length() - 1) {
            vecOut.emplace_back(word);
        }
    }
    return vecOut;
}

void SearchServer::processingRequest(std::vector<std::string>&request){
    std::vector<std::string>uniqWords;//создаем вектор уникальных слов
    uniqWords = {request[0]};
    for (auto &word: request) {
        bool ok = true;
        for (auto &item: uniqWords) {
            if (word == item) ok = false;
        }
        if (ok)uniqWords.emplace_back(word);
    }
        //сортируем по возрастанию
    for (int i = 0; i < uniqWords.size(); i++) {
        int minInd = i;
        for (int j = i + 1; j < uniqWords.size(); j++) {
            if (counter(uniqWords[j]) == 0) {         //если документов не найдено удаляем слово из запроса
                std::cout << "On request : '" << uniqWords[j] << "' no documents found !" << std::endl;
                uniqWords.erase(uniqWords.begin() + j);
                j--;
                continue;
            }
            if (counter(uniqWords[j]) < counter(uniqWords[minInd])) {
                minInd = j;
            }
        }
        std::swap(uniqWords[minInd], uniqWords[i]);
    }
    request = uniqWords;
}

/**
* Метод обработки поисковых запросов
* @param queries_input поисковые запросы взятые из файла
requests.json
* @return возвращает отсортированный список релевантных ответов для
заданных запросов
*/
std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input){
    //парсим запросы
    for(auto &str:queries_input) {
        vecRequests.emplace_back(parsingRequest(str));
    }
    ConverterJSON conv("C:\\Users\\Kot\\CLionProjects\\FinalProject\\Gyr_searchEngine");
        for(auto &request:vecRequests) {
            processingRequest(request);//обработка запроса

            std::vector<size_t> listDocs;//список документов
            bool full = false;
            for(int i = 0;i<request.size();i++) {
                for (auto &item: _index.GetWordCount(request[i])) {
                    bool ok = false;
                    for(auto num:listDocs) {
                        if(item.doc_id == num)ok = true;
                    }
                    if(ok)break;
                    listDocs.push_back(item.doc_id);
                    if (listDocs.size() == conv.GetResponsesLimit()){
                        full = true;
                        break;
                    }
                }
                if(full)break;
            }

            //вектор документов с их абсолютной релевантностью
            std::vector<std::pair<size_t, int>> absRelDocs;

            for (auto &item: listDocs) {
                int countRel = 0;
                for (auto &word: request) {
                    countRel += countInDoc(word, item);
                }
                absRelDocs.emplace_back(item, countRel);
            }

            //сортируем вектор по убыванию
            for (int i = 0; i < absRelDocs.size(); ++i) {
                int maxInd = i;
                for (int j = i + 1; j < absRelDocs.size(); ++j) {
                    if (absRelDocs[j].second > absRelDocs[maxInd].second) {
                        maxInd = j;
                    }
                }
                std::swap(absRelDocs[i], absRelDocs[maxInd]);
            }

            //вектор документов с относительной релевантностью одного запроса
            std::vector<RelativeIndex> indOneReq;
            for (auto &item: absRelDocs) {
                RelativeIndex relativeIndex{};
                relativeIndex.doc_id = item.first;
                relativeIndex.rank = (float) item.second / (float) absRelDocs[0].second;
                indOneReq.emplace_back(relativeIndex);
            }
            vecRelativeIndex.emplace_back(indOneReq);
        }

    return vecRelativeIndex;
}

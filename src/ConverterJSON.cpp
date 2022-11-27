#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "SearchServer.h"
#include "nlohmann/json.hpp"
#include <filesystem>

/**
 *Метод проверяет configfile на валидность
 * @return название поискового движка и его версию
 */
std::string ConverterJSON::nameSearchEngine() {
    std::string filePath = mainPath + "\\" + "config.json";
    std::vector<std::string> files;
    nlohmann::json config;
    if (!std::filesystem::exists(filePath)) {
        throw std::invalid_argument("config file is missing");
    }
    std::ifstream file(filePath);
    file >> config;
    if (config["config"] == nullptr) {
        throw std::invalid_argument("config file is empty");
    }
    name = config["config"]["name"];
    version =  config["config"]["version"];
    maxResponses = config["config"]["max_responses"];
    for(auto &address:config["files"]){
        files.push_back(address);
    }
    return name + " V" += version;
}

/**
* Метод получения содержимого файлов
* @return Возвращает список с содержимым файлов перечисленных в config.json
*/
std::vector <std::string> ConverterJSON::GetTextDocuments(){
    std::vector<std::string>textDocuments;
    nlohmann::json config;
    std::ifstream file(mainPath + "\\" + "config.json");
    file >> config;
    for( auto &source:config["files"]){
        std::string fullText;
        std::string address = source;
        std::ifstream doc(mainPath + "\\" + address);
        if(!doc.is_open())std::cout<<source<<": is invalid address";
        doc.seekg(0);
        while (!doc.eof()){
            std::string partText;
            std::getline(doc,partText);
            fullText.append(partText);
        }
        textDocuments.push_back(fullText);
        doc.close();
    }
    return textDocuments;
}

/**
* Метод считывает поле max_responses для определения предельного
*/
int ConverterJSON::GetResponsesLimit() {
    nlohmann::json config;
    std::ifstream file(mainPath + "\\" + "config.json");
    file>>config;
    file.close();
    return (config["config"]["max_responses"]);
}

/**
* Метод получения запросов из файла requests.json
* @return возвращает список запросов из файла requests.json
*/
std::vector <std::string> ConverterJSON::GetRequests(){
    nlohmann::json requests;
    std::ifstream file (mainPath+"\\"+"requests.json");
    file>>requests;
    std::vector<std::string>output;
    std::string w;
    for(auto &words:requests["requests"]){
        output.push_back(words);
    }
    file.close();
    return output;
}

std::string numReq(size_t x){
    if(x>99)return std::to_string(x);
    if(x>9)return "0"+std::to_string(x);
    return "00" + std::to_string(x);
}

/**
* Положить в файл answers.json результаты поисковых запросов
*/
void ConverterJSON::putAnswers(std::vector <std::vector< RelativeIndex>>answers){
    std::ofstream ans(mainPath + "\\" + "answers.json");
    nlohmann::json mainJSON;
    nlohmann::json answersJSON;
    nlohmann::json reqJSON;
    nlohmann::json relevanceJSON;
    nlohmann::json pairJSON;
    std::vector< RelativeIndex>relevance;
    size_t count = 1;
    for(auto &answer:answers){
        if(answer.empty()){
            reqJSON["result"] = "false";
        }
        else if(answer.size() == 1){
            reqJSON["result"] = "true";
            reqJSON["docId"] = answer.front().doc_id;
            reqJSON["rank"] = answer.front().rank;
        }
        else {
            for(auto &xPair:answer){
                pairJSON["docId"] = xPair.doc_id;
                pairJSON["rank"] = xPair.rank;
                relevanceJSON += (pairJSON);
            }
            reqJSON["result"] = "true";
            reqJSON["relevance"] = relevanceJSON;
        }
        answersJSON["request " + numReq(count)] = reqJSON;
        reqJSON.clear();
        count++;
    }
    mainJSON["answers"] = answersJSON;
    ans<<mainJSON;
    ans.close();
}

void ConverterJSON::printAnswers(){
    std::ifstream file(mainPath + "\\" + "answers.json");
    nlohmann::json answers;
    file>>answers;
    size_t count = 1;
    for(auto &req:answers["answers"]){
        std::cout<<"request "<<numReq(count)<<std::endl;
        if(req.count("relevance")) {
            std::cout<<"  relevance : "<< std::endl;
            for (auto &ans: req["relevance"]){
                std::cout<<"      docId : "<<ans["docId"]<<", "<<"rank : "<<ans["rank"]<<", "<<std::endl;
            }
        }else{
            if(req["result"] == "true") {
                std::cout << "    result : " << req["result"] << std::endl;
                std::cout << "    docId : "<<req["docId"]<<", "<<"rank : "<<req["rank"]<<", "<<std::endl;
            }else{
                std::cout << "  result : " << req["result"] << std::endl;
            }
        }
        count++;
    }
}


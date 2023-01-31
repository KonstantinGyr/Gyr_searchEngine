#include "ConverterJSON.h"
using   path = std::filesystem::path;

/**
 *Метод проверяет configfile на валидность
 * @return название поискового движка и его версию
 */
std::string ConverterJSON::updateConfig() {
    path p = mainPath;
    std::vector<std::string> filesVec;
    nlohmann::json config;
    if (!std::filesystem::exists(p /= "config.json")) {
        throw std::invalid_argument("exception: non-existent config file path");
    }
    std::ifstream file(p );
    if(!file.is_open()){
        throw std::invalid_argument("exception: method 'ConverterJSON::nameSearchEngine' failed to open 'config.json' file ");
    }
    file >> config;
    if (config["config"] == nullptr || config["config"].empty()) {
        throw std::invalid_argument("exception: field \"config\" in 'config.json' is empty or does not exist");
    }
    if (config["config"]["name"] == nullptr || config["config"]["name"].empty()) {
        throw std::invalid_argument("exception: field \"name\" in 'config.json' is empty or does not exist");
    }
    if (config["config"]["version"] == nullptr || config["config"]["version"].empty()) {
        throw std::invalid_argument("exception: field \"version\" in 'config.json' is empty or does not exist");
    }
    if (config["config"]["max_responses"] == nullptr || config["config"]["max_responses"].empty()) {
        throw std::invalid_argument("exception: field \"max_responses\" in 'config.json' is empty or does not exist");
    }
    name = config["config"]["name"];
    version =  config["config"]["version"];
    maxResponses = config["config"]["max_responses"];
    if (config["files"] == nullptr || config["files"].empty()) {
        throw std::invalid_argument("exception: field \"files\" in 'config.json' is empty or does not exist");
    }
    for(const auto &address:config["files"]){
        filesVec.push_back(address);
    }
    file.close();
    return name + " V" += version;
}

/**
* Метод получения содержимого файлов
* @return Возвращает список с содержимым файлов перечисленных в config.json
*/
std::vector<std::string>  ConverterJSON::GetTextDocuments()const {
    std::vector<std::string>textDocuments;
    nlohmann::json config;
    path p = mainPath;
    std::ifstream file(p /= "config.json");
    if(!file.is_open()){
        throw std::invalid_argument("exception: method 'ConverterJSON::GetTextDocuments' failed to open 'config.json' file ");
    }
    file >> config;
    file.close();
    for(const auto &item:config["files"]){
        std::string fullText;
        std::string docName = item;
        p = mainPath;
        p /= "resources";
        std::ifstream doc(p /= docName);
        if(!doc.is_open()){
            throw std::invalid_argument("exception: method 'ConverterJSON::GetTextDocuments' failed to open 'resources' folder ");
        }
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
int ConverterJSON::GetResponsesLimit()const {
    path p = mainPath;
    nlohmann::json config;
    std::ifstream file(p /= "config.json");
    if(!file.is_open()){
        throw std::invalid_argument("exception: method 'ConverterJSON::GetResponsesLimit' failed to open 'config.json' file ");
    }
    file>>config;
    file.close();
    return (config["config"]["max_responses"]);
}
/**
* Метод получения запросов из файла requests.json
* @return возвращает список запросов из файла requests.json
*/
std::vector<std::string> ConverterJSON::GetRequests()const  {
    path p = mainPath;
    nlohmann::json requests;
    std::ifstream file (p /= "requests.json");
    if(!file.is_open()){
        throw std::invalid_argument("exception: method 'ConverterJSON::GetRequests' failed to open 'requests.json' file ");
    }
    file>>requests;
    if (requests["requests"] == nullptr || requests["requests"].empty()) {
        throw std::invalid_argument("exception: field \"requests\" in 'requests.json' is empty or does not exist");
    }
    file.close();
    std::vector<std::string>output;
    for(auto &words:requests["requests"]){
        output.push_back(words);
    }
    return output;
}

auto numReq(size_t x){
    if(x>99)return std::to_string(x);
    if(x>9)return "0"+std::to_string(x);
    return "00" + std::to_string(x);
}
/**
* Положить в файл answers.json результаты поисковых запросов
*/
void ConverterJSON::putAnswers(const std::vector <std::vector< RelativeIndex>>&answers)const {
    nlohmann::json mainJSON;
    nlohmann::json answersJSON;
    nlohmann::json reqJSON;
    nlohmann::json relevanceJSON;
    nlohmann::json pairJSON;
    std::vector< RelativeIndex>relevance;
    size_t count = 1;
    for(const auto &answer:answers){
        if(answer.empty()){
            reqJSON["result"] = false;
        }
        else if(answer.size() == 1){
            reqJSON["result"] = true;
            reqJSON["docId"] = (answer.front().doc_id);
            reqJSON["rank"] = answer.front().rank;
        }
        else {
            for(const auto &xPair:answer){
                pairJSON["docId"] = xPair.doc_id;
                pairJSON["rank"] = xPair.rank;
                relevanceJSON += (pairJSON);
            }
            reqJSON["result"] = true;
            reqJSON["relevance"] = relevanceJSON;
            relevanceJSON.clear();
        }
        answersJSON["request " + numReq(count)] = reqJSON;
        reqJSON.clear();
        count++;
    }
    mainJSON["answers"] = answersJSON;
    path p = mainPath;
    std::ofstream ans(p /= "answers.json");
    if(!ans.is_open()){
        throw std::invalid_argument("exception: method 'ConverterJSON::putAnswers' failed to open 'answers.json' file ");
    }
    ans << std::setw(4) << mainJSON << std::endl;
    ans.close();
 }

/**
 * Метод печатает файл answers.json
 */
void ConverterJSON::printAnswers()const {
    path p = mainPath;
    nlohmann::json answers;
    std::ifstream file(p /= "answers.json");
    if(!file.is_open()){
        throw std::invalid_argument("exception: method 'ConverterJSON::printAnswers' failed to open 'answers.json' file ");
    }
    file>>answers;
    file.close();
    std::cout<<answers.dump(4)<<std::endl;
}



#include "ConverterJSON.h"

/**
 * Метод обновляет config.json
 */
void ConverterJSON::updateConfig(int n){
    nlohmann::json confFile;
    maxResponses = n;
    confFile["config"]["name"] = "Gyr_searchEngine";
    confFile["config"]["version"] = "1.0";
    confFile["config"]["max_responses"] = maxResponses;
    for(auto &item:std::filesystem::directory_iterator(mainPath + "\\" + "resources")){
        if(item.is_regular_file()&&item.path().extension().compare(".txt") == 0) {
            confFile["files"].emplace_back(item.path().filename());
        }
    }
    std::ofstream file(mainPath +"\\" + "config.json");
    file<<confFile;
    file.close();
}
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
    for(const auto &address:config["files"]){
        files.push_back(address);
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
    std::ifstream file(mainPath + "\\" + "config.json");
    if(!file.is_open()){
        throw std::invalid_argument("method 'ConverterJSON::GetTextDocuments' failed to open 'config.json' file ");
    }
    file >> config;
    file.close();
    for(const auto &item:config["files"]){
        std::string fullText;
        std::string docName = item;
        std::ifstream doc(mainPath + "\\resources\\" + docName);
        if(!doc.is_open()){
            throw std::invalid_argument("method 'ConverterJSON::GetTextDocuments' failed to open 'resources' folder ");
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
 * @return возвращает имена документов
 */
std::vector<std::string> ConverterJSON::getNameDocuments()const {
    std::vector<std::string>nameVec;
    nlohmann::json config;
    std::ifstream file(mainPath + "\\" + "config.json");
    if(!file.is_open()){
        throw std::invalid_argument("method 'ConverterJSON::getNameDocuments' failed to open 'config.json' file ");
    }
    file >> config;
    file.close();
    for(const auto &item:config["files"]){
        nameVec.emplace_back(item);
    }
    return nameVec;
}
/**
* Метод считывает поле max_responses для определения предельного
*/
int ConverterJSON::GetResponsesLimit()const {
    nlohmann::json config;
    std::ifstream file(mainPath + "\\" + "config.json");
    if(!file.is_open()){
        throw std::invalid_argument("method 'ConverterJSON::GetResponsesLimit' failed to open 'config.json' file ");
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
    nlohmann::json requests;
    std::ifstream file (mainPath+"\\"+"requests.json");
    if(!file.is_open()){
        throw std::invalid_argument("method 'ConverterJSON::GetRequests' failed to open 'requests.json' file ");
    }
    file>>requests;
    file.close();
    std::vector<std::string>output;
    for(auto &words:requests["requests"]){
        output.push_back(words);
    }
    return output;
}
/**
 * Метод загрузки запросов в requests.json
 * @param inVec принимает вектор запросов из main
 */
void ConverterJSON::SetRequest(std::vector<std::string>inVec){
    nlohmann::json requests;
    for(auto &item:inVec){
        requests["requests"].emplace_back(item);
    }
    std::ofstream file(mainPath + "\\" + "requests.json");
    if(!file.is_open()){
        throw std::invalid_argument("method 'ConverterJSON::SetRequest' failed to open 'requests.json' file ");
    }
    file<<std::setw(4)<<requests<<std::endl;
    file.close();
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
    std::ofstream ans(mainPath + "\\" + "answers.json");
    if(!ans.is_open()){
        throw std::invalid_argument("method 'ConverterJSON::putAnswers' failed to open 'answers.json' file ");
    }
    ans << std::setw(4) << mainJSON << std::endl;
    ans.close();
 }

/**
 * Метод печатает файл answers.json
 */
void ConverterJSON::printAnswers()const {
    nlohmann::json answers;
    std::ifstream file(mainPath + "\\" + "answers.json");
    if(!file.is_open()){
        throw std::invalid_argument("method 'ConverterJSON::printAnswers' failed to open 'answers.json' file ");
    }
    file>>answers;
    file.close();
    std::cout<<answers.dump(4)<<std::endl;
}



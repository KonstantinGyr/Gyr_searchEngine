#include "ConverterJSON.h"

/**
 * счетчик слова из запроса во всех документах
 */
size_t SearchServer::counter(const std::string& word) {
    size_t sum =0;
    for(auto &item:_index.GetWordCount(word)){
        sum += item.count;
    }
    return sum;
}

/**
 * счетчик слова из запроса в одном документе
 */
size_t SearchServer::countInDoc(const std::string& word,size_t docId) {
    for(auto &item:_index.GetWordCount(word)){
        if(item.doc_id == docId)return item.count;
    }
    return 0;
}

std::vector<std::string>parsingRequest(std::string str){
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

/**
 * Функция обработки запроса
 * @param request
 */
void SearchServer::processingRequest( std::vector<std::string>&request){
    std::vector<std::string>uniqWords;//создаем вектор уникальных слов
    uniqWords = {request[0]};
    for (const auto &word: request) {
        bool ok = true;
        for (const auto &item: uniqWords) {
            if (word == item) ok = false;
        }
        if (ok)uniqWords.emplace_back(word);
    }
        //сортируем по возрастанию
    for (int i = 0; i < uniqWords.size(); i++) {
        int minInd = i;
        for (int j = i + 1; j < uniqWords.size(); j++) {
            if (counter(uniqWords[j]) == 0) {
                //если документов не найдено удаляем слово из запроса
                std::cout << "This word : " << uniqWords[j] << " no documents found !" << std::endl;
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

auto SearchServer::creatVec(const std::vector<size_t> &listDocs,const std::vector<std::string> &request){
    //вектор документов с их абсолютной релевантностью
    std::vector<std::pair<size_t, int>>absRelDocs;
    for (const auto &item: listDocs) {
        size_t countRel = 0;
        for (const auto &word: request) {
            countRel += countInDoc(word, item);
        }
        absRelDocs.emplace_back(item, countRel);
    }//сортируем по убыванию
   std::sort(absRelDocs.begin(),absRelDocs.end(),[](std::pair<size_t,int>p1,std::pair<size_t,int>p2){
      return p1.second > p2.second || (p1.second == p2.second && p1.first < p2.first);
   });
    std::vector<RelativeIndex> indOneReq;//вектор документов с индексами релевантности
    for (const auto &item: absRelDocs) {
        RelativeIndex relativeIndex{};
        relativeIndex.doc_id = item.first;
        relativeIndex.rank = (float) item.second / (float) absRelDocs[0].second;
        indOneReq.emplace_back(relativeIndex);
    }
   return  indOneReq ;
}

/**
* Метод обработки поисковых запросов
* @param queries_input поисковые запросы взятые из файла
requests.json
* @return возвращает отсортированный список релевантных ответов для
заданных запросов
*/
std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input,int resp){
    //парсим запросы
    for(auto &str:queries_input) {
        vecRequests.emplace_back(parsingRequest(str));//создаем вектор с запросами
    }
    ConverterJSON conv;
        for(auto &request:vecRequests)
        {       //(обрабатываем вектор запроса: оставляем уникальные слова и сортируем их по возрастанию)
            processingRequest(request);
            std::vector<size_t> listDocs;//список документов
            bool full = false;
            for(const auto & i : request)//проходим по запросу
            {
                for (const auto &item: _index.GetWordCount(i))
                {                                       //ищем в индексе слово из запроса
                    bool ok = false;
                    for(const auto &num:listDocs) {
                        if(item.doc_id == num)ok = true;
                    }
                    if(ok)break;
                    listDocs.push_back(item.doc_id);
                    if (listDocs.size() == resp){
                        full = true;
                        break;
                    }
                }
                if(full)break;
            }
            //вектор документов с относительной релевантностью одного запроса
            std::vector<RelativeIndex> indOneReq = creatVec(listDocs,request);
            vecRelativeIndex.emplace_back(indOneReq);
        }
    return vecRelativeIndex;
}

//#include <iostream>
//#include <algorithm>
//#include <cassert>
#include "ConverterJSON.h"

/**
 * ������� ����� �� ������� �� ���� ����������
 */
size_t SearchServer::counter(const std::string& word) {
    size_t sum =0;
    for(auto &item:_index.GetWordCount(word)){
        sum += item.count;
    }
    return sum;
}

/**
 * ������� ����� �� ������� � ����� ���������
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
 * ������� ��������� �������
 * @param request
 */
void SearchServer::processingRequest( std::vector<std::string>&request){
    std::vector<std::string>uniqWords;//������� ������ ���������� ����
    uniqWords = {request[0]};
    for (const auto &word: request) {
        bool ok = true;
        for (const auto &item: uniqWords) {
            if (word == item) ok = false;
        }
        if (ok)uniqWords.emplace_back(word);
    }
        //��������� �� �����������
    for (int i = 0; i < uniqWords.size(); i++) {
        int minInd = i;
        for (int j = i + 1; j < uniqWords.size(); j++) {
            if (counter(uniqWords[j]) == 0) {
                //���� ���������� �� ������� ������� ����� �� �������
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

auto SearchServer::creatVec(const std::vector<size_t> &listDocs,const std::vector<std::string> &request){
    //������ ���������� � �� ���������� ��������������
    std::vector<std::pair<size_t, int>>absRelDocs;
    for (const auto &item: listDocs) {
        size_t countRel = 0;
        for (const auto &word: request) {
            countRel += countInDoc(word, item);
        }
        absRelDocs.emplace_back(item, countRel);
    }
    for (int i = 0; i < absRelDocs.size(); ++i) {  //��������� ������ �� ��������
        int maxInd = i;
        for (int j = i + 1; j < absRelDocs.size(); ++j) {
            if (absRelDocs[j].second > absRelDocs[maxInd].second) {
                maxInd = j;
            }
        }
        std::swap(absRelDocs[i], absRelDocs[maxInd]);
    }
    std::vector<RelativeIndex> indOneReq;//������ ���������� � ��������� �������������
    for (const auto &item: absRelDocs) {
        RelativeIndex relativeIndex{};
        relativeIndex.doc_id = item.first;
        relativeIndex.rank = (float) item.second / (float) absRelDocs[0].second;
        indOneReq.emplace_back(relativeIndex);
    }
   return  indOneReq ;
}

/**
* ����� ��������� ��������� ��������
* @param queries_input ��������� ������� ������ �� �����
requests.json
* @return ���������� ��������������� ������ ����������� ������� ���
�������� ��������
*/
std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input,int resp){
    //������ �������
    for(auto &str:queries_input) {
        vecRequests.emplace_back(parsingRequest(str));//������� ������ � ���������
    }
    ConverterJSON conv;
        for(auto &request:vecRequests)
        {       //(������������ ������ �������: ��������� ���������� ����� � ��������� �� �� �����������)
            processingRequest(request);
            std::vector<size_t> listDocs;//������ ����������
            bool full = false;
            for(const auto & i : request)//�������� �� �������
            {
                for (const auto &item: _index.GetWordCount(i))
                {                                       //���� � ������� ����� �� �������
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
            //������ ���������� � ������������� �������������� ������ �������
            std::vector<RelativeIndex> indOneReq = creatVec(listDocs,request);
            vecRelativeIndex.emplace_back(indOneReq);
        }
    return vecRelativeIndex;
}

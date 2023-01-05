#include "InvertedIndex.h"
#include <sstream>
#include <iostream>

std::map<std::string, std::vector<Entry>> tempMap;// ��������� ��������� �������
std::mutex accessMap;

/**
 * ������� ������ �������� �� ������
 */
void parsingText(std::string text,int count){
   std::string line;
   std::stringstream stringStream(text);
   while(std::getline(stringStream,line)){
    //   InvertedIndex::setDict(line,count);
      std::size_t  pos,prev = 0;
       while ((pos = line.find_first_of(", .:-\";!?",prev)) != std::string::npos){
           if(pos > prev){
               std::string word = line.substr(prev,pos - prev);
               InvertedIndex::setDict(word,count);
               prev = pos + 1;
           }
           else if(pos == prev)prev = pos + 1;
       }
       if(prev < line.length()){
           InvertedIndex::setDict(line.substr(prev,std::string::npos),count);
       }
   }
}
/**
 * ����� ��������� �����, ����� ��������� � ��������� ��������� ��������� �������
 * @param word
 * @param numDoc
 */
void InvertedIndex::setDict(const std::string& word, int numDoc) {
    Entry entry{};
    std::vector<Entry> vecEntry;
    std::pair<std::string,std::vector<Entry>> xPair;
    accessMap.lock();
    if(tempMap.count(word)){
        bool ok = false;
        for(auto &item:tempMap.at(word)){
            if(item.doc_id == numDoc){
                item.count++;
                ok = true;
            }
        }
        if(!ok){
            entry.doc_id = numDoc;
            entry.count = 1;
            tempMap.at(word).emplace_back(entry);
        }
    } else{
        entry.doc_id = numDoc;
        entry.count = 1;
        vecEntry.push_back(entry);
        xPair.first = word;
        xPair.second = vecEntry;
        tempMap.insert(xPair);
    }
    accessMap.unlock();
}

/**
* �������� ��� ��������� ���� ����������, �� ������� ����� ��������� �����
* ����� ��������� ������ �������� � ��������� ������ � ����������� ���
* @param texts_input ���������� ����������
*/
void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs){
    docs = std::move(input_docs);
    std::vector<std::thread>threads;
    int countDoc = 0;
    for(const auto &text:docs) {
        threads.emplace_back( parsingText, text, countDoc);
        countDoc++;
    }
    for(auto &stream:threads){
        stream.join();
    }
  freq_dictionary = std::move(tempMap);//���������� ���������� ���������� �������
}
/**
 * ����� ���������� ���������� ��������� ����� word � ����������� ���� ����������
* @param word �����, ������� ��������� �������� ���������� ����������
* @return ���������� �������������� ������ � �������� ����
*/
std::vector<Entry> InvertedIndex::GetWordCount(const std::string &word)const{
    std::vector<Entry>vec;
    if(freq_dictionary.count(word))return freq_dictionary.at(word);
    return vec ;
}

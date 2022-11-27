#include <iostream>
#include <utility>
#include "InvertedIndex.h"

std::map<std::string, std::vector<Entry>> tempMap;// временный частотный словарь
std::mutex accessMap;

void  parsingText(std::string text,int count){
    InvertedIndex ind ;
    std::string word;
   /* for(auto item : text){
        if(item != ' ')word+=item;
        if(item == ' ' && !word.empty()){
            ind.setDict(word,count);
            word.clear();
        }
    }*/
    for(int i = 0;i<text.length();i++){
        if(text[i] == ' '){
            ind.setDict(word,count);
            word.clear();
            continue;
        }
        word.push_back(text[i]);
        if(i == text.length()-1){
            ind.setDict(word,count);
        }
    }
}
InvertedIndex::InvertedIndex() = default;

std::map<std::string, std::vector<Entry>> InvertedIndex::getDictionary() {
    return freq_dictionary;
}

void InvertedIndex::setDict(std::string word, int numDoc) {
    Entry entry{};
    std::vector<Entry> vecEntry;
    std::pair<std::string,std::vector<Entry>> xPair;
    accessMap.lock();
    if(tempMap.count(word)){
        if(tempMap.at(word).back().doc_id == numDoc){
            tempMap.at(word).back().count ++;
        }
        else{
            entry.doc_id = numDoc;
            entry.count = 1;
            tempMap.at(word).push_back(entry);
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
* Обновить или заполнить базу документов, по которой будем совершать
поиск
* @param texts_input содержимое документов
*/
void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
    docs = std::move(input_docs);
    int countDoc = 0;
    for(auto text:docs){
        std::thread streamText( parsingText, text,countDoc);
        streamText.join();
        countDoc++;
    }
    freq_dictionary = std::move(tempMap);
}

/**
 * Метод определяет количество вхождений слова word в загруженной базе документов
* @param word слово, частоту вхождений которого необходимо определить
* @return возвращает подготовленный список с частотой слов
*/
std::vector<Entry> InvertedIndex::GetWordCount(const std::string &word){
    std::vector<Entry>vec;
    if(freq_dictionary.count(word))return freq_dictionary.at(word);
    else return vec ;
}

void InvertedIndex::printDict (){
    accessMap.lock();
    for(auto &item:freq_dictionary){
        std::cout<<item.first<<" : ";
        for( auto &x:item.second){
            std::cout<<"docID : "<<x.doc_id<<" - "<<x.count<<" ; ";
        }
        std::cout<<std::endl;
    }
    accessMap.unlock();
}
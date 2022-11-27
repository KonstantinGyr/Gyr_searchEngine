#include <map>
#include <string>
#include <vector>
#include <mutex>
#include <thread>

#ifndef SEARCHENGINE_INVERTEDINDEX_H
#define SEARCHENGINE_INVERTEDINDEX_H

struct Entry {
    size_t doc_id, count;
    // Данный оператор необходим для проведения тестовых сценариев
    bool operator ==(const Entry& other) const {
        return (doc_id == other.doc_id &&
                count == other.count);
    }
};

class InvertedIndex {
private:
    // список содержимого документов
    std::map<std::string, std::vector<Entry>> freq_dictionary;
    std::vector<std::string> docs;

public:
    InvertedIndex() ;

    std::map<std::string, std::vector<Entry>> getDictionary();

    void setDict(std::string word ,int numDoc);

    void UpdateDocumentBase(std::vector<std::string> input_docs);

    std::vector<Entry> GetWordCount(const std::string& word);

    void printDict ();
};



#endif //SEARCHENGINE_INVERTEDINDEX_H

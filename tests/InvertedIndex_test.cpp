#include <filesystem>
#include "InvertedIndex.h"
#include "ConverterJSON.h"
#include "gtest/gtest.h"

void sort(std::vector<Entry>&inVec){
    for (int i = 0; i < inVec.size() - 1; i++) {
        int min = i;
        for (int j = i + 1; j < inVec.size(); j++) {
            if (inVec[j].doc_id < inVec[min].doc_id)min = j;
        }
        std::swap(inVec[i], inVec[min]);
    }
}

std::string pt (){
    std::string p = std::filesystem::current_path().string();
    while (true){
        if( p.back() != '\\'){
            p.pop_back();
        }
        else{
            p.pop_back();
            break;
        }
    }
    return p;
}

InvertedIndex ind;
ConverterJSON conv(pt());

/*TEST(InvertedIndex, updateDocumentBase) {
    const std::vector<Entry>expected =
    {
            {0,2},
            {1,1},
            {2,5},
            {3,3},
            {4,3}
    };
    ind.UpdateDocumentBase(conv.GetTextDocuments());
    std::vector<Entry> result = ind.GetWordCount("the");
    if(result.size()>1)sort(result);
    for(auto &ent:result){
        std::cout<<"docID : "<<ent.doc_id<<" - quantity : "<<ent.count<<std::endl;
    }
    ASSERT_EQ(expected,result);
}*/


TEST(InvertedIndex, updateDocumentBase2) {
    const std::vector<Entry>expected = {
            {0,4},
            {1,1},
            {2,5}
    };
    const std::vector<std::string> docs = {
                    "milk milk milk milk water water water",
                    "milk water water",
                    "milk milk milk milk milk water water water water water",
                    "americano cappuccino"
    };
    ind.UpdateDocumentBase(docs);
    std::vector<Entry> result = ind.GetWordCount("milk");
    if(result.size()>1)sort(result);
    for(const auto &ent:result){
        std::cout<<"docID : "<<ent.doc_id<<" - quantity : "<<ent.count<<std::endl;
    }
    ASSERT_EQ(expected,result);
}



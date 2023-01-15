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

std::filesystem::path testPt(std::filesystem::current_path().parent_path()) ;

InvertedIndex ind;
ConverterJSON conv(testPt);

TEST(InvertedIndex, updateDocumentBase)
{
    const std::vector<Entry>expected =
    {
            {0,4},
            {1,2},
            {2,1},
            {3,2}
    };
    ind.UpdateDocumentBase(conv.GetTextDocuments());
    std::vector<Entry> result = ind.GetWordCount(conv.GetRequests()[1]);
    if(result.size()>1)sort(result);
    for(auto &ent:result){
        std::cout<<"docID : "<<ent.doc_id<<" - quantity : "<<ent.count<<std::endl;
    }
    ASSERT_EQ(expected,result);
}


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



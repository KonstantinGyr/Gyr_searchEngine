#include <filesystem>
#include "InvertedIndex.h"
#include "ConverterJSON.h"
#include "gtest/gtest.h"

InvertedIndex ind;
ConverterJSON conv(std::filesystem::current_path().parent_path());

TEST(InvertedIndex, updateDocumentBase) {
    const std::vector<Entry> expected =
            {
                    {0, 4},
                    {1, 2},
                    {2, 1},
                    {3, 2}
            };
    ind.UpdateDocumentBase(conv.GetTextDocuments());
    std::vector<Entry> result = ind.GetWordCount(conv.GetRequests()[1]);
    if (result.size() > 1) {
        std::sort(result.begin(), result.end(), [](Entry e1, Entry e2) {
            return e1.doc_id < e2.doc_id;
        });
        for (auto &ent: result) {
            std::cout << "docID : " << ent.doc_id << " - quantity : " << ent.count << std::endl;
        }
        ASSERT_EQ(expected, result);
    }
}


TEST(InvertedIndex, updateDocumentBase2) {
        const std::vector<Entry> expected = {
                {0, 4},
                {1, 1},
                {2, 5}
        };
        const std::vector<std::string> docs = {
                "milk milk milk milk water water water",
                "milk water water",
                "milk milk milk milk milk water water water water water",
                "americano cappuccino"
        };
        ind.UpdateDocumentBase(docs);
        std::vector<Entry> result = ind.GetWordCount("milk");
        if (result.size() > 1) {
            std::sort(result.begin(), result.end(), [](Entry e1, Entry e2) {
                return e1.doc_id < e2.doc_id;
            });
            for (const auto &ent: result) {
                std::cout << "docID : " << ent.doc_id << " - quantity : " << ent.count << std::endl;
            }
            ASSERT_EQ(expected, result);
        }
    }



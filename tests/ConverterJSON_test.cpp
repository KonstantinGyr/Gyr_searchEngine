#include "ConverterJSON.h"
#include "gtest/gtest.h"

ConverterJSON converter("C:\\Users\\Kot\\CLionProjects\\FinalProject\\Gyr_searchEngine");

TEST(ConverterJSON, readConfig) {
    std::string str = "Gyr_searchEngine V1.0";
    ASSERT_EQ(converter.nameSearchEngine(), str);
}

TEST(ConverterJSON, TestPrintAnswers) {
    converter.printAnswers();
    ASSERT_EQ(0,0);
}

TEST(ConverterJSON, getResponsesLimit) {

    ASSERT_EQ(converter.GetResponsesLimit(), 5);
}

TEST(ConverterJSON, getRequests) {
    std::vector<std::string> expected =
            {
                "Bjarne Stroustrup",
                "function name",
                "language",
                "C++"
            };
    ASSERT_EQ(converter.GetRequests(), expected);
}

TEST(ConverterJSON, getFiles) {
    int count = 1;
    for (const auto &item: converter.GetTextDocuments()) {
        std::cout <<"Text ¹ "<<count<<" : "<< item << std::endl;
        ++count;
    }
    ASSERT_EQ(0,0);
}

TEST(ConverterJSON, putAnswers) {
    const std::vector<std::vector<RelativeIndex>> expected = {
            {
                    {2, 1},
                    {0, 0.7},
                    {1, 0.3}
            },
            {
                    {0,0.754}
            },
            {
            }
    };
    converter.putAnswers(expected);
    ASSERT_EQ(0,0);
}

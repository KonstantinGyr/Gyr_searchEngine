#include <string>
#include <filesystem>
#include <fstream>
#include "ConverterJSON.h"
#include "gtest/gtest.h"

std::string p (){
    std::string path = std::filesystem::current_path().string();
    while (true){
        if( path.back() != '\\'){
            path.pop_back();
        }
        else{
            path.pop_back();
            break;
        }
    }
    return path;
}

ConverterJSON converter(p());

TEST(ConverterJSON, readConfig) {
    std::string str = "Gyr_searchEngine V1.0";
    ASSERT_EQ(converter.nameSearchEngine(), str);
}

TEST(ConverterJSON, getResponsesLimit) {
    converter.updateConfig(5);
    int expected = 5;
    int result = converter.GetResponsesLimit();
    ASSERT_EQ(result, expected);
}

TEST(ConverterJSON, getRequests) {
    std::vector<std::string> expected =
            {
                "Bjarne Stroustrup",
                "function name",
                "language",
                "C++"
            };
    converter.SetRequest(expected);
    std::vector<std::string>result = converter.GetRequests();
    ASSERT_EQ(result , expected);
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
    converter.printAnswers();
    ASSERT_EQ(0,0);
}


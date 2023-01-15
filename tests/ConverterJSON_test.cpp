#include <string>
#include <filesystem>
#include "ConverterJSON.h"
#include "gtest/gtest.h"

std::filesystem::path testPath(std::filesystem::current_path().parent_path()) ;

ConverterJSON converter(testPath);

TEST(ConverterJSON, readConfig) {
    std::string str = "Gyr_searchEngine V1.0";
    ASSERT_EQ(converter.nameSearchEngine(), str);
}

TEST(ConverterJSON, getResponsesLimit) {
    int expected = 5;
    int result = converter.GetResponsesLimit();
    ASSERT_EQ(result, expected);
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


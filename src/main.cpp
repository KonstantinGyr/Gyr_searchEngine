#include <iostream>
#include <string>
#include <filesystem>
#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"



auto numReq(int x){
    if(x>99)return std::to_string(x);
    if(x>9)return "0"+std::to_string(x);
    return "00" + std::to_string(x);
}

int main(int argc,char *argv[]) {
    setlocale(LC_ALL, "rus");
    std::filesystem::path p(argv[0]);
    ConverterJSON converterJson(p.parent_path());
    try {
        std::cout << converterJson.nameSearchEngine() << std::endl;
    }
    catch (const std::invalid_argument &x) {
        std::cerr << x.what()<<std::endl;
    }
    InvertedIndex invertedIndex;
    try {
        invertedIndex.UpdateDocumentBase(converterJson.GetTextDocuments());
    }
    catch (const std::invalid_argument &x) {
        std::cerr << x.what() << std::endl;
    }
    SearchServer searchServer(invertedIndex);
    try {
        converterJson.putAnswers(searchServer.search(converterJson.GetRequests(),
                                                     converterJson.GetResponsesLimit()));
    }
    catch (const std::invalid_argument &x) {
        std::cerr << x.what() << std::endl;
    }
    try {
        converterJson.printAnswers();
    }catch (const std::invalid_argument &x) {
        std::cerr << x.what() << std::endl;
    }
    return 0;
}

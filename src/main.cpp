#include <iostream>
#include <vector>
#include <string>
#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"
#include <filesystem>



int main(int argc,char *argv[]) {
    setlocale(LC_ALL,"rus");
    std::filesystem::path p(argv[0]);
    ConverterJSON converterJson(p.parent_path().string());
    try {
        std::cout << converterJson.nameSearchEngine() << std::endl;
    }
    catch (const std::invalid_argument &x) {
        std::cerr << x.what();
    }
    int a = 1;
    for(auto &request:converterJson.GetRequests()){

        std::cout<<"Request 00"<< a <<" : " <<request<<std::endl;
        ++a;
    }
    std::cout<<std::endl;
    InvertedIndex invertedIndex;
    invertedIndex.UpdateDocumentBase(converterJson.GetTextDocuments());
    SearchServer searchServer(invertedIndex);
    converterJson.putAnswers(searchServer.search(converterJson.GetRequests()));
    converterJson.printAnswers();
    return 0;
}

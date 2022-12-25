#include 3<iostream>
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
    std::filesystem::path p(argv[0]);
    ConverterJSON converterJson(p.parent_path().string());
    try {
        std::cout << converterJson.nameSearchEngine() << std::endl;
    }
    catch (const std::invalid_argument &x) {
        std::cerr << x.what()<<std::endl;
    }
    std::cout<<"Download txt documents in folder : "<< p.parent_path().string() + "\\resources" << std::endl;
    std::cout<<"How many responses per query :";
    int resp ;
    std::cin>>resp;
    std::cin.ignore();
    try {
        converterJson.updateConfig(resp);
    }
    catch (const std::invalid_argument &x) {
        std::cerr << x.what() << std::endl;
    }
    std::cout<<"Documents uploaded to the database :"<<std::endl;
    for(int i=0;i<converterJson.getNameDocuments().size();++i){
        try{
           std::cout<<"   docId "<<numReq(i)<<" : "<<converterJson.getNameDocuments()[i]<<std::endl;
        }
        catch (const std::invalid_argument &x) {
            std::cerr << x.what()<<std::endl;
        }
    }
    std::cout<<"How many requests will you have :" ;
    int num;
    std::cin>>num;
    std::cin.ignore();
    std::cout<<"Enter your queries separated by a space"<<std::endl;
    std::vector<std::string>inRequests;
    for(int i=0;i<num;++i){
        std::cout<<"  Request "<< numReq(i+1) <<" :";
        std::string req;
        std::getline(std::cin,req);
        inRequests.emplace_back(req);
    }
    try {
        converterJson.SetRequest(inRequests);
    }
    catch (const std::invalid_argument &x) {
        std::cerr << x.what() << std::endl;
    }
    std::cout<<std::endl;
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

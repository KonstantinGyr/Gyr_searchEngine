#include <iostream>
#include <string>
#include <filesystem>
#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"

int main(int argc,char *argv[]) {
    try {
        std::filesystem::path p(argv[0]);
        ConverterJSON converterJson(p.parent_path());
        try {
            std::cout << converterJson.updateConfig() << std::endl;
        }
        catch (const std::invalid_argument &x) {
            std::cerr << x.what() << std::endl;
            return 0;
        }
        InvertedIndex invertedIndex;
        try {
            invertedIndex.UpdateDocumentBase(converterJson.GetTextDocuments());
        }
        catch (const std::invalid_argument &x) {
            std::cerr << x.what() << std::endl;
            return 0;
        }
        SearchServer searchServer(invertedIndex);
        try {
            converterJson.putAnswers(searchServer.search(converterJson.GetRequests(),
                                                         converterJson.GetResponsesLimit()));
        }
        catch (const std::invalid_argument &x) {
            std::cerr << x.what() << std::endl;
            return 0;
        }
        std:: cout <<"The search is over, the results are written to the file \"answers.json\".";
    }
    catch(std::exception &exc){
        std::cerr << "Standard exception: " << exc.what() ;
    }
    return 0;
}

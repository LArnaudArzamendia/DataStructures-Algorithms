#include "poscode.hpp"
#include "utils.hpp"
#include <string>
#include <iostream>

int main(int nargs, char** vargs){
    std::string strfile("../codes_500K.txt");
    size_t n = 500000;
    Poscode *data = readCodes(strfile, n);
    for(size_t i = 0; i <  10; i++){        
        std::cout<<data[i].getData()<<std::endl;
    }

    return 0;
}
#include<stdio.h>

#include "node.hpp"
#include "driver.hpp"
#include "parser.hpp"
#include "scanner.hpp"

Driver::Driver(char* f) : file_name_(f) {
}

void Driver::parse() {
    fileRead();
    yy::parser parse(*this);
    // parse.set_debug_level(true);
    if (parse()) {
        fprintf(stderr,"parser erorr\n");
        exit(1);
    }
    fileClose();
}

void Driver::fileRead() {
    FILE *fp = fopen(file_name_, "r");
    
    if(fp == NULL) {
        perror("ファイルが開けませんでした");
        exit(EXIT_FAILURE);
    }
    yyin = fp;
}

void Driver::fileClose() {

}
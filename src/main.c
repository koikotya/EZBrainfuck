#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
// #include "generator.h"

extern FILE *yyin;
extern Node *parse_result;
extern int yyparse(void);
extern int list_size;
extern void dfs1(Node *p);
extern void dfs2(Node *p);
FILE *Fresult;


int file_read(char *fname) {
    FILE *fp = fopen(fname, "r");
    
    if(fp == NULL) {
        perror("ファイルが開けませんでした");
        exit(EXIT_FAILURE);
    }
    yyin = fp;
    return 0;
}

int main(int argc, char *argv[]) {
    file_read(argv[1]);
    if (argc < 3) Fresult = fopen("a.bf","w");
    else Fresult = fopen(argv[2],"w");

    if (yyparse()) {
        fprintf(stderr, "parser erorr\n");
        exit(1);
    }
    // printf("syntax ok\n");
    // print_node(parse_result,0);
    dfs1(parse_result);
    list_size = 0;
    dfs2(parse_result);
    fclose(Fresult);
    fprintf(stderr,"Successfully generated code\n");
}
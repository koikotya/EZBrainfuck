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

#ifdef __EMSCRIPTEN__
// emcc main.c ast.c bf.c ezbf.tab.c generator.c lex.yy.c -s EXPORTED_RUNTIME_METHODS=['ccall'] -s EXPORT_ES6=1 -s MODULARIZE=1 -s ENVIRONMENT=web -o ezbf.js
#include <emscripten\emscripten.h>

char result[1<<20] = "";
typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern YY_BUFFER_STATE yy_scan_string(char * str);

char err[1000] = "";

EMSCRIPTEN_KEEPALIVE char* gen_code(char *s) {
    yy_scan_string(s);

    if (yyparse()) {
        print_err("parser erorr\n");
        exit(1);
    }
    // printf("syntax ok\n");
    // print_node(parse_result,0);
    dfs1(parse_result);
    list_size = 0;
    dfs2(parse_result);
    print_err("Successfully generated code\n");
    return result;
}

EMSCRIPTEN_KEEPALIVE char* get_err() {
    return err;
}

int main() {
    return 0;
}
#else
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
        print_err("parser erorr\n");
        exit(1);
    }
    // printf("syntax ok\n");
    // print_node(parse_result,0);
    dfs1(parse_result);
    list_size = 0;
    dfs2(parse_result);
    fclose(Fresult);
    print_err("Successfully generated code\n");
}
#endif
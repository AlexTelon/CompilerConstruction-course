#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "ast.hh"
#include "parser.hh"

using namespace std;

extern int error_count;
extern bool yydebug;
bool assembler_trace = false;
bool print_egen = false;
bool print_ast = false;
bool print_quads = false;
bool typecheck = true;
bool optimize = true;
bool quads = true;
bool assembler = true;

void usage(char *program_name)
{
    cerr << "Usage:\n"
         << program_name << " [-acdfpqstyz] inputfile\n"
         << program_name << " [-h?]\n"
         << "Options:\n"
         << "  -h, -?            Shows this message.\n"
         << "  -a                Print AST (abstract syntax tree).\n"
         << "  -c                Disable type checking.\n"
         << "  -d                Turn on parser debugging.\n"
         << "  -f                Don't optimize.\n"
         << "  -p                Don't generate quads.\n"
         << "  -q                Print quad lists.\n"
         << "  -s                Don't generate assembler code.\n"
         << "  -t                Include trace printouts in assembler code.\n"
         << "  -y                Print symbol table.\n"
         << "  -z                egen debug grej.\n";
    exit(1);
}


int main(int argc, char **argv)
{
    char options[] = "acdfpqstyhz?";
    int option;
    bool print_symtab = false;

    extern  FILE *yyin;

    opterr = 0;
    optopt = '?';

    // Check for options.
    while ((option = getopt(argc, argv, options)) != EOF) {
        switch (option) {
        case 'a':
            cout << "An AST will be printed for each block.\n" << flush;
            print_ast = true;
            break;
        case 'c':
            cout << "No type checking will be performed.\n" << flush;
            typecheck = false;
            break;
        case 'd':
            cout << "Bison debugging turned on.\n" << flush;
            yydebug = true;
            break;
        case 'f':
            cout << "No optimization will be done.\n" << flush;
            optimize = false;
            break;
        case 'p':
            cout << "No quads will be generated.\n" << flush;
            quads = false;
            break;
        case 'q':
            cout << "A quad list will be printed for each block.\n"
                 << flush;
            print_quads = true;
            break;
        case 's':
            cout << "No assembler code will be generated.\n" << flush;
            assembler = false;
            break;
        case 't':
            cout << "Assembler code will contain quad labels.\n" << flush;
            assembler_trace = true;
            break;
        case 'y':
            cout << "Symbol table will be printed after compilation.\n";
            print_symtab = true;
            break;
        case 'z':
            cout << "EGEN IS ACTIVE\n";
            print_egen = true;
            break;
        case 'h':
        case '?':
            usage(argv[0]);
            break;
        default:
            break;
        }
    }

    if (optind > argc || optind < argc - 1) {
        usage(argv[0]);
    } else if (optind == argc) {
        yyin = stdin;
    } else {
        yyin = fopen(argv[optind], "r");
        if (yyin == NULL) {
            perror(argv[optind]);
            exit(1);
        }
    }

    // Start the compilation. This is where all the magic is done.
    // This function resides in parser.cc, which is generated by bison from
    // parser.y.
    yyparse();

    // If given the appropriate flag, prints the symbol table after the input
    // has been parsed.
    if (print_symtab) {
        sym_tab->print(2);
        sym_tab->print(1);
    }

    exit(error_count);
}




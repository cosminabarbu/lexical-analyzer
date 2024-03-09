#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Token types
typedef enum {
    KEYWORD,
    IDENTIFIER,
    CONSTANT,
    STRING,
    SYMBOL
} TokenType;

// Structure to represent a token
typedef struct {
    char lexeme[50];
    TokenType type;
} Token;

// Function to check if a given string is a keyword
int isKeyword(char *str) {
    char keywords[][10] = {
            "int", "char", "return", "printf", "scanf" ,"include", "if",
            "while","else", "float", "do", "break", "double", "unsigned",
            "long","short", "static", "struct", "void", "switch", "case" ,
            "argv", "argc" , "strlen" , "strcpy", "strcat" , "strcmp" ,"strlwr",
            "sizeof", "enum" , "auto" , "default" , "typedef", "for" ,
            "fclose", "perror" , "fopen" , "fgetc"
    };

    for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1; // Keyword
        }
    }
    return 0; // Not a keyword
}

// Function to check if a given character is a symbol
int isSymbol(char ch) {
    char symbols[] = {'(', ')', '{', '}', '[', ']', '=', '"', ';', ',',
                        '#', '<', '>', '.', '*', '%', '&', '+', '-'};

    for (int i = 0; i < sizeof(symbols) / sizeof(symbols[0]); i++) {
        if (ch == symbols[i]) {
            return 1; // Symbol
        }
    }
    return 0; // Not a symbol
}

// Function to analyze the given input and print tokens
void gettoken(char *input, FILE *outputFile) {
    Token token;
    int i = 0;
    int inputLength = strlen(input);

    fprintf(outputFile, "%-20s%-20s%-20s\n", "token", "type", "value");
    fprintf(outputFile, "--------------------|--------------------|--------------------\n");

    while (i < inputLength - 1) {
        // Skip whitespaces and comments
        while (input[i] == ' ' || input[i] == '\t' || input[i] == '\n' || input[i] == '/' || input[i] == '*') {
            // Handle single-line comments
            if (input[i] == '/' && input[i + 1] == '/') {
                while (input[i] != '\n' && input[i] != '\0') {
                    i++;
                }
            }
                // Handle multi-line comments
            else if (input[i] == '/' && input[i + 1] == '*') {
                i += 2;
                while (!(input[i] == '*' && input[i + 1] == '/')) {
                    if (input[i] == '\0') {
                        fprintf(stderr, "Error: Unclosed multi-line comment.\n");
                        return;
                    }
                    i++;
                }
                i += 2;
            } else {
                i++;
            }
        }

        // Initialize token
        token.lexeme[0] = '\0';

        // Check for symbols
        if (isSymbol(input[i])) {
            token.lexeme[0] = input[i++];
            token.lexeme[1] = '\0';
            token.type = SYMBOL;
            fprintf(outputFile, "%-20s%-20s%-20s\n", token.lexeme, "Symbol", token.lexeme);
        }
            // Check for keywords or identifiers
        else if (isalpha(input[i])) {
            int j = 0;
            while (isalpha(input[i]) || isdigit(input[i])) {
                token.lexeme[j++] = input[i++];
            }
            token.lexeme[j] = '\0';
            if (isKeyword(token.lexeme)) {
                token.type = KEYWORD;
                fprintf(outputFile, "%-20s%-20s%-20s\n", token.lexeme, "Keyword", token.lexeme);
            } else {
                token.type = IDENTIFIER;
                fprintf(outputFile, "%-20s%-20s%-20s\n", token.lexeme, "Identifier", token.lexeme);
            }
        }
            // Check for constants (assuming integers for simplicity)
        else if (isdigit(input[i])) {
            int j = 0;
            while (isdigit(input[i])) {
                token.lexeme[j++] = input[i++];
            }
            token.lexeme[j] = '\0';
            token.type = CONSTANT;
            fprintf(outputFile, "%-20s%-20s%-20s\n", token.lexeme, "Constant", token.lexeme);
        }
            // Check for strings
        else if (input[i] == '"') {
            int j = 0;
            token.lexeme[j++] = input[i++];
            while (input[i] != '"' && input[i] != '\0') {
                token.lexeme[j++] = input[i++];
            }
            if (input[i] == '"') {
                token.lexeme[j++] = input[i++];
            }
            token.lexeme[j] = '\0';
            token.type = STRING;
            fprintf(outputFile, "%-20s%-20s%-20s\n", token.lexeme, "String", token.lexeme);
        } else {
            i++;
            continue;
        }
    }
}

int main() {
    // Open the input file for reading
    FILE *inputFile = fopen("input.txt", "r");
    if (inputFile == NULL) {
        perror("Error opening input file");
        return 1;
    }

    // Open the output file for writing
    FILE *outputFile = fopen("output.txt", "w");
    if (outputFile == NULL) {
        perror("Error opening output file");
        fclose(inputFile);
        return 1;
    }

    // Read the content of the input file character by character
    int c;
    char inputBuffer[1000];  // Assuming a maximum line length of 1000 characters
    int i = 0;

    while ((c = fgetc(inputFile)) != EOF && i < sizeof(inputBuffer) - 1) {
        inputBuffer[i++] = (char)c;
    }
    inputBuffer[i] = '\0';

    // Call the gettoken function with the input buffer
    gettoken(inputBuffer, outputFile);

    // Close files
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
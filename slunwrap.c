#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Tokens are strings up to (TOKEN_BUFFER_SZ - 1) characters in length. */
#define TOKEN_BUFFER_SZ 1024

/* Up to (TOKEN_STACK_SZ - 1) tokens can be used simultaneously. */
#define TOKEN_STACK_SZ 4

typedef struct token_t {
    char buffer[TOKEN_BUFFER_SZ];
    int pos;
} token_t;

typedef enum parser_state_t {
    PS_NODE_NAME_IFS = 0,
    PS_NODE_NAME = 1,
    PS_NODE_NUM = 2,
    PS_NODE_NUM_RANGE = 3
} parser_state_t;

typedef struct parser_t {
    parser_state_t state;
} parser_t;

void token_init(token_t * token)
{
    token->buffer[0] = '\0';
    token->pos = 0;
}

int token_append(token_t * token, char c)
{
    if (token->pos + 1 < TOKEN_BUFFER_SZ) {
        token->buffer[token->pos] = c;
        token->pos++;
        token->buffer[token->pos] = '\0';
        return 0;
    } else {
        return 1;
    }
}

void token_stack_unwind(token_t * tokens, int * t, parser_t * parser)
{
    int i = 0;
    int unwind = 0;
    /* Print tokens interpreted by count. */
    if (1 == *t) {
        printf("%s%03d\n", tokens[0].buffer, atoi(tokens[1].buffer));
    } else if (2 == *t) {
        int lower = atoi(tokens[1].buffer);
        int upper = atoi(tokens[2].buffer);
        int i = 0;
        for (i = lower; i <= upper; ++i) {
            printf("%s%03d\n", tokens[0].buffer, i);
        }
    } else {
        fprintf(stderr, "Warning: ignoring token_stack_unwind, "
                        "bad number of tokens: %d\n", *t);
    }
    /* Reset tokens. */
    if (PS_NODE_NUM == parser->state) {
        unwind = 1;
    }
    for (i = unwind; i <= *t; ++i) {
        token_init(tokens + i);
    }
    *t = unwind;
}

void parser_init(parser_t * parser)
{
    parser->state = PS_NODE_NAME;
}

#define parser_error(parser, c, message) ( \
    fprintf(stderr, \
            "Parser error (state=%d) (c=%c): %s\n", \
            ((parser_t *) parser)->state, \
            c, \
            message) \
)

int main(int argc, char * argv[])
{
    token_t tokens[TOKEN_STACK_SZ];
    token_t * token = NULL;
    parser_t parser;
    int t = 0;
    int c = EOF;
    int i = 0;
    /* Initialize tokens. */
    for (i = 0; i < TOKEN_STACK_SZ; ++i) {
        token_init(tokens + i);
    }
    /* Initialize parser. */
    parser_init(&parser);
    /* Scan tokens and execute parser. */
    while (EOF != (c = getc(stdin))) {
        token = tokens + t;
        switch (c) {
            case '[':
                if (PS_NODE_NAME == parser.state) {
                    if (t + 1 < TOKEN_STACK_SZ) {
                        parser.state = PS_NODE_NUM;
                        t++;
                    } else {
                        parser_error(&parser, c, "Token stack overflowed");
                        return 1;
                    }
                } else {
                    parser_error(&parser, c, "Unexpected grouping");
                    return 1;
                }
                break;

            case ']':
                if (PS_NODE_NUM == parser.state ||
                        PS_NODE_NUM_RANGE == parser.state) {
                    parser.state = PS_NODE_NAME_IFS;
                    token_stack_unwind(tokens, &t, &parser);
                } else {
                    parser_error(&parser, c, "Unexpected delimiter");
                    return 1;
                }
                break;

            case '-':
                if (PS_NODE_NUM == parser.state) {
                    if (t + 1 < TOKEN_STACK_SZ) {
                        parser.state = PS_NODE_NUM_RANGE;
                        t++;
                    } else {
                        parser_error(&parser, c, "Token stack overflowed");
                        return 1;
                    }
                } else {
                    parser_error(&parser, c, "Unexpected range operator");
                    return 1;
                }
                break;

            case ',':
                if (PS_NODE_NUM == parser.state ||
                        PS_NODE_NUM_RANGE == parser.state) {
                    parser.state = PS_NODE_NUM;
                    token_stack_unwind(tokens, &t, &parser);
                } else if (PS_NODE_NAME_IFS == parser.state) {
                    parser.state = PS_NODE_NAME;
                }else {
                    parser_error(&parser, c, "Unexpected delimiter");
                    return 1;
                }
                break;

            case ' ':
            case '\t':
                /* Silently ignore. */
                break;

            default:
                if (PS_NODE_NAME == parser.state) {
                    token_append(token, c);
                } else if (PS_NODE_NUM == parser.state ||
                           PS_NODE_NUM_RANGE == parser.state) {
                    if (c >= '0' && c <= '9') {
                        token_append(token, c);
                    } else {
                        parser_error(&parser, c, "Expected digit");
                        return 1;
                    }
                } else if (PS_NODE_NAME_IFS == parser.state) {
                    if (c < 32 || c > 126) {
                        break;
                    } else {
                        parser_error(&parser, c, "Unexpected character for state");
                        return 1;
                    }
                } else {
                    parser_error(&parser, c, "Unexpected character for state");
                    return 1;
                }
                break;
        }
    }
    return 0;
}

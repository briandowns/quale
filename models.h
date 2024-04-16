#ifndef _MODELS_H
#define _MODELS_H

#include <stdlib.h>

typedef struct user {
    int id;
    char *first_name;
    char *last_name;
    char *email;
} user_t;

typedef struct vote {
    int id;
    char *description;
    int user_id;
} vote_t;

typedef struct vote_option {
    int id;
    char *option;
    int count;
    int vote_id;
} vote_option_t;

vote_t* vote_new(const char *desc, const uint64_t user_id);
void vote_free(vote_t *vote);

vote_option_t* vote_option_new(const char *text, const int vote_id);
void vote_option_free(vote_option_t *vo);

user_t* user_new(const char *first_name, const char *last_name, const char *email);
void user_free(user_t *user);

#endif /* _MODELS_H */

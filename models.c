#include <stdlib.h>
#include <string.h>

#include "models.h"

vote_t* vote_new(const char *desc, const uint64_t user_id) { 
    vote_t *vote = malloc(sizeof(vote_t));
    vote->description = malloc(strlen(desc));
    strcpy(vote->description, desc);
    vote->user_id = user_id;

    return vote;
}

void vote_free(vote_t *vote) {
    if (vote != NULL) {
        if (vote->description != NULL) {
            free(vote->description);
        }

        free(vote);
    }
}

vote_option_t* vote_option_new(const char *option, const int vote_id) {
    vote_option_t *vo = malloc(sizeof(vote_option_t));
    vo->option = malloc(strlen(option));
    strcpy(vo->option, option);
    vo->count = 0;
    vo->vote_id = vote_id;

    return vo;
}

void vote_option_free(vote_option_t *vo) {
    if (vo != NULL) {
        if (vo->option != NULL) {
            free(vo->option);
        }

        free(vo);
    }
}

user_t* user_new(const char *first_name, const char *last_name, const char *email) {
    user_t *user = malloc(sizeof(user_t));
    strcpy(user->first_name, first_name);
    strcpy(user->last_name, last_name);
    strcpy(user->email, email);

    return user;
}

void user_free(user_t *user) {
    if (user != NULL) {
        if (user->first_name != NULL) {
            free(user->first_name);
        }
        if (user->last_name != NULL) {
            free(user->last_name);
        }
        if (user->email != NULL) {
            free(user->email);
        }
        free(user);
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sqlite3.h>

#include "database.h"
#include "models.h"

static sqlite3 *db;

int create_table(const char *table) {
    sqlite3_stmt *stmt;

    char *query = {0};
    char *err = {0};
    
    if (strcmp(table, "votes") == 0) {
        query = strdup("CREATE TABLE IF NOT EXISTS votes(id INTEGER PRIMARY KEY AUTOINCREMENT, description TEXT, user_id INTEGER)");
    } else if (strcmp(table, "vote_options") == 0) {
        query = strdup("CREATE TABLE IF NOT EXISTS vote_options(id INTEGER PRIMARY KEY AUTOINCREMENT, option TEXT, count INTEGER, vote_id INTEGER)");
    } else if (strcmp(table, "users") == 0) {
        query = strdup("CREATE TABLE IF NOT EXISTS users(id INTEGER PRIMARY KEY AUTOINCREMENT, first_name TEXT, last_name TEXT, email TEXT)");
    }

    int rc = sqlite3_exec(db, query, NULL, 0, &err);
    if(rc != SQLITE_OK){
        fprintf(stderr, "error creating table [%s]: %s\n", table, err);
        sqlite3_free(err);

        return 1;
    }

    free(query);

    return 0;
}

void database_init() {
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    sqlite3_open_v2(FLASHCARD_DB, &db, flags, NULL);
	if (db == NULL) {
		printf("failed to open DB\n");
	}

    char *tables[] = {"votes", "vote_options", "users"};
    for (size_t i = 0; i < sizeof(tables) / sizeof(tables[0]); i++) {
        printf("creating table %s\n", tables[i]);
        if (create_table(tables[i]) == 1) {

        }
    }
}

void database_cleanup() {
    sqlite3_close_v2(db);
}

int insert_vote(const vote_t *vote) {
    char *err = {0};
    char *query = sqlite3_mprintf("INSERT INTO votes (description, user_id) VALUES (%Q, %d)", vote->description, vote->user_id);

    int rc = sqlite3_exec(db, query, NULL, 0, &err);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "failed to insert record; error: %s\n", err);
        sqlite3_free(err);

        return 1;
    }

    return 0;
}

int insert_vote_option(const vote_option_t *vo) {
    char *err = {0};

    char *query = sqlite3_mprintf("INSERT INTO vote_options (option, count, vote_id) VALUES (\"%s\", %d, %d);", vo->option, vo->count, vo->vote_id);
    int rc = sqlite3_exec(db, query, NULL, NULL, &err);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "failed to insert record; error: %d\n", rc);
        sqlite3_free(err);

        return 1;
    }

    return 0;
}

static int callback_vote(void *data, int argc, char **argv, char **azColName) {
    vote_t *vote = (vote_t*)data;
    vote->id = atoi(argv[0]);
    vote->description = realloc(vote->description, strlen(argv[1] + 1));
    strcpy(vote->description, argv[1]);
    vote->user_id = atoi(argv[2]);

    return 0;
}

static int callback_vote_option(void *data, int argc, char **argv, char **azColName) {
    vote_option_t *vo = (vote_option_t*)data;
    vo->id = atoi(argv[0]);
    vo->option = realloc(vo->option, strlen(argv[1] + 1));
    strcpy(vo->option, argv[1]);
    vo->count = atoi(argv[2]);
    vo->vote_id = atoi(argv[3]);

    return 0;
}

vote_t* get_vote_by_id(const int id) {
    sqlite3_stmt *stmt;

    vote_t *vote = vote_new("", 0);

    char *err = {0};
    char *query = sqlite3_mprintf("SELECT * FROM votes WHERE id = %d", id);

    sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

    int rc = sqlite3_exec(db, query, callback_vote, vote, &err);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "error getting vote [%d]: %s\n", id, err);
        sqlite3_free(err);
        vote_free(vote);

        return NULL;
    }

    return vote;
}

vote_t* get_vote_option_by_id(const int id) {
    sqlite3_stmt *stmt;

    vote_t *vote = vote_new("", 0);

    char *err = {0};
    char *query = sqlite3_mprintf("SELECT * FROM vote_options WHERE id = %d", id);

    sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

    int rc = sqlite3_exec(db, query, callback_vote, vote, &err);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "error getting vote [%d]: %s\n", id, err);
        sqlite3_free(err);
        vote_free(vote);

        return NULL;
    }

    return vote;
}

vote_option_t* get_vote_option_by_vote_id(const int id) {
    sqlite3_stmt *stmt;

    vote_option_t *vo = vote_option_new("", 0);

    char *err = {0};
    char *query = sqlite3_mprintf("SELECT vote_options.id, votes.description FROM votes JOIN vote_options ON vote_options.vote_id = votes.id WHERE votes.id = %d", id);

    sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

    int rc = sqlite3_exec(db, query, callback_vote_option, vo, &err);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "error getting vote [%d]: %s\n", id, err);
        sqlite3_free(err);
        vote_option_free(vo);

        return NULL;
    }

    return vo;
}
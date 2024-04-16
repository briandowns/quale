#ifndef _DATABASE_H
#define _DATABASE_H

#include <sqlite3.h>

#include "models.h" 

#define FLASHCARD_DB "quale.db"

void database_init();
void database_cleanup();
int insert_vote(const vote_t *vote);
int insert_vote_option(const vote_option_t *vo);
vote_t* get_vote_by_id(const int id); 
user_t* get_user_by_id(const u_int64_t id);

#endif /* _DATABASE_H */

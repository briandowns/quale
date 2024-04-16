#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#include "api.h"
#include "database.h"
#include "models.h"

#define STR1(x) #x
#define STR(x) STR1(x)

#define DEFAULT_PORT 8080

int control = 0;

/**
 * sig_handler captures ctrl-c
 */
static void sig_handler(int dummy) {
    control = 1;
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    signal(SIGINT, sig_handler);

    database_init();

    struct _u_instance instance;

    if (ulfius_init_instance(&instance, DEFAULT_PORT, NULL, NULL) != U_OK) {
        y_log_message(Y_LOG_LEVEL_ERROR, "error ulfius_init_instance, abort");
        return EXIT_FAILURE;
    }

    if (api_add_routes(&instance) != 0) {
        perror("unable to add routes");
        return EXIT_FAILURE;
    }

    // Start the framework
    if (ulfius_start_framework(&instance) == U_OK) {
        printf("Start framework on port %d\n", instance.port);

        // Wait for the user to press <enter> on the console to quit the application
        getchar();
    } else {
        fprintf(stderr, "error starting framework\n");
    }

    ulfius_stop_framework(&instance);
    ulfius_clean_instance(&instance);

    vote_t *vote1 = vote_new("Favorite color", 0);
    insert_vote(vote1);
    vote_free(vote1);

    vote1 = get_vote_by_id(10);
    vote_free(vote1);

    vote_option_t *vo1 = vote_option_new("Black", 1);
    vote_option_t *vo2 = vote_option_new("Red", 1);
    insert_vote_option(vo1);
    insert_vote_option(vo2);

    vote_option_free(vo1);
    vote_option_free(vo2);

    database_cleanup();

    return 0;
}

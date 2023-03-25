#include "logger.h"
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

pthread_barrier_t barrier;

void *thread_body(void *arg)
{
    pthread_barrier_wait(&barrier);
    char *job_name = (char*) arg;
    log_printf(LEVEL_DEBUG, "task '%s' is running", job_name);
    return NULL;
}

int main()
{
    char *tasks[] = {"job_01", "job_02", "job_03", "job_04", "job_05", "job_06", "job_07", "job_08", "job_09", "job_10"};
    size_t count = sizeof(tasks) / sizeof(tasks[0]);

#ifdef TO_FILE
    log_init_fpath("./message.log", false);
#else
    log_init_fstream(stdout);
#endif
    log_set_verbose(LEVEL_DEBUG);
    atexit(log_release);

    if (pthread_barrier_init(&barrier, NULL, count) != 0) {
        log_printf(LEVEL_ERROR, "%s", strerror(errno));
        return EXIT_FAILURE;
    }


    pthread_t *threads = (pthread_t*) calloc(count, sizeof(pthread_t));
    if (!threads) {
        log_printf(LEVEL_ERROR, "memmory cannot be allocated");
        return EXIT_FAILURE;
    }

    log_printf(LEVEL_INFO, "%d tasks runing", count);
    for (size_t i=0; i!=count; ++i) {
        int result = pthread_create(&threads[i], NULL, thread_body, tasks[i]);
        if (result) {
            log_printf(LEVEL_ERROR, "thread [%d] cannot be created", i);
            return EXIT_FAILURE;    
        }
    }

    for (size_t i=0; i!=count; ++i) {
        int result = pthread_join(threads[i], NULL);
        if (result) {
            log_printf(LEVEL_ERROR, "thread [%d] cannot be joined", i);
            return EXIT_FAILURE;
        }
    }
    
    log_printf(LEVEL_WARNING, "test warning message");

    pthread_barrier_destroy(&barrier);
    free(threads);
    log_release();

    return 0;
}

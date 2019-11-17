#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include <time.h>
#include "helpers.h"

/*
 Returns last time rule was modified.
 */
time_t last_modified_time(char *path) {
    struct stat attr;
    if (stat(path, &attr) == 0) {
        return attr.st_mtime;
    }
    return 0;
}

/*
 Returns 1 if t2 is modified more recently,
 0 otherwise.
*/
int compare_times(struct timespec t1, struct timespec t2) {
    if (t1.tv_sec > t2.tv_sec) {
        // t1 older than t2
        return 1;
    }
    if (t1.tv_sec < t2.tv_sec) {
        return 0;
    }
    if (t1.tv_nsec > t2.tv_nsec) {
        // t1 older than t2 in nano
        return 1;
    }
    return 0;
}

/*
Execute given action.
*/
void execute_action(Action *act) {
    pid_t ppid = getppid();
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("fork");
        exit(1);
    }
    if (pid == 0) {
        execvp(act->args[0], act->args);
    }
    if (pid > 0) {
        int exitstatus;
        if (waitpid(ppid, &exitstatus, 0) == -1 ) {
            exit(EXIT_FAILURE);
        }
    }
}

/*
Recursively evaluate
each dependency rule.
*/
void evaluate_rule(Rule *rule) {
    time_t last_mtime = last_modified_time(rule->actions->args[0]);
    Dependency *dep = rule->dependencies;
    int rerun_actions = 0;
    while (dep != NULL) {
        pid_t pid = fork();
        
        evaluate_rule(dep->rule);
        time_t dep_mtime = last_modified_time(dep->rule->target);
        
        if (last_mtime == 0 || dep_mtime > last_mtime) {
            rerun_actions = 1;
        }
        dep = dep->next_dep;
    }
    for (int i = 0; i < 10; i++) {
        int status;
        wait(&status);
    }
    if (rerun_actions) {
        Action *act = rule->actions;
        while (act != NULL) {
            execute_action(act);
            act = act->next_act;
        }
    }
    
}


// ====  end helpers ====



void run_make(char *target, Rule *rules, int pflag)
{
    Rule *rule = rules;
    if (target != NULL) {
        rule = get_rule(target, rules);
    }
    evaluate_rule(rule);
}

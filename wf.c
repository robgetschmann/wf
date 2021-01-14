/**
 * @file wf.c
 *
 * @author Robert A. Getschmann <rob@getschmann.net>
 *
 * @section DESCRIPTION
 *
 * Entry point for the word frequency (wf) program; This module coordinates
 * the child processes which perform filting of the input as wel as the word
 * frequency analysis
 *
 * Any catastrophic error conditions are handled by calling assert() and
 * terminating the process;  In these situations memory will not be freed
 * and file descriptors will not be closed; This is acceptable since the
 * operating system will clean up upon process termination
 *
 * @section LICENSE
 *
 * ===========================================================================
 *
 * Copyright © 2018, Robert A. Getschmann <rob@getschmann.net>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the <organization>.
 * 4. Neither the name of the <organization> nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY <COPYRIGHT HOLDER> ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ===========================================================================
 */

#include <wf.h>

#include <count.h>
#include <squeeze.h>

/*
 * forward declarations
 */
typedef struct Job Job;

/**
 * @brief   word frequency job function entry point prototype
 */
typedef void (JobFunc)(Job* job);

/**
 * @brief   object for maintaining a child process "job" configurables
 */
struct Job {
    JobFunc* entry;     /** function entry point for the job */
    int channel[2];     /** pipe for process communication */
};

/*
 * function prototypes
 */
static JobFunc jobCount;
static JobFunc jobSqueeze;

/**
 * @brief   vector of the jobs for processing the text word frequencies
 */
static Job jobVector[] = {
    { jobSqueeze,   { -1, -1} },
    { jobCount,     { -1, -1} },
};

/**
 * @brief   the count of the number of jobs supported
 */
static uint32_t jobMax = sizeof(jobVector) / sizeof(jobVector[0]);

/**
 * @brief   the top frequency counts to be displayed - Global
 */
uint32_t frequencyCount = 20;

/**
 * @brief   entry point for the word frequency count job
 * @param   job the job entry details
 * @return  none
 */
static void
__attribute__ ((noreturn))
jobCount(Job* job)
{

    int fd; /* input file descriptor */

    /* The count job only reads from the pipe input, close the output. */
    close(job->channel[1]);

    /* Read input from the pipe, and write out to standard output. */
    fd = dup2(job->channel[0], STDIN_FILENO);
    assert(fd != -1);

    /*
     * Exit the child process with the return status from count().
     * Upon exiting the child process the input and output file streams
     * will be closed indicated EOF to any readers.
     *
     * Final cleanup of dynamically allocated memory will not be performed
     * since operating system takes care of this when the process terminates.
     */
    exit(count(stdin, stdout));

}

/**
 * @brief   entry point for the input filtering job
 * @param   job the job entry details
 * @return  none
 */
static void
__attribute__ ((noreturn))
jobSqueeze(Job* job)
{

    int fd; /* output file descriptor */

    /* The squeeze job only writes to the pipe output, close the input. */
    close(job->channel[0]);

    /* Read input from standard input, and write out to the pipe. */
    fd = dup2(job->channel[1], STDOUT_FILENO);
    assert(fd != -1);

    /*
     * Exit the child process with the return status from squeeze().
     * Upon exiting the child process the input and output file streams
     * will be closed indicated EOF to any readers.
     *
     * Final cleanup of dynamically allocated memory will not be performed
     * since operating system takes care of this when the process terminates.
     */
    exit(squeeze(stdin, stdout));

}

/**
 * @brief   entry point for the "wf" program
 * @details the main() function is the entry point for the "wf" program;  It
 *          processes the command line arguments and spawns two child processes
 *          for performing word frequency for an input stream;  The first child
 *          process filters valid words;  The second child process performs the
 *          frequency analysis
 * @param   argc the command line argument count
 * @param   argv the command line argument vector
 * @return  0 - success, 1 - filtering process failed, 2 - frequency analysis
 *          process failed, 3 - both failed, 4 - invalid invocation
 * @todo    improve handling of wait() child status processing
 */
int
main(int argc,
     char** argv)
{

    /* Process any command line arguments. */
    switch (argc) {

        /* Error - zero or one command line argument only! */
        default: {
            fprintf(stderr,
                    "usage: %s [ <FILE> | \"-\" ]\n",
                    basename(argv[0]));
            exit (4);
            break;
        }

        /* No command line arguments provided. */
        case 1: {
            /* Input in from standard input. */
            break;
        }

        /* A single command line argument provided. */
        case 2: {
            /* "-" as the file name indicates standard input. */
            if (strncmp(argv[1], "-", strlen("-")) != 0) {
                fclose(stdin);
                stdin = fopen(argv[1], "r");
                assert(stdin);
            }
            break;
        }

    }

    /*
     * Create a pipe for IPC between the child processes.  Copy
     * the channel object into each of the jobs for pipeline setup.
     */
    int channel[2];
    assert(pipe(channel) != -1);

    memcpy(jobVector[0].channel, channel, sizeof(channel));
    memcpy(jobVector[1].channel, channel, sizeof(channel));

    uint32_t job_count = 0;

    /* Create a child process for the squeeze and count jobs. */
    for (uint32_t i = 0; i < jobMax; i++) {

        pid_t child;

        switch ((child = fork())) {

            /* Error - fork() failed, abort. */
            case -1: {
                assert(child != -1);
                break;
            }

            /* Parent - do nothing. */
            default: {
                job_count++;
                break;
            }

            /* Child - jump to the job to be executed. */
            case 0: {
                jobVector[i].entry(&jobVector[i]);
                break;
            }

        }

    }

    /* The parent does not do IPC with the child processes, close the pipe. */
    close(channel[0]);
    close(channel[1]);

    /* Wait for the child processes to terminate. */
    int status = 0;

    for (uint32_t i = 0; i < job_count; i++) {

        pid_t child __attribute__ ((unused));
        int child_status;

        child = wait(&child_status);
        
        if (WIFEXITED(child_status)) {
            status += WEXITSTATUS(status);
        }
        /* TODO: Handle other process events. */

    }

    /*
     * Since this is a filter the memory is used for the complete run of the
     * program.  Memory for the various structures won't be free.  Instead the
     * operating system will clean up upon process termination.
     */
    exit(status);

}

/*
 * vim: cindent:cinoptions+={0,>1s,(0,t0,l1,^0:expandtab:smartindent:sw=4:ts=4:tw=0
 */

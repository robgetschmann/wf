/**
 * @file wf.c
 *
 * @author Robert A. Getschmann <rob@getschmann.net>
 *
 * @section DESCRIPTION
 *
 * @section LICENSE
 *
 * ===========================================================================
 *
 * Copyright (c) 2017-2018, Robert A. Getschmann <rob@getschmann.net>
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

/**
 *
 */
typedef struct Job Job;

typedef void (JobFunc)(Job* job,
                       int channel[2]);

/**
 *
 */
struct Job {
    JobFunc* entry;
    struct {
        int input;
        int output;
    } fd;
};

/*
 * Forward Declarations
 */
static JobFunc jobCount;
static JobFunc jobSqueeze;

/**
 *
 */
static Job jobVector[] = {
    { jobSqueeze },
    { jobCount },
};

/**
 *
 */
const uint32_t frequencyCount = 20;

/**
 * @brief
 * @details
 * @param
 * @return
 */
static void
__attribute__ ((noreturn))
jobCount(Job* job,
         int channel[2])
{

    close(channel[1]);

    /* Read input from the pipe, and write out out to standard output. */
    job->fd.input = dup2(channel[0], STDIN_FILENO);
    job->fd.output = STDOUT_FILENO;

    /* Open the input/output file descriptors as streams. */
    FILE* ifp = fdopen(job->fd.input, "r");
    assert(ifp);

    FILE* ofp = fdopen(job->fd.output, "w");
    assert(ofp);

    exit(count(ifp, ofp));

}

/**
 * @brief
 * @details
 * @param
 * @return
 */
static void
__attribute__ ((noreturn))
jobSqueeze(Job* job,
           int channel[2])
{

    close(channel[0]);

    /* Read input from standard input, and write out out to the pipe. */
    job->fd.input = STDIN_FILENO;
    job->fd.output = dup2(channel[1], STDOUT_FILENO);

    /* Open the input/output file descriptors as streams. */
    FILE* ifp = fdopen(job->fd.input, "r");
    assert(ifp);

    FILE* ofp = fdopen(job->fd.output, "w");
    assert(ofp);

    exit(squeeze(ifp, ofp));

}

/**
 * @brief   Entry point for the "wf" program
 * @details The main() function is the entry point for the "wf" program;  It
 *          processes the command line arguments and spawns two child processes
 *          for performing word frequency for an input stream;  The first child
 *          process filters valid words;  The second child process performs the
 *          frequency analysis
 * @param   argc The command line argument count
 * @param   argv The command line argument vector
 * @return  0 - success, 1 - filtering process failed, 2 - frequency analysis
 *          process failed, 3 - both failed
 */
int
main(int argc,
     char** argv)
{

    /* Process any command line arguments. */
    switch (argc) {

        default: {
            assert(argc == 1 || argc == 2);
            break;
        }

        /* No command line arguments provided. */
        case 1: {
            /* Input in from standard input. */
            break;
        }

        /* A single command line argument provided. */
        case 2: {
            fclose(stdin);
            stdin = fopen(argv[1], "r");
            break;
        }

    }

    /* Create a pipe for IPC between the child processes. */
    int channel[2];
    assert(pipe(channel) != -1);

    for (uint32_t i = 0; i < 2; i++) {

        pid_t child;

        switch ((child = fork())) {

            /* Error - fork() failed, abort. */
            case -1: {
                assert(child != -1);
                break;
            }
            /* Parent - do nothing. */
            default: {
                break;
            }
            /* Child - jump to the job to be executed. */
            case 0: {
                jobVector[i].entry(&jobVector[i], channel);
                break;
            }

        }

    }

    /* The parent needs to close the pipe used only by the child processes. */
    close(channel[0]);
    close(channel[1]);

    /* Wait for the child processes to terminate. */
    for (uint32_t i = 0; i < 2; i++) {

        pid_t child __attribute__ ((unused));
        int status;

        child = wait(&status);

    }

}

/*
 * vim: cindent:cinoptions+={0,>1s,(0,t0,l1,^0:expandtab:smartindent:sw=4:ts=4:tw=0
 */

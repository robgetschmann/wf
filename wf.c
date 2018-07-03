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

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

extern int count(FILE*, FILE*);
extern int squeeze(FILE*, FILE*);

/*
 * Forward Declarations.
 */

static int job_count(FILE* ifp, FILE* ofp);
static int job_squeeze(FILE* ifp, FILE* ofp);

/**
 *
 */
int
job_count(FILE* ifp,
          FILE* ofp)
{
    return (0);
}

/**
 *
 */
int
job_squeeze(FILE* ifp,
            FILE* ofp)
{
    return (0);
}

/**
 *
 */
int
main(int argc,
     char** argv)
{

    FILE* wf_ifp = stdin;
    FILE* wf_ofp = stdout;

    switch (argc) {

        default: {
            exit (1);
            break;
        }
        case 1: {
            break;
        }
        case 2: {
            wf_ifp = fopen(argv[1], "r");
            break;
        }

    }

    int comm[2];

    pipe(comm);

    pid_t child;
    int rc;

    switch (child = fork()) {

        /* Error */
        case -1: {
            break;
        }

        /* Child Process */
        case 0: {

            FILE* ifp, *ofp;
            int rc;

            close(comm[0]);

            ifp = stdin;
            ofp = fdopen(comm[1], "w");

            rc = squeeze(ifp, ofp);
            fclose(ofp);

            break;

        }

        /* Parent Process */
        default: {

            FILE* ifp, *ofp;
            int status;

            close(comm[1]);

            ifp = fdopen(comm[0], "r");
            ofp = stdout;

            rc = count(ifp, ofp);

            /* Wait for the child to terminate. */
            waitpid(child, &status, 0);

            break;

        }

    }

    exit(rc);

}

/*
 * vim: cindent:cinoptions+={0,>1s,(0,t0,l1,^0:expandtab:smartindent:sw=4:ts=4:tw=0
 */

/**
 * @file squeeze.c
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

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <squeeze.h>

/*
 * Forward Declarations.
 */
static int isalpha(int c);
static int tolower(int c);

static int __attribute__ ((unused))
squeeze_main(int argc, char** argv);

/**
 * @brief
 * @details
 * @param
 * @return
 */
int
isalpha(int c)
{

    return (((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) ? 1 : 0);

}

/**
 * @brief
 * @details
 * @param
 * @return
 */
int
squeeze(FILE* ifp,
        FILE* ofp)
{

    char buffer[4096];
    bool squeezing = false;
    int rc;

    setvbuf(ofp, buffer, _IOLBF, sizeof(buffer));
    squeezing = false;
    rc = 0;

    for (char c; (c = fgetc(ifp)) != EOF; ) {
        if (isalpha(c)) {
            fputc(tolower(c), ofp);
            squeezing = false;
        }
        else if (squeezing == false) {
            fputc('\n', ofp);
            squeezing = true;
        }
    }

    /* End of input reached, flush the output stream. */
    if (feof(ifp)) {
        fflush(ofp);
        rc = 0;
    }
    /* An input error occurred, fail the function. */
    else if (ferror(ifp)) {
        rc = -1;
    }

    return (rc);

}

/**
 * @brief
 * @details
 * @param
 * @return
 */
static int
squeeze_main(int argc,
             char** argv)
{

    exit(squeeze(stdin, stdout));

}


/**
 * @brief
 * @details
 * @param
 * @return
 */
int
tolower(int c)
{

    if (c >= 'A' && c <= 'Z') {
        c = c - 'A' + 'a';
    }

    return (c);

}

/*
 * vim: cindent:cinoptions+={0,>1s,(0,t0,l1,^0:expandtab:smartindent:sw=4:ts=4:tw=0
 */

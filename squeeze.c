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

#include <squeeze.h>
#include <wf.h>

/**
 * @brief   Filter input within the character class [a-zA-Z]
 * @details Filter the input stream to remove characters not in the class
 *          [a-zA-Z]; Words ([a-zA-Z]) will be output to the oufput file
 *          stream and delimited by a newline character.
 * @param   ifp input stream file pointer
 * @param   ofp output stream file pointer
 * @return  0 - success, -1 - failure
 */
int
squeeze(FILE* ifp,
        FILE* ofp)
{

    /* Output will be fully buffered. */
    char buffer[4096];
    setvbuf(ofp, buffer, _IOLBF, sizeof(buffer));

    bool squeezing = false;
    char byte = '\0';
    int length = 0;
    int read_count = 0;
    int status = 0;

    /*
     * Process a byte at a time from the input stream.
     *
     * All upper case characters in the class [a-zA-Z] are converted to
     * the lower case equivalent.
     *
     * A newline character delimts words in the output stream.
     *
     * The squeezing Boolean is true when multiple contiguous characters
     * NOT in the class [a-zA-Z] are being processed.
     */
    while ((read_count = fread(&byte, 1, 1, ifp))) {

        if (isalpha(byte)) {
            fputc(tolower(byte), ofp);
            length++;
            squeezing = false;
        }

        else if (length != 0 && squeezing == false) {
            fputc('\n', ofp);
            length = 0;
            squeezing = true;
        }

    }

    /* End of input reached, flush the output stream. */
    if (feof(ifp)) {
        status = 0;
    }
    /* An input error occurred, fail the function. */
    else if (ferror(ifp)) {
        status = -1;
    }

    return (status);

}

/*
 * vim: cindent:cinoptions+={0,>1s,(0,t0,l1,^0:expandtab:smartindent:sw=4:ts=4:tw=0
 */

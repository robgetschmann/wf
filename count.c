/**
 * @file count.c
 *
 * @author Robert A. Getschmann <rob@getschmann.net>
 *
 * @section DESCRIPTION
 *
 * Implementation module for character "count" (frequency) functionality
 *
 * @section LICENSE
 *
 * ===========================================================================
 *
 * Copyright (c) 2018, Robert A. Getschmann <rob@getschmann.net>
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
#include <heap.h>
#include <trie.h>

/**
 * @brief   process a stream of words and determine each word's frequency
 * @param   ifp the input stream containing newline delimited valid words
 * @param   ofp the output stream for the frequency count and word display
 * @return  0 - success, -1 - failure
 */
int
count(FILE* ifp,
      FILE* ofp)
{

    /* Create a new trie and a new heap */
    Heap* heap = heapNew(frequencyCount);
    TrieNode* trie = trieNodeNew();

    /*
     * Process each newline delimited word in the input stream  and insert
     * it into the trie.
     */
    char* buffer = NULL;
    size_t buffer_size = 0;

    while (getdelim(&buffer, &buffer_size, '\n', ifp) != -1) {

        /* Eat the newline. */
        buffer[strlen(buffer)-1] = '\0';

        TrieNode* node = trieInsert(trie, buffer);
        assert(node);

        heapInsert(heap, node, buffer);

    }

    /* Free the buffer allocated by getdelim(). */
    if (buffer) {
        free(buffer);
    }

    int status = 0;

    /* End of input reached, report success. */
    if (feof(ifp)) {
        status = 0;
    }
    /* An input error occurred, report failure. */
    else if (ferror(ifp)) {
        status = -1;
    }

    /* Only sort and display if the input stream ended cleanly. */
    if (status == 0) {

        /* Sort the heap. */
        heapSort(heap);

        /* Display the frequency of the top word counts. */
        heapDump(heap, ofp);

    }

    return (status);

}

/*
 * vim: cindent:cinoptions+={0,>1s,(0,t0,l1,^0:expandtab:smartindent:sw=4:ts=4:tw=0
 */

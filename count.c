/**
 * @file count.c
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

#include <count.h>
#include <heap.h>
#include <trie.h>
#include <wf.h>

/*
 * Forward Declarations
 */
static int dump(Heap* heap, FILE* fp);

static int insert(TrieNode** root,
                  Heap* heap,
                  const char* word,
                  const char* original);

/**
 * @brief
 * @details
 * @param   ifp the input stream containing newline delimited valid words
 * @param   ofp the output stream for the frequency count and word display
 * @return  0 - success, -1 - failure
 */
int
count(FILE* ifp,
      FILE* ofp)
{

    Heap* heap = heapNew(frequencyCount);
    TrieNode* root = NULL;
    int status = 0;

    for (char buffer[4096]; fgets(buffer, sizeof(buffer), ifp); ) {
        buffer[strlen(buffer)-1] = '\0';
        insert(&root, heap, buffer, buffer);
    }

    /* End of input reached, flush the output stream. */
    if (feof(ifp)) {
        status = 0;
    }
    /* An input error occurred, fail the function. */
    else if (ferror(ifp)) {
        status = -1;
    }

    /* Display the frequency of the top word counts. */
    dump(heap, ofp);

    return (status);

}

/**
 * @brief   Display the top frequency counts and the corresponding word
 * @details
 * @param   heap the heap containing the top frequency counts and word
 * @param   ofp the output file stream
 * @return  0 - success, -1 - failure
 */
static int
dump(Heap* heap,
     FILE* ofp)
{

    for (uint32_t i = 0; i < heap->count; i++) {
        fprintf(ofp, "%7d %s\n",
                heap->vector[i].frequency,
                heap->vector[i].word);
    }

    return (0);

}

/**
 * @brief
 * @details
 * @param
 * @return  0 - success, -1 - failure
 */
static int
insert(TrieNode** root,
       Heap* heap,
       const char* word,
       const char* original)
{

    if (!*root) {

        *root = trieNodeNew();

    }

    if (*word != '\0') {

        insert(&(*root)->child[tolower(*word) - 'a'],
               heap,
               word+1,
               original);

    }

    else {

        // word is already present, increase the frequency
        if ((*root)->complete == true) {
            (*root)->frequency++;
        }
        else {
            (*root)->complete = true;
            (*root)->frequency = 1;
        }

        // Insert in min heap also
        heapInsert(heap, root, original);

    }

    return (0);

}

/*
 * vim: cindent:cinoptions+={0,>1s,(0,t0,l1,^0:expandtab:smartindent:sw=4:ts=4:tw=0
 */

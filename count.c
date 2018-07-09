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

/*
 * Forward declarations
 */
static int
dump(Heap* heap,
     FILE* fp);

static int
insert(TrieNode** root,
       Heap* heap,
       const char* word,
       const char* original);

/**
 * @brief   Process a stream of words and determine each word's frequency
 * @details
 * @param   ifp the input stream containing newline delimited valid words
 * @param   ofp the output stream for the frequency count and word display
 * @return  0 - success, -1 - failure
 */
int
count(FILE* ifp,
      FILE* ofp)
{

    static char* buffer;
    static size_t buffer_size;

    Heap* heap = heapNew(frequencyCount);
    TrieNode* root = NULL;
    int status = 0;

    while (getdelim(&buffer, &buffer_size, '\n', ifp) != -1) {
        /* Eat the newline. */
        buffer[strlen(buffer)-1] = '\0';
        insert(&root, heap, buffer, buffer);
    }

    /* End of input reached, report success. */
    if (feof(ifp)) {
        status = 0;
    }
    /* An input error occurred, report failure. */
    else if (ferror(ifp)) {
        status = -1;
    }

    /* Display the frequency of the top word counts. */
    dump(heap, ofp);

    /* Free the buffer allocated by getdelim(). */
    if (buffer) {
        free(buffer);
    }

    return (status);

}

/**
 * @brief   Display the top frequency counts and the corresponding word
 * @details Display the top frequency counts; The first column contains
 *          the count and the second column contains the corresponding word;
 *          Each line is a record and the records are sorted in reverse
 *          numerical order based on the frequency count
 * @param   heap the heap containing the top frequency counts and word
 * @param   ofp the output file stream
 * @return  0 - success, -1 - failure
 */
static int
dump(Heap* heap,
     FILE* ofp)
{

    for (uint32_t i = 0; i < heap->count; i++) {
        fprintf(ofp, "%d %s\n",
                heap->vector[i].frequency,
                heap->vector[i].word);
    }

    return (0);

}

/**
 * @brief   Insert a word into a Trie and prioritize it in the corresponding
 *          heap
 * @details
 * @param   root Trie head
 * @param   heap Heap for most frequent words in Trie
 * @param   word pointer to current character in word being inserted
 * @param   original pointer to original word being inserted
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

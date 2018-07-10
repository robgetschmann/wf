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
 * function prototypes
 */
static int
insert(TrieNode* trie,
       Heap* heap,
       const char* word);

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

    static char* buffer;
    static size_t buffer_size;

    Heap* heap = heapNew(frequencyCount);
    TrieNode* trie = trieNodeNew();
    int status = 0;

    /*
     * Process each newline delimited word in the input stream  and insert
     * it into the trie.
     */
    while (getdelim(&buffer, &buffer_size, '\n', ifp) != -1) {
        /* Eat the newline. */
        buffer[strlen(buffer)-1] = '\0';
        insert(trie, heap, buffer);
    }

    /* End of input reached, report success. */
    if (feof(ifp)) {
        status = 0;
    }
    /* An input error occurred, report failure. */
    else if (ferror(ifp)) {
        status = -1;
    }

    /* Sort the heap. */
    heapSort(heap);

    /* Display the frequency of the top word counts. */
    heapDump(heap, ofp);

    /* Free the buffer allocated by getdelim(). */
    if (buffer) {
        free(buffer);
    }

    return (status);

}

/**
 * @brief   insert a word into a trie and prioritize it in the corresponding
 *          heap
 * @param   trie trie root
 * @param   heap Heap for most frequent words in trie
 * @param   word pointer to current character in word being inserted
 * @return  0 - success, -1 - failure
 */
static int
insert(TrieNode* trie,
       Heap* heap,
       const char* word)
{

    const char* traverse = word;
    TrieNode* node;

    for (node = trie; *traverse != '\0'; traverse++) {

        if (!node->child[tolower(*traverse)-'a']) {
            node->child[tolower(*traverse)-'a'] = trieNodeNew();
        }

        node = node->child[tolower(*traverse)-'a'];

    }

    /*
     * At this point the terminating node in the trie has been reached
     * and the word and frequency can be inserted or updated.
     */

    /* The word is already in the trie, update the frequency. */
    if (node->complete == true) {
        node->frequency++;
    }
    /* The word is new and needs to be inserted into the trie. */
    else {
        node->complete = true;
        node->frequency = 1;
    }

    /* Insert or update the word in the heap. */
    heapInsert(heap, node, word);

    return (0);

}

/*
 * vim: cindent:cinoptions+={0,>1s,(0,t0,l1,^0:expandtab:smartindent:sw=4:ts=4:tw=0
 */

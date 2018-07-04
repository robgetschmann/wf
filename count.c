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

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <count.h>
#include <heap.h>
#include <trie.h>

/*
 * Forward Declarations.
 */
static int __attribute__ ((unused))
    count_main(int argc, char** argv);

    // Inserts a new word to both Trie and Heap
void insertUtil(TrieNode** root,
                MinHeap* minHeap,
                const char* word,
                const char* dupWord)
{

    // Base Case
    if (*root == NULL) {
        *root = newTrieNode();
    }

    // There are still more characters in word
    if (*word != '\0' ) {
        insertUtil(&((*root)->child[tolower(*word ) - 97]),
                   minHeap,
                   word + 1,dupWord);
    }

    // The complete word is processed
    else
    {
        // word is already present, increase the frequency
        if ( (*root)->isEnd ) {
            ++( (*root)->frequency );
        }
        else
        {
            (*root)->isEnd = 1;
            (*root)->frequency = 1;
        }

        // Insert in min heap also
        insertInMinHeap( minHeap, root, dupWord );
    }

}


// add a word to Trie & min heap. A wrapper over the insertUtil
void insertTrieAndHeap(const char *word,
                       TrieNode** root,
                       MinHeap* minHeap)
{

    insertUtil(root, minHeap, word, word);

}

// A utility function to show results, The min heap
// contains k most frequent words so far, at any time
void displayMinHeap( MinHeap* minHeap )
{
    int i;

    // print top K word with frequency
    for( i = 0; i < minHeap->count; ++i ) {
        printf("%7d %s\n",
               minHeap->array[i].frequency,
               minHeap->array[i].word);
    }
}

/**
 * @brief
 * @details
 * @param
 * @return
 */
int
count(FILE* ifp,
      FILE* ofp)
{

    char buffer[4096];
    MinHeap* minHeap;
    TrieNode* root;
    int status;

    minHeap = createMinHeap(20);
    root = NULL;

    status = 0;

    while (fgets(buffer, sizeof(buffer), ifp)) {
        buffer[strlen(buffer)-1] = '\0';
        insertTrieAndHeap(buffer, &root, minHeap);
    }

    if (ferror(ifp)) {
        status = -1;
    }

    displayMinHeap(minHeap);

    return (status);

}

/**
 * @brief
 * @details
 * @param
 * @return
 */
static int
count_main(int argc,
           char** argv)
{

    exit(count(stdin, stdout));

}

/*
 * vim: cindent:cinoptions+={0,>1s,(0,t0,l1,^0:expandtab:smartindent:sw=4:ts=4:tw=0
 */

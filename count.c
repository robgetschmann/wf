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

#include <wf.h>

#include <count.h>
#include <heap.h>
#include <trie.h>

/*
 * Forward Declarations.
 */
static int insert(TrieNode** root, Heap* heap, const char* word, const char* dupWord);
static int dump(Heap* heap, FILE* fp);

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

    dump(heap, ofp);

    fclose(ifp);
    fclose(ofp);

    return (status);

}

/**
 * @brief Inserts a new word to both Trie and Heap
 * @details
 * @param
 * @return
 */
static int
insert(TrieNode** root,
       Heap* heap,
       const char* word,
       const char* dupWord)
{

    // Base Case
    if (!*root) {

        *root = trieNodeNew();

    }

    // There are still more characters in word
    if (*word != '\0') {

        insert(&(*root)->child[tolower(*word) - 'a'],
               heap,
               word+1,
               dupWord);

    }

    // The complete word is processed
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
        heapInsert(heap, root, dupWord);

    }

    return (0);

}

/**
 *
 * @brief
 * @details
 * @param
 * @return
 */
static int
dump(Heap* heap,
     FILE* fp)
{

    for (uint32_t i = 0; i < heap->count; i++) {
        fprintf(fp, "%7d %s\n",
                heap->vector[i].frequency,
                heap->vector[i].word);
    }

    return (0);

}

/*
 * vim: cindent:cinoptions+={0,>1s,(0,t0,l1,^0:expandtab:smartindent:sw=4:ts=4:tw=0
 */

/**
 * @file trie.c
 *
 * @author Robert A. Getschmann <rob@getschmann.net>
 *
 * @section DESCRIPTION
 *
 * Implementation module for trie object
 *
 * @section LICENSE
 *
 * ===========================================================================
 *
 * Copyright © 2018, Robert A. Getschmann <rob@getschmann.net>
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

#include <trie.h>

/**
 * @brief   insert a word into a trie
 * @param   trie trie root
 * @param   word pointer to the word to be inserted
 * @return  a pointer to the trie node containing the word
 */
TrieNode*
trieInsert(TrieNode* trie,
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
    node->frequency++;

    return (node);

}

/**
 * @brief   allocate and initialize a new Trie Node from the heap
 * @param   none
 * @return  a pointer to a new Trie Node
 */
TrieNode*
trieNodeNew(void)
{

    TrieNode* node;

    node = calloc(1, sizeof(*node));

    node->frequency = 0;
    node->index = -1;

    return (node);

}

/*
 * vim: cindent:cinoptions+={0,>1s,(0,t0,l1,^0:expandtab:smartindent:sw=4:ts=4:tw=0
 */

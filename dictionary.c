/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "dictionary.h"
// set default values of node

int D_COUNT = 0;
bool D_LOADED = false;
node *root;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    //---------------logic is flawed!!!!! - step through find the right way
    node *curs = root;
    int i;
    int c = 0;


    while(word[c] != '\0')
    {
        i = trie_index(word[c]);
        c++;
        if(curs->children[i] == NULL)
        {
            return false;
        }
        else
        {
            curs = curs->children[i];
        }

    }


    if(curs->is_word)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // remember root node of trie and set defaults
    root = calloc(1,sizeof(node));
    root->is_word = false;
    if(root == NULL)
    {
        fprintf(stderr, "Unable to allocate memory");
        return false;
    }

    // creat create dictonary word placeholder
    char dword[LENGTH + 1];

    //open file
    FILE *inptr = fopen(dictionary, "r");
    // check to see if file was opened
    if(inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", dictionary);
        return false;
    }

    //scan dictionary word by word
    while(fscanf(inptr, "%s", dword) != EOF)
    {
        //Build word in to trie structure otherwisw
        if(!trie_build_nodes(root, dword, 0))
        {
            fclose(inptr);
            return false;
        }

    }

    D_LOADED = true;
    fclose(inptr);
    //fprintf(stderr, "Succesfully loaded");
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // TODO
    if(D_LOADED)
    {
        return D_COUNT;
    }
    else
    {
        return 0;
    }
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    //fprintf(stderr, "WORD COUNT: %i", D_COUNT);
    free_trie_mem(root);
    return true;


}

//-------LOAD NODES RECURSIVELY---------//


bool trie_build_nodes(node *current_node, char *dword, int index)
{
    int i = trie_index(dword[index]);


    if(dword[index] == '\0')
    {
        current_node->is_word = true;
        D_COUNT++;
        return true;
    }

    if(current_node->children[i] == NULL)
    {
        //create new node pointer
        node *new_node = calloc(1, sizeof(node));
        if(new_node == NULL)
        {
            fprintf(stderr, "Unable to allocate memory");
            return false;
        }
        new_node->is_word = false;
        current_node->children[i] = new_node;
        index++;
        return trie_build_nodes(current_node->children[i], dword, index);
    }
    else
    {
        index++;
        return trie_build_nodes(current_node->children[i], dword, index); //look at dword!
    }

}


void free_trie_mem(node *current_node)
{
    for(int c = 0; c < 27; c++)
    {
        if(current_node->children[c] != NULL)
        {
            free_trie_mem(current_node->children[c]);
        }
    }

    free(current_node);
}

//-----get the index for array of
int trie_index(char c)
{
    c = tolower(c);
    return c == 39 ? 26 : c - 97;
}


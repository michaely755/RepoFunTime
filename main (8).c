#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Trie
{
    int count;
    struct Trie *children[26];
};

struct Trie *createTrie()
{
    struct Trie *node = (struct Trie *)malloc(sizeof(struct Trie));
    if (node)
    {
        node->count = 0;
        for (int i = 0; i < 26; ++i)
        {
            node->children[i] = NULL;
        }
    }
    return node;
}

void insert(struct Trie *pTrie, char *word)
{
    struct Trie *node = pTrie;
    for (int i = 0; word[i] != '\0'; ++i)
    {
        int index = word[i] - 'a';
        if (!node->children[index])
        {
            node->children[index] = createTrie();
        }
        node = node->children[index];
    }
    node->count++;
}

int numberOfOccurrences(struct Trie *pTrie, char *word)
{
    struct Trie *node = pTrie;
    for (int i = 0; word[i] != '\0'; i++)
    {
        int index = word[i] - 'a';
        if (!node->children[index])
        {
            return 0;
        }
        node = node->children[index];
    }
    return node->count;
}

void deallocateTrie(struct Trie *pTrie)
{
    if (pTrie)
    {
        for (int i = 0; i < 26; ++i)
        {
            if (pTrie->children[i])
            {
                deallocateTrie(pTrie->children[i]);
            }
        }
        free(pTrie);
    }
}



int readDictionary(char *filename, char **pInWords)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        exit(1);
    }

    int numWords;
    fscanf(file, "%d", &numWords);

    for (int i = 0; i < numWords; ++i)
    {
        pInWords[i] = (char *)malloc(100 * sizeof(char));
        fscanf(file, "%s", pInWords[i]);
    }

    fclose(file);

    return numWords;
}

int main(void)
{
    char *inWords[256];

    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i)
    {
        printf("%s\n", inWords[i]);
    }

    struct Trie *pTrie = createTrie();
    for (int i = 0; i < numWords; i++)
    {
        insert(pTrie, inWords[i]);
    }

    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }

    deallocateTrie(pTrie);

    for (int i = 0; i < numWords; ++i)
    {
        free(inWords[i]);
    }

    return 0;
}
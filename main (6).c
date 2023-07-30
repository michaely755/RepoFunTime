#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
    int id;
    char name;
    int order;
};

struct Node
{
    struct RecordType data;
    struct Node *next;
};

// Fill out this structure
struct HashType
{
    struct Node **array;
    int size;
};

// Compute the hash function
int hash(struct RecordType data, int size)
{
    unsigned int hashVal = 31; 
    hashVal = hashVal * 17 + data.id;
    hashVal = hashVal * 17 + data.name;
    hashVal = hashVal * 17 + data.order;
    return hashVal % size;
}


// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray)
{
    int i;
    for (i = 0; i < pHashArray->size; ++i)
    {
        if (pHashArray->array[i] == NULL)
            continue;

        printf("Index %d:", i);
        struct Node *current = pHashArray->array[i];
        while (current != NULL)
        {
            printf(" -> %d, %c, %d", current->data.id, current->data.name, current->data.order);
            current = current->next;
        }
        printf("\n");
    }
}

void insert(struct HashType *hashTable, struct RecordType data)
{
    int index = hash(data, hashTable->size);

    // Check if there's an existing entry at the index
    struct Node *current = hashTable->array[index];
    while (current != NULL)
    {
        if (current->data.id == data.id)
        {
            // Update the existing entry and return
            current->data = data;
            return;
        }
        current = current->next;
    }

    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));

    newNode->data = data;
    newNode->next = hashTable->array[index];
    hashTable->array[index] = newNode;
}

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Your hash implementation
    int hashSize = 15; 
    struct HashType hashTable;
    hashTable.size = hashSize;
    hashTable.array = (struct Node **)malloc(hashSize * sizeof(struct Node *));

    // Initialize the hash table with NULL values
    for (int i = 0; i < hashSize; i++)
    {
        hashTable.array[i] = NULL;
    }

    // Inserting records into the hash table
    for (int i = 0; i < recordSz; i++)
    {
        insert(&hashTable, pRecords[i]);
    }

    displayRecordsInHash(&hashTable);

    // Freeing allocated memory for the hash table
    for (int i = 0; i < hashSize; i++)
    {
        struct Node *current = hashTable.array[i];
        while (current != NULL)
        {
            struct Node *temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(hashTable.array);

    return 0;
}
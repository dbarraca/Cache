#include <stdio.h>
#include <stdlib.h>

/*	Cache emulation - statistics generation */
/*	Generated for CSC 315 Lab 5 */
#define INDEX_MASK 0x

typedef struct LookupTable {
    int size;
    int associativity;
    int **addressTable;
} LookupTable;

typedef struct Cache {
    int hits;
    int misses;
    int reads;
    int writes;
    int indices;
    LookupTable *table;
} Cache;

Cache *CacheCreate(int size, int associativity) {
    LookupTable *newTable;
    int index;
    int totalIndices = size / associativity;
    Cache *newCache = calloc(1, sizeof(Cache));

    newCache->hits = 0;
    newCache->misses = 0;
    newCache->reads = 0;
    newCache->writes = 0;
    newCache->indices = totalIndices;
    newCache->table = newTable = calloc(1, sizeof(LookupTable));
    newTable->size = size;
    newTable->associativity = associativity;
    newTable->addressTable = calloc(totalIndices, sizeof(int *));

    // Initialize table's addressTable
    for (index = 0; index < associativity; index++) {
        newTable->addressTable[index] = calloc(1, sizeof(int));
    }

    return newCache;
}

// Returns 1 if hit, 0 if miss
int CacheLookup(Cache *cache, int address) {
    int indexMask = cache->indices << 2, tagMask = 0xFFFFFFFF << (2 + cache->indices);
    int index, associativeNdx;
    LookupTable *lookup = cache->table;

    for (associativeNdx = 0, index = address & indexMask; 
        associativeNdx < lookup->associativity; 
        associativeNdx++) {
        if (lookup->addressTable[index][associativeNdx] & tagMask == address & tagMask) {
            return 1;
        }
    }

    return 0;
}


void mem_read(int *mp, Cache *cache) {
    int indexMask = cache->indices << 2;

    printf("Memory read from location %p\n", mp);
    cache->reads++;

    if (CacheLookup(cache, (int)mp)) {
        cache->hits++;
    }
}

void mem_write(int *mp, Cache *cache) {
    printf("Memory write to location %p\n", mp);

}



int main()
{
    int a[10][10], b[10][10], mult[10][10], r1, c1, r2, c2, i, j, k;

    int *mp1, *mp2, *mp3;

    // Initialize caches
    Cache *cache_16_1 = CacheCreate(16, 1);
    Cache *cache_16_2 = CacheCreate(16, 2);
    Cache *cache_16_4 = CacheCreate(16, 4);
    Cache *cache_256_1 = CacheCreate(256, 1);
    Cache *cache_256_2 = CacheCreate(256, 2);
    Cache *cache_256_4 = CacheCreate(256, 4);

    printf("Size of pointer is: %d\n\n", sizeof(mp1));

    printf("Enter rows and column for first matrix: ");
    scanf("%d%d", &r1, &c1);
    printf("Enter rows and column for second matrix: ");
    scanf("%d%d",&r2, &c2);

/* If colum of first matrix in not equal to row of second matrix, asking user to enter the size of matrix again. */
    while (c1!=r2)
    {
        printf("Error! column of first matrix not equal to row of second.\n");
        printf("Enter rows and column for first matrix: ");
        scanf("%d%d", &r1, &c1);
        printf("Enter rows and column for second matrix: ");
        scanf("%d%d",&r2, &c2);
    }

/* Storing elements of first matrix. */
    printf("\nEnter elements of matrix 1:\n");
    for(i=0; i<r1; ++i)
    for(j=0; j<c1; ++j)
    {
//        printf("Enter elements a%d%d: ",i+1,j+1);
//        scanf("%d",&a[i][j]);
	a[i][j] = i+j; // build sample data

    }

/* Storing elements of second matrix. */
    printf("\nEnter elements of matrix 2:\n");
    for(i=0; i<r2; ++i)
    for(j=0; j<c2; ++j)
    {
//        printf("Enter elements b%d%d: ",i+1,j+1);
//        scanf("%d",&b[i][j]);

         b[i][j] = 10 + i + j;
    }

/* Initializing elements of matrix mult to 0.*/
    for(i=0; i<r1; ++i)
    for(j=0; j<c2; ++j)
    {
       mult[i][j]=0;
    }

/* Multiplying matrix a and b and storing in array mult. */
    for(i=0; i<r1; ++i)
    for(j=0; j<c2; ++j)
    for(k=0; k<c1; ++k)
    {
  
        mp1 = &mult[i][j];
	mp2 = &a[i][k];
	mp3 = &b[k][j];   


	mem_read(mp1);
 	mem_read(mp2);
	mem_read(mp3);
        mult[i][j]+=a[i][k]*b[k][j];
	mem_write(mp1); 
   }

/* Displaying the multiplication of two matrix. */
    printf("\nOutput Matrix:\n");
    for(i=0; i<r1; ++i)
    for(j=0; j<c2; ++j)
    {
        printf("%d  ",mult[i][j]);
        if(j==c2-1)
            printf("\n\n");
    }
    return 0;
}

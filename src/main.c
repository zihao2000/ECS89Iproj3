#include <stdio.h> 	 	    		
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include "KeyValueMap.h"

char **LoadKVPairs(const char *filename);
bool RunTests(SKeyValueMapRef kvmap, char **kvpairs, int kvpaircount, int findtests);
void InsertKVPairs(SKeyValueMapRef kvmap, char **kvpairs, int kvpaircount);
void InteractiveMenu(SKeyValueMapRef kvmap);

int main(int argc, char *argv[]){
    SKeyValueMapRef KVMap;
    char **KVPairs;
    bool DoTests = false;
    int KVPairCount = 0, FindTestSize = 100000;
    
    if(2 > argc){
        printf("Syntax Error: proj3 testfile [-t [finds]]\n");
        return EXIT_FAILURE;
    }
    if(3 <= argc){
        if(!strcmp(argv[2],"-t")){
            DoTests = true;
            if(4 <= argc){
                FindTestSize = atoi(argv[3]);
                if(FindTestSize < 100){
                    FindTestSize = 100;
                }
            }
        }
    }
    
    KVPairs = LoadKVPairs(argv[1]);
    if(!KVPairs){
        printf("Failed to load %s\n",argv[1]);
        return EXIT_FAILURE;
    }
    for(int Index = 0; KVPairs[Index]; Index += 2){
        KVPairCount++;
    }
    KVMap = KeyValueMapCreate();
    if(DoTests){
        if(!RunTests(KVMap,KVPairs,KVPairCount,FindTestSize)){
            printf("Tests failed\n");
        }
    }
    else{
        InsertKVPairs(KVMap,KVPairs,KVPairCount);
        printf("Successfully loaded %s\n",argv[1]);
        InteractiveMenu(KVMap);
    }
    KeyValueMapDestroy(KVMap);
    free(KVPairs);


    return EXIT_SUCCESS;
}

char **LoadKVPairs(const char *filename){
    FILE *FP = fopen(filename,"r");
    size_t FileSize;
    char **KVPairs;
    char *Buffer;
    char *WordAnchor;
    int Index = 0;

    if(!FP){
        return NULL;
    }
    fseek(FP,0,SEEK_END);
    FileSize = ftell(FP);
    if(!FileSize){
        return NULL;
    }
    fseek(FP,0,SEEK_SET);
    KVPairs = malloc((sizeof(char *) + sizeof(char)) * (FileSize + 1)); 
    Buffer = (char *)(KVPairs + FileSize + 1);
    fread(Buffer,sizeof(char),FileSize,FP);
    Buffer[FileSize] = '\0';
    while(*Buffer){
        WordAnchor = Buffer;
        while(*Buffer && '=' != *Buffer){
            Buffer++;
        }
        if('=' != *Buffer){
            break;
        }
        *Buffer = '\0';
        KVPairs[Index] = WordAnchor;
        Index++;
        Buffer++;
        WordAnchor = Buffer;
        while(*Buffer && '\n' != *Buffer){
            Buffer++;
        }
        if('\n' != *Buffer){
            break;
        }
        *Buffer = '\0';
        KVPairs[Index] = WordAnchor;
        Index++;
        Buffer++;
    }
    KVPairs[Index] = NULL;
    return KVPairs;
}

bool RunTests(SKeyValueMapRef kvmap, char **kvpairs, int kvpaircount, int findtests){
    SKeyValueMapIteratorRef Iterator, Next;
    int SmallInsertTestSize, SmallFindTestSize, KeyCount;
    clock_t StartTime, EndSmallTime, EndFullTime;
    double TimeSmall, TimeFull;
    double ConstantOverheadSmall, ConstantOverheadFull;
    double RatioInsertFind, RatioFind;

    SmallFindTestSize = findtests / 10;
    SmallInsertTestSize = kvpaircount / 10;
    StartTime = clock();
    for(int Index = 0; Index < SmallInsertTestSize; Index++){
        if(!KeyValueMapSet(kvmap,kvpairs[Index*2],kvpairs[Index*2+1])){
            printf("Failed to set \"%s\"\n",kvpairs[Index*2]);
        }
    }
    for(int Index = 0; Index < SmallInsertTestSize; Index++){
        if(strcmp(KeyValueMapGet(kvmap,kvpairs[Index*2]),kvpairs[Index*2+1])){
            printf("Lookup of %s was incorrect\n",kvpairs[Index*2]);
            return false;
        }
    }
    EndSmallTime = clock();
    for(int Index = SmallInsertTestSize; Index < kvpaircount; Index++){
        if(!KeyValueMapSet(kvmap,kvpairs[Index*2],kvpairs[Index*2+1])){
            printf("Failed to set \"%s\"\n",kvpairs[Index*2]);
        }
    }
    for(int Index = SmallInsertTestSize; Index < kvpaircount; Index++){
        if(strcmp(KeyValueMapGet(kvmap,kvpairs[Index*2]),kvpairs[Index*2+1])){
            printf("Lookup of %s was incorrect\n",kvpairs[Index*2]);
            return false;
        }
    }
    EndFullTime = clock();
    TimeSmall = (double)(EndSmallTime - StartTime)/CLOCKS_PER_SEC;
    TimeFull = (double)(EndFullTime - StartTime)/CLOCKS_PER_SEC;
    ConstantOverheadSmall = TimeSmall / (log2(SmallInsertTestSize) * SmallInsertTestSize);
    ConstantOverheadFull = TimeFull / (log2(kvpaircount) * kvpaircount);
    RatioInsertFind = fabs(ConstantOverheadFull/ConstantOverheadSmall);

    printf("Small Insert Time: %lf\n",TimeSmall);
    printf("Full Insert Time : %lf\n",TimeFull);
    printf("Constant Ratios  : %lg\n",RatioInsertFind);

    srand(0x12345678);
    StartTime = clock();
    for(int Index = 0; Index < SmallFindTestSize; Index++){
        int RandIndex = rand() % kvpaircount;
        if(strcmp(KeyValueMapGet(kvmap,kvpairs[RandIndex*2]),kvpairs[RandIndex*2+1])){
            printf("Lookup of %s was incorrect\n",kvpairs[RandIndex*2]);
            return false;
        }
    }
    EndSmallTime = clock();
    for(int Index = SmallFindTestSize; Index < findtests; Index++){
        int RandIndex = rand() % kvpaircount;
        if(strcmp(KeyValueMapGet(kvmap,kvpairs[RandIndex*2]),kvpairs[RandIndex*2+1])){
            printf("Lookup of %s was incorrect\n",kvpairs[RandIndex*2]);
            return false;
        }
    }
    EndFullTime = clock();
    TimeSmall = (double)(EndSmallTime - StartTime)/CLOCKS_PER_SEC;
    TimeFull = (double)(EndFullTime - StartTime)/CLOCKS_PER_SEC;
    ConstantOverheadSmall = TimeSmall / (log2(kvpaircount) * SmallFindTestSize);
    ConstantOverheadFull = TimeFull / (log2(kvpaircount) * findtests);
    RatioFind = fabs(ConstantOverheadFull/ConstantOverheadSmall);
    printf("Small Find Time  : %lf\n",TimeSmall);
    printf("Full Find Time   : %lf\n",TimeFull);
    printf("Constant Ratios  : %lg\n",RatioFind);

    KeyCount = 0;
    Iterator = KeyValueMapBegin(kvmap);
    while(Iterator && Iterator != KeyValueMapEnd(kvmap)){
        Next = KeyValueMapIteratorNext(Iterator);
        if(Next != KeyValueMapEnd(kvmap)){
            if(strcmp(KeyValueMapIteratorKey(Iterator), KeyValueMapIteratorKey(Next)) > 0){
                printf("Key %s out of order with %s\n", KeyValueMapIteratorKey(Iterator), KeyValueMapIteratorKey(Next));
                return false;
            }
        }
        KeyCount++;
        Iterator = Next;
    }
    if(KeyCount != kvpaircount){
        printf("Number of key mismatched %d expected %d\n",KeyCount,kvpaircount);
        return false;
    }
    printf("Forward Keys     : %d\n",KeyCount);
    Iterator = KeyValueMapEnd(kvmap);
    Iterator = KeyValueMapIteratorPrevious(Iterator);
    if(!Iterator){
        printf("Empty KeyValueMap\n");
        return false;
    }
    KeyCount = 1;
    do{
        Next = KeyValueMapIteratorPrevious(Iterator);
        if(Next){
            if(strcmp(KeyValueMapIteratorKey(Iterator), KeyValueMapIteratorKey(Next)) < 0){
                printf("Key %s out of order with %s\n", KeyValueMapIteratorKey(Iterator), KeyValueMapIteratorKey(Next));
                return false;
            }
        }
        KeyCount++;
        Iterator = Next;
    }while(Iterator && Iterator != KeyValueMapBegin(kvmap));
    if(KeyCount != kvpaircount){
        printf("Number of key mismatched %d expected %d\n",KeyCount,kvpaircount);
        return false;
    }
    printf("Reverse Keys     : %d\n",KeyCount);
    printf("Total Keys       : %d\n",kvpaircount);

    return true;
}

void ListKeys(SKeyValueMapRef kvmap){
    SKeyValueMapIteratorRef Iterator;
    printf("Listing keys\n");
    Iterator = KeyValueMapBegin(kvmap);
    while(Iterator && (Iterator != KeyValueMapEnd(kvmap))){
        printf("%s\n",KeyValueMapIteratorKey(Iterator));
        Iterator = KeyValueMapIteratorNext(Iterator);
    }
}

void FindValue(SKeyValueMapRef kvmap){
    char Buffer[1024];
    const char *Value;
    int Index;
    printf("Enter Key> ");
    fgets(Buffer,sizeof(Buffer),stdin);
    Index = strlen(Buffer);
    while(Index){
        Index--;
        if(!isspace(Buffer[Index])){
            break;
        }
        Buffer[Index] = '\0';
    }
    Index = 0;
    while(isspace(Buffer[Index])){
        Index++;
    }
    Value = KeyValueMapGet(kvmap,Buffer + Index);
    if(!Value){
        printf("Failed to Find Key %s\n",Buffer+Index);
        return;
    }
    printf("%s = %s\n",Buffer+Index,Value);
}

void SetValue(SKeyValueMapRef kvmap){
    char KeyBuffer[1024];
    char ValueBuffer[1024];
    char *Key;
    char *Value;
    int Index;
    printf("Enter Key> ");
    fgets(KeyBuffer,sizeof(KeyBuffer),stdin);
    Index = strlen(KeyBuffer);
    while(Index){
        Index--;
        if(!isspace(KeyBuffer[Index])){
            break;
        }
        KeyBuffer[Index] = '\0';
    }
    Index = 0;
    while(isspace(KeyBuffer[Index])){
        Index++;
    }
    Key = KeyBuffer + Index;
    printf("Enter Value> ");
    fgets(ValueBuffer,sizeof(ValueBuffer),stdin);
    Index = strlen(ValueBuffer);
    while(Index){
        Index--;
        if(!isspace(ValueBuffer[Index])){
            break;
        }
        ValueBuffer[Index] = '\0';
    }
    Index = 0;
    while(isspace(ValueBuffer[Index])){
        Index++;
    }
    Value = ValueBuffer + Index;
    if(!strlen(Key) || !strlen(Value)){
        printf("Invalid Key/Value Pair\n");
        return;
    }
    if(!KeyValueMapSet(kvmap,Key,Value)){
        printf("Failed to Set Key/Value Pair\n");
        return;
    }
    printf("Successfully Set %s to %s\n", Key, Value);
}

void DeleteKey(SKeyValueMapRef kvmap){
    char Buffer[1024];
    int Index;
    printf("Enter Key> ");
    fgets(Buffer,sizeof(Buffer),stdin);
    Index = strlen(Buffer);
    while(Index){
        Index--;
        if(!isspace(Buffer[Index])){
            break;
        }
        Buffer[Index] = '\0';
    }
    Index = 0;
    while(isspace(Buffer[Index])){
        Index++;
    }
    if(!KeyValueMapDelete(kvmap,Buffer+Index)){
        printf("Failed to Delete Key\n");
        return;
    }
    printf("Successfully Deleted Key %s\n",Buffer+Index);
}

void InsertKVPairs(SKeyValueMapRef kvmap, char **kvpairs, int kvpaircount){
    for(int Index = 0; Index < kvpaircount; Index++){
        KeyValueMapSet(kvmap,kvpairs[Index*2],kvpairs[Index*2+1]);
    }
}

void InteractiveMenu(SKeyValueMapRef kvmap){
    bool Done = false;
    char Buffer[1024];
    do{
        printf("\n---Key Value Map Program---\n");
        printf("   L - List Keys\n");
        printf("   F - Find Value\n");
        printf("   S - Set Value\n");
        printf("   D - Delete Key\n");
        printf("   --------------\n");
        printf("   Q - Quit\n");
        printf("---------------------------\n");
        printf("> ");
        fgets(Buffer,sizeof(Buffer),stdin);
        switch(toupper(Buffer[0])){
            case 'L':   ListKeys(kvmap);
                        break;
            case 'F':   FindValue(kvmap);
                        break;
            case 'S':   SetValue(kvmap);
                        break;
            case 'D':   DeleteKey(kvmap);
                        break;
            case 'Q':   Done = true;
                        break;
            default:    printf("Unknown command %c\n",toupper(Buffer[0]));
        }
    }while(!Done);
}

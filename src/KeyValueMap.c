#include "KeyValueMap.h" 	 	    		
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Make the iterator pointer the same as node */
struct STRUCT_KEY_VALUE_MAP_ITERATOR_TAG{
    SKeyValueMapRef DKVMap;
    size_t DTreeHeight;
    char *DKey;
    char *DValue;
    SKeyValueMapIteratorRef DParent;
    SKeyValueMapIteratorRef DLeft;
    SKeyValueMapIteratorRef DRight;
};

struct STRUCT_KEY_VALUE_MAP_TAG{
    SKeyValueMapIteratorRef DRoot;
    SKeyValueMapIterator DEnd;
    size_t DSize;
};

SKeyValueMapIteratorRef KVMapNodeCreate(SKeyValueMapRef kvmap){
    SKeyValueMapIteratorRef Node = malloc(sizeof(SKeyValueMapIterator));
    memset(Node,0,sizeof(SKeyValueMapIterator));
    Node->DKVMap = kvmap;
    return Node;
}

void KVMapNodePrint(SKeyValueMapIteratorRef node){
    if(node->DLeft){
        printf("/");
        KVMapNodePrint(node->DLeft);
    }
    printf("%s\n",node->DKey);
    if(node->DRight){
        printf("\\");
        KVMapNodePrint(node->DRight);
    }

}

void KVMapNodeDelete(SKeyValueMapIteratorRef node){
    if(!node){
        return;
    }
    if(node->DLeft){
        KVMapNodeDelete(node->DLeft);        
    }
    if(node->DRight){
        KVMapNodeDelete(node->DRight);        
    }
    if(node->DKey){
        free(node->DKey);
    }
    if(node->DValue){
        free(node->DValue);
    }
    free(node);
}

size_t KVMapNodeHeight(SKeyValueMapIteratorRef node){
    if(!node){
        return 0;
    }
    return node->DTreeHeight;
}

void KVMapNodeUpdateHeight(SKeyValueMapIteratorRef node){
    if(node){
        int LHeight, RHeight;
        LHeight = KVMapNodeHeight(node->DLeft);
        RHeight = KVMapNodeHeight(node->DRight);
        node->DTreeHeight = LHeight > RHeight ? LHeight + 1 : RHeight + 1;    
    }
}

SKeyValueMapIteratorRef KVMapNodeLeftMost(SKeyValueMapIteratorRef node){
    if(!node){
        return NULL;
    }
    if(node->DLeft){
        return KVMapNodeLeftMost(node->DLeft);
    }
    return node;
}

SKeyValueMapIteratorRef KVMapNodeRightMost(SKeyValueMapIteratorRef node){
    if(!node){
        return NULL;
    }
    if(node->DRight){
        return KVMapNodeRightMost(node->DRight);
    }
    return node;
}

SKeyValueMapIteratorRef KVMapNodeFind(SKeyValueMapIteratorRef node, const char *key){
    int Compare;
    if(!node || !key){
        return NULL;
    }
    Compare = strcmp(node->DKey,key);
    if(Compare == 0){
        return node;
    }else if(0 < Compare){
        return KVMapNodeFind(node->DRight,key);
    }else{
        return KVMapNodeFind(node->DLeft,key);
    }
}

SKeyValueMapIteratorRef KVMapNodeBalance(SKeyValueMapIteratorRef node){
    SKeyValueMapIteratorRef Parent, Left, Right, Return;
    int LHeight, RHeight;
    Parent = node->DParent;
    Left = node->DLeft;
    Right = node->DRight;
    LHeight = KVMapNodeHeight(Left);
    RHeight = KVMapNodeHeight(Right);
    if(LHeight > RHeight){
        if(KVMapNodeHeight(Left->DLeft) > KVMapNodeHeight(Left->DRight)){
            node->DLeft = Left->DRight;
            if(node->DLeft){
                node->DLeft->DParent = node;
            }
            KVMapNodeUpdateHeight(node);
            Left->DRight = node;
            node->DParent = Left;
            Left->DParent = Parent;
            KVMapNodeUpdateHeight(Left);
            if(Parent){
                if(Parent->DLeft == node){
                    Parent->DLeft = Left;
                }
                else{
                    Parent->DRight = Left;
                }
            }
            else{
                Left->DKVMap->DRoot = Left;
            }
            Return = Left;
        }
        else{
            SKeyValueMapIteratorRef NewNode = Left->DRight;
            Left->DRight = NewNode->DLeft;
            if(Left->DRight){
                Left->DRight->DParent = Left;
            }
            KVMapNodeUpdateHeight(Left);
            NewNode->DLeft = Left;
            Left->DParent = NewNode;
            node->DLeft = NewNode->DRight;
            if(node->DLeft){
                node->DLeft->DParent = node;
            }
            KVMapNodeUpdateHeight(node);
            NewNode->DRight = node;
            node->DParent = NewNode;
            NewNode->DParent = Parent;
            KVMapNodeUpdateHeight(NewNode);
            if(Parent){
                if(Parent->DLeft == node){
                    Parent->DLeft = NewNode;
                }
                else{
                    Parent->DRight = NewNode;
                }
            }
            else{
                NewNode->DKVMap->DRoot = NewNode;
            }
            Return = NewNode;
        }
    }
    else{
        if(KVMapNodeHeight(Right->DRight) > KVMapNodeHeight(Right->DLeft)){
            node->DRight = Right->DLeft;
            if(node->DRight){
                node->DRight->DParent = node;
            }
            KVMapNodeUpdateHeight(node);
            Right->DLeft = node;
            node->DParent = Right;
            Right->DParent = Parent;

            KVMapNodeUpdateHeight(Right);
            if(Parent){
                if(Parent->DLeft == node){
                    Parent->DLeft = Right;
                }
                else{
                    Parent->DRight = Right;
                }
            }
            else{
                Left->DKVMap->DRoot = Right;
            }
            Return = Right;
        }
        else{
            SKeyValueMapIteratorRef NewNode = Right->DLeft;
            Right->DLeft = NewNode->DRight;
            if(Right->DLeft){
                Right->DLeft->DParent = Right;
            }
            KVMapNodeUpdateHeight(Right);
            NewNode->DRight = Right;
            Right->DParent = NewNode;
            node->DRight = NewNode->DLeft;
            if(node->DRight){
                node->DRight->DParent = node;
            }
            KVMapNodeUpdateHeight(node);
            NewNode->DLeft = node;
            node->DParent = NewNode;
            NewNode->DParent = Parent;
            KVMapNodeUpdateHeight(NewNode);
            if(Parent){
                if(Parent->DLeft == node){
                    Parent->DLeft = NewNode;
                }
                else{
                    Parent->DRight = NewNode;
                }
            }
            else{
                NewNode->DKVMap->DRoot = NewNode;
            }
            Return = NewNode;
        }
    }
    return Return;
}

void KVMapNodeSet(SKeyValueMapRef kvmap, SKeyValueMapIteratorRef node, const char *key, const char *value){
    SKeyValueMapIteratorRef NewNode;
    int Compare, LHeight, RHeight;
    if(!node){ 
        node = KVMapNodeCreate(kvmap);
        node->DKey = strcpy(malloc(strlen(key) + 1), key);
        node->DValue = strcpy(malloc(strlen(value) + 1), value);
        kvmap->DRoot = node;
        kvmap->DSize++;
        return;
    }
    Compare = strcmp(node->DKey,key);
    if(!Compare){
        free(node->DValue);
        node->DValue = strcpy(malloc(strlen(value) + 1), value);
        return;
    }
    else if(0 > Compare){
        if(node->DLeft){
            KVMapNodeSet(kvmap,node->DLeft,key,value);
        }
        else{
            node->DLeft = NewNode = KVMapNodeCreate(kvmap);
            NewNode->DKey = strcpy(malloc(strlen(key) + 1), key);
            NewNode->DValue = strcpy(malloc(strlen(value) + 1), value);
            NewNode->DParent = node;
            kvmap->DSize++;
        }
    }
    else{
        if(node->DRight){
            KVMapNodeSet(kvmap,node->DRight,key,value);
        }
        else{
            node->DRight = NewNode = KVMapNodeCreate(kvmap);
            NewNode->DKey = strcpy(malloc(strlen(key) + 1), key);
            NewNode->DValue = strcpy(malloc(strlen(value) + 1), value);
            NewNode->DParent = node;
            kvmap->DSize++;
        }
    }
    LHeight = KVMapNodeHeight(node->DLeft);
    RHeight = KVMapNodeHeight(node->DRight);
    node->DTreeHeight = LHeight > RHeight ? LHeight + 1 : RHeight + 1;
    Compare = LHeight - RHeight;
    if((-1 > Compare)||(1 < Compare)){
        KVMapNodeBalance(node);
    }
}

SKeyValueMapRef KeyValueMapCreate(){
    SKeyValueMapRef KVMap = malloc(sizeof(SKeyValueMap));
    KVMap->DRoot = NULL;
    memset(&KVMap->DEnd,0,sizeof(SKeyValueMapIterator));
    KVMap->DEnd.DKVMap = KVMap;
    KVMap->DSize = 0;
    return KVMap;
}

bool KeyValueMapDestroy(SKeyValueMapRef kvmap){
    if(! kvmap)
        return false;
    KVMapNodeDelete(kvmap->DRoot);
    free(kvmap);
    return true;
}

size_t KeyValueMapSize(SConstKeyValueMapRef kvmap){
    if(! kvmap)
        return 0;
    return kvmap->DSize;
}

const char *KeyValueMapGet(SConstKeyValueMapRef kvmap, const char *key){
    SKeyValueMapIteratorRef Node = KeyValueMapFind(kvmap, key);
    if(!Node){
        return 0;
    }
    return Node->DValue;
}

bool KeyValueMapSet(SKeyValueMapRef kvmap, const char *key, const char *value){
    //Added corner cases against bad input
    if(!kvmap || !key || !value)
        return false;
    KVMapNodeSet(kvmap, kvmap->DRoot, key, value);
    return true;
}

bool KeyValueMapDelete(SKeyValueMapRef kvmap, const char *key){
    SKeyValueMapIteratorRef Node = KeyValueMapFind(kvmap, key);
    if(Node){
        KeyValueMapIteratorDelete(Node);
        return true;
    }else{
        return false;
    }
}

SKeyValueMapIteratorRef KeyValueMapBegin(SConstKeyValueMapRef kvmap){
    if(!kvmap){
        return 0;
    }
    if(kvmap->DRoot){
        return KVMapNodeLeftMost(kvmap->DRoot);
    }
    //changed ->DEnd to DRoot
    return &((SKeyValueMapRef)kvmap)->DEnd;
}

SKeyValueMapIteratorRef KeyValueMapEnd(SConstKeyValueMapRef kvmap){
    //added test kvmap null
    if(!kvmap){
        return 0;
    }
    return &((SKeyValueMapRef)kvmap)->DEnd;
}

SKeyValueMapIteratorRef KeyValueMapFind(SConstKeyValueMapRef kvmap, const char *key){
    if(!kvmap){
        return 0;
    }
    if(!key){
        return 0;
    }
    return KVMapNodeFind(kvmap->DRoot,key);
}

SKeyValueMapIteratorRef KeyValueMapIteratorNext(SKeyValueMapIteratorRef iterref){
    if(!iterref){
        return 0;
    }
    if(&iterref->DKVMap->DEnd == iterref){
        return 0;
    }
    if(iterref->DRight){
        return KVMapNodeLeftMost(iterref->DRight);
    }
    while(iterref->DParent){
        if(iterref == iterref->DParent->DLeft){
            return iterref->DParent;
        }
        iterref = iterref->DParent;
    }
    return &iterref->DKVMap->DEnd;
}

SKeyValueMapIteratorRef KeyValueMapIteratorPrevious(SKeyValueMapIteratorRef iterref){
    if(!iterref){
        return NULL;
    }
    if(&iterref->DKVMap->DEnd == iterref){
        return KVMapNodeRightMost(iterref->DKVMap->DRoot);
    }
    if(iterref->DLeft){
        return KVMapNodeRightMost(iterref->DLeft);
    }
    while(iterref->DParent){
        if(iterref == iterref->DParent->DRight){
            return iterref->DParent;
        }
        iterref = iterref->DParent;
    }
    return NULL;
}

const char *KeyValueMapIteratorKey(SKeyValueMapIteratorRef iterref){
    if(!iterref){
        return 0;
    }
    return iterref->DKey;
}

const char *KeyValueMapIteratorValue(SKeyValueMapIteratorRef iterref){
    if(!iterref){
        return 0;
    }
    return iterref->DValue;
}

bool KeyValueMapIteratorSet(SKeyValueMapIteratorRef iterref, const char *value){
    if(!iterref || !value){
        return false;
    }
    free(iterref->DValue);
    iterref->DValue = strcpy(malloc(strlen(value) + 1), value);
    return true;
}

SKeyValueMapIteratorRef KeyValueMapIteratorDelete(SKeyValueMapIteratorRef iterref){
    SKeyValueMapIteratorRef Successor, Parent;
    if(!iterref){
        return NULL;
    }
    if(&iterref->DKVMap->DEnd == iterref){
        return NULL;
    }
    Parent = iterref->DParent;
    Successor = KeyValueMapIteratorNext(iterref);
    if(!iterref->DLeft){
        if(!iterref->DRight){
            if(Parent){
                if(Parent->DLeft == iterref){
                    Parent->DLeft = NULL;
                }
                else{
                    Parent->DRight = NULL;
                }
            }
            else{
                iterref->DKVMap->DRoot = NULL;
            }
        }
        else{
            if(Parent){
                if(Parent->DLeft == iterref){
                    Parent->DLeft = iterref->DRight;
                }
                else{
                    Parent->DRight = iterref->DRight;
                }
            }
            else{
                iterref->DKVMap->DRoot = iterref->DRight;
            }
        }
    }
    else{
        if(!iterref->DRight){
            if(Parent){
                if(Parent->DLeft == iterref){
                    Parent->DLeft = iterref->DLeft;
                }
                else{
                    Parent->DRight = iterref->DLeft;
                }
            }
            else{
                iterref->DKVMap->DRoot = iterref->DLeft;
            }
        }
        else{
            if(iterref->DRight == Successor){
                Successor->DLeft = iterref->DLeft;
                Successor->DLeft->DParent = Successor;
                Successor->DParent = Parent;
                if(Parent){
                    if(Parent->DLeft == iterref){
                        Parent->DLeft = Successor;
                    }
                    else{
                        Parent->DRight = Successor;
                    }
                }
                else{
                    iterref->DKVMap->DRoot = Successor;
                }
                Parent = Successor;
            }
            else{
                Successor->DLeft = iterref->DLeft;
                Successor->DLeft->DParent = Successor;
                Successor->DParent->DLeft = Successor->DRight;
                if(Successor->DRight){
                    Successor->DRight->DParent = Successor->DParent;
                }
                Successor->DRight = iterref->DRight;
                Successor->DRight->DParent = Successor;                
                iterref->DParent = Successor->DParent;
                if(Parent){
                    if(Parent->DLeft == iterref){
                        Parent->DLeft = Successor;
                    }
                    else{
                        Parent->DRight = Successor;
                    }
                }
                else{
                    iterref->DKVMap->DRoot = Successor;
                }
                Parent = iterref->DParent;
            }
        }
    }
    iterref->DKVMap->DSize--;
    if(!iterref->DKVMap->DSize){
        iterref->DKVMap->DRoot = NULL;
    }
    iterref->DLeft = NULL;
    iterref->DRight = NULL;
    KVMapNodeDelete(iterref);
    while(Parent){
        int LHeight, RHeight, Compare;
        LHeight = KVMapNodeHeight(Parent->DLeft);
        RHeight = KVMapNodeHeight(Parent->DRight);
        Parent->DTreeHeight = LHeight > RHeight ? LHeight + 1 : RHeight + 1;
        Compare = LHeight - RHeight;
        if((-1 > Compare)||(1 < Compare)){
            Parent = KVMapNodeBalance(Parent);
        }
        Parent = Parent->DParent;
    }
    

    return Successor;
}
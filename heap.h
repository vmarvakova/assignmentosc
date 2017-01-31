#ifndef HEAP_H
#define HEAP_H

#include <cstddef>
#include <new>

#include <iostream>
using std::cout;
using std::endl;

class MemControlBlock {
  
public:
    
    /** @brief True if the MemControlBlock is before a block of available memory */
    bool available;
    
    /** @brief The size of the following block of memory, in bytes */
    size_t size;
    
    MemControlBlock * previous;
    MemControlBlock * next;
    
    MemControlBlock(const bool availableIn, const int sizeIn)
        : available(availableIn), size(sizeIn),
          previous(nullptr), next(nullptr)
    {
    }
    
};

class Heap {

private:
    char * const memory;
    
    /** @brief The first MemControlBlock for the heap -- the head of the list */
    MemControlBlock * startOfHeap;
    
public:
    
    ~Heap() {
        delete [] memory;
    }
    
    // no copy constructor, move constructor, assignment operator, ...
    Heap(const Heap &) = delete;
    Heap(Heap &&) = delete;    
    Heap & operator=(const Heap &) = delete;
    Heap & operator=(Heap &&) = delete;
    
    Heap(const size_t sizeIn)
        : memory(new char[sizeIn]) {
            
        // make a MemControlBlock at the start of the reserved memory
        startOfHeap = new(memory) MemControlBlock(// true = is available
                                                  true, 
                                                  // size = the size of the heap - the size of the MemControlBlock a the start of the heap
                                                  sizeIn - sizeof(MemControlBlock)
                                                 );
    }

    /** @brief Used for debugging - get the address of the start of the heap */
    char* getStartOfHeap() const {
        return memory;
    }
    
    /** @brief Used for debugging -- print out the details of the MemControlBlocks on the heap */
    void print() const {
        MemControlBlock * curr = startOfHeap;
        
        for (int i = 0; curr; ++i, curr = curr->next) {
            cout << "Block " << i << ": ";
            if (curr->available) {
                cout << " free, ";
            } else {
                cout << " in use, ";
            }
            cout << curr->size << " bytes\n";
        }
            
    }
    
    
    
    /** @brief Request a block of memory of the given size
     * 
     * Uses the 'Best Fit' algorithm to choose a suitable block of available memory
     *
     * Returns the memory address of the start of the requested memory
     * If no block is big enough, it returns nullptr.
     */
    char * allocateMemoryBestFit(size_t requested) {
        // TODO: your code for allocateMemory memory goes here
        
             MemControlBlock * start = startOfHeap;
        size_t sizeMem = sizeof(MemControlBlock);
        
        if(requested%4!=0){
            requested=requested+(4-requested%4);
        }
        
        
        MemControlBlock * blockN;
        int m = 0;
        
    
        while(start!=nullptr){
            if (start->available==true){
                
        if(m==0)  {
                m = start->size;
                blockN=start;
            }
        if(start->available==true && sizeof(start)>=requested && sizeof(start)<m){
                m = sizeof(start);
                    }
            }
            start=start->next;
        }
        
        blockN->available=false;

        char * newChar = nullptr;
        
        while (blockN!=nullptr){
            char * add = reinterpret_cast<char *> (blockN);

            if(sizeof(blockN) - requested - sizeMem > 0){
                
                add=add+ sizeof(MemControlBlock)+requested;
                MemControlBlock * newR = new (add) MemControlBlock(true,sizeof(blockN) - requested - sizeMem-16);
  
                newR->next=blockN->next;
                blockN->next=newR;
                              newR->previous=blockN;
                
             newChar  = reinterpret_cast<char *> (blockN)+sizeMem;

            }
            
            return newChar;
        }
        
        return nullptr;
    }
    
    /** @brief Deallocate the memory used by the object at the given address */
    void deallocateMemory(char * toDeallocate) {
        // TODO: your code for deallocateMemory memory goes here
        
        MemControlBlock * curr = startOfHeap;
        char * d = reinterpret_cast<char * >(toDeallocate)-sizeof(MemControlBlock);
        
        while(curr){
            char * c = reinterpret_cast<char * >(curr);
            
            if(c==d) {
                curr->available=true;
                
                if(curr->previous){
                    
                    if(curr->previous->available==true){
                        curr->previous->size = curr->previous->size+curr->size;
                        curr->previous->next = curr->next;
                    }
                }
                
                if(curr->next){
                    if(curr->next->available==true){
                        curr->previous->size = curr->previous->size+curr->next->size;
                        if(curr->next->next){
                            curr->previous->next = curr->next->next;
                            curr->next->next->previous=curr->previous;
                        }
                        else {
                            curr->previous->next=nullptr;
                        }
                        
                    }
                }
            }
            
            curr=curr->next;
            
        }
    }

};

#endif

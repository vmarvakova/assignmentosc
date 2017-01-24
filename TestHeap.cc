#include "heap.h"

#include <iostream>
using std::cout;
using std::endl;

#include <vector>
using std::vector;

class TestObject {

public:
    
    int x;
    int y;
    
    TestObject() : x(0), y(1) {}
    
};

int main() {
    
    
    Heap heap(1048576);
    
    vector<char*> created(4);
    
    {
        cout << "Test 1: making space for a TestObject\n";
        
        char * addrOne = heap.allocateMemoryBestFit(sizeof(TestObject));
        
        if (addrOne == nullptr) {
            cout << " - Failed: returned nullptr\n";
            cout << "  * heap.print() gives\n";
            heap.print();
        } else {
            TestObject * objOne = new(addrOne) TestObject();            
            char * shouldBe = heap.getStartOfHeap() + sizeof(MemControlBlock);
            if (shouldBe == addrOne) {
                cout << " - Passed: object created at the start of the heap, immediately after the MemControlBlock\n";
            } else {
                cout << " - Failed: address returned should have been " << sizeof(MemControlBlock) << " bytes after the start of the heap, but was ";
                
                cout << static_cast<size_t>(addrOne - heap.getStartOfHeap());
                cout << " bytes after it\n";
                cout << "  * heap.print() gives\n";
                heap.print();
            }
        }

        
        created[0] = addrOne;
    }
    
    for (int i = 1; i < 4; ++i) {
        cout << "\nTest " << i + 1 << ": making space for another TestObject\n";
        
        char * addrTwo = heap.allocateMemoryBestFit(sizeof(TestObject));
        TestObject * objTwo = new(addrTwo) TestObject();
        
        created[i] = addrTwo;
        
        if (objTwo == nullptr) {
            cout << " - Failed: returned nullptr\n";
            cout << "  * heap.print() gives\n";
            heap.print();
        } else {
            char * shouldBe = created[i - 1] + sizeof(TestObject) + sizeof(MemControlBlock);
            if (shouldBe == addrTwo) {
                cout << " - Passed: object created after the MemControlBlock after test object number " << i << endl;
            } else {
                cout << " - Failed: address returned should have been " << sizeof(MemControlBlock) << " bytes after the start of the heap, but was ";
                
                cout << static_cast<size_t>(addrTwo - heap.getStartOfHeap());
                cout << " bytes after it\n";
                cout << "  * heap.print() gives\n";
                heap.print();
            }
        }
    }

    cout << "\nDeallocating the first object (this will crash if it was null)\n";    
    heap.deallocateMemory(created[0]);
    cout << "  * heap.print() gives\n";
    heap.print();
    cout << "\nDeallocating the second object (this will crash if it was null)\n";    
    heap.deallocateMemory(created[1]);
    cout << "  * heap.print() gives\n";
    heap.print();
    
    cout << "\nTest 5 - Creating an object of size ";
    cout << 2 * sizeof(TestObject) << endl;
    cout << "(This should fit in the space of the first two, plus an MCB (i.e. the hole after freeing the first two allocations)\n\n";
    
    {
        char * addrOne = heap.allocateMemoryBestFit(2 * sizeof(TestObject) + sizeof(MemControlBlock));

        if (addrOne == nullptr) {
            cout << " - Failed: returned nullptr\n";
            cout << "  * heap.print() gives\n";
            heap.print();
        } else {
            char * shouldBe = heap.getStartOfHeap() + sizeof(MemControlBlock);
            if (shouldBe == addrOne) {
                cout << " - Passed: object created at the start of the heap, immediately after the MemControlBlock\n";
            } else {
                cout << " - Failed: address returned should have been " << sizeof(MemControlBlock) << " bytes after the start of the heap, but was ";
                
                cout << static_cast<size_t>(addrOne - heap.getStartOfHeap());
                cout << " bytes after it\n";
                cout << "  * heap.print() gives\n";
                heap.print();
            }
        }
        
    }
    
}

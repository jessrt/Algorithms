#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>

using namespace std;
ifstream inFile;
ofstream outFile1;
ofstream outFile2;


class listNode{

private:
    //variables needed for listNode

    int data;
    listNode *next;

public:
    //list node constructor

    listNode(){
        data = 0;
        next = NULL;
    }

    listNode(int data1){
        data = data1;
        next = NULL;
    }

    ~listNode(){
        delete next;
    }

    //declare friends

    friend class linkedListStack;
    friend class linkedListQueue;
    friend class RadixSort;
};

string inttoString(int d){
    ostringstream tempstr;
    tempstr << d;
    return tempstr.str();
}

int getMaxDigits(int pn){
    int maxDigits;
    string maxdig = inttoString(pn);
    maxDigits = maxdig.length();
    return maxDigits;
}


class linkedListStack{


public:
    listNode* top;

    //linkedListStack constructor
    linkedListStack(){
        top  = NULL;
    }

    //linkedListStack deconstructor
    ~linkedListStack(){
        delete top;
    }

    //declare friends

    friend class RadixSort;

    //push method
    void push(listNode* newNode){

        //The new node points to where last added node is
        //The top pointer now points to the new top.


            newNode->next = top;
            top = newNode;
    }

    listNode* pop(){
        listNode *temp = top;
        top = top->next;
        return temp;
    }

    bool isEmpty(){
        if (top-> next == NULL){
            return true;
        } else {return false;}
    }

    //we print stack only once at end when all data read to outputfile2
    void printStack(){
        string conclusion = "Top ->";
        listNode *temp;

        if (isEmpty()){
            return;
        }  else {
            temp = top;
            while ((temp->next != NULL)) {
                outFile2 << "(" + inttoString((temp->data)) + "," + inttoString((temp->next->data)) + ") ->";
                temp = temp->next;
            }
        }
                outFile2<< "(" << inttoString((temp->data)) << ", NULL) -> NULL";
				}

};


class linkedListQueue{
    //declare friends
    friend class RadixSort;

private:
    listNode *head;
    listNode *tail;

public:

    linkedListQueue(){
        listNode* dummy;
        head = dummy;
        tail = dummy;
    }

    void addTail(linkedListQueue *Q, listNode *node){
        if (head->next == NULL){
            head->next = node;
            tail = node;
        } else{
            tail->next = node;
            tail = node;
        }
    }

    listNode* deleteHead(){
            if(head->next == NULL){
                return NULL;
            } else {
                listNode *tem;
                tem = head->next;
                head->next = tem->next;
                tem->next = NULL;
                return tem;
            }
    }

    bool isEmpty(linkedListQueue *Q){
        if (head->next == NULL){
            return true;
        } else {return false;}
    }

    void printQueue(){
        listNode *temp;
        if(head->next == NULL){
            return;
        }
        else{
            temp = head->next;
            while (temp != NULL){
                outFile2<< "(" + inttoString((temp->data)) + "," + inttoString(((temp->next)->data)) + ") ->";
                temp = temp->next;
            }
            if (temp->next == NULL){
                outFile2<< "(" + inttoString((temp->data)) + "NULL) -> NULL" <<endl;
                outFile2<< "(" + inttoString((temp->data)) + "NULL) -> NULL";

            }
        }
    }
};

class RadixSort{

public:


    int tableSize = 10;
    int data;
    int currentTable;
    int previousTable;
    int maxDigits; 			//largest int length
    int offSet; 			//abs value of largest neg int; add before & subtract after
    int currentDigit;
    int currentQueue;
    int hashIndex;
    int temp;
    int digit;
    linkedListQueue *hashTable[2][10];


    RadixSort(){
        linkedListQueue* hashtable[2][tableSize];
        for(int i =0; i < 2;i++)
            for(int r = 0; r<tableSize; r++)
                hashtable[i][r] = new linkedListQueue();

    }

    int getdigit(listNode *n, int c){
        int d;
        n->data = d;
        int dig;
        if (inttoString(d).length() < c){
            dig = 0;
            return dig;
        }
        if (c ==1){
            dig = d%10;
        }else {
            while (c != 1){
                d = d/10;
                c--;
            }
            dig = d%10;
        }
        return dig;
    }


    void firstReading(){
        int negativeNum = 0;
        int positiveNum = 0;
        int data;

        if (!inFile){
            cout << "Can't open file";
            exit(1);
        }
        while (inFile >> data){
            if (data < negativeNum){
                negativeNum = data;
            }
            if (data > positiveNum){
                positiveNum = data;
            }
        }
        inFile.close();

        offSet = abs(negativeNum);
        positiveNum += offSet;
        maxDigits = getMaxDigits(positiveNum);
    }

    void loadStack(){
        linkedListStack *lls = new linkedListStack();

        while (inFile >> data){
            data += offSet;
            listNode *newNode = new listNode(data);
            lls->push(newNode);
        }
        inFile.close();
        outFile2 <<"*** Below is the constructed stack ***"<<endl;
        lls->printStack();
    }

    string printTable(){
        for (int i = 0; i < tableSize; i++)
            if (!(hashTable[currentDigit][i] ->isEmpty(hashTable[currentDigit][i]))) {

                outFile2<<"Table[" + inttoString(currentQueue) +"][" + inttoString(i) + "]:";
                hashTable[currentDigit][i]->printQueue();
            }

    }

    void dumpStack (){
        linkedListStack lls;
        int digit;
        int hashIndex;

        while(!(lls.isEmpty())){
            listNode *node = lls.pop();

            digit = getdigit(node, currentDigit);
            hashIndex = digit;
            hashTable[currentTable][hashIndex] -> addTail(hashTable[currentTable][hashIndex], node);
        }

        outFile2<<printTable();
    }


    void radixS(){
        listNode *node;
        linkedListQueue *hashTable[2][10];

        currentDigit = 0;
        currentTable = 0;
        dumpStack();
        currentTable = 1;
        previousTable = 0;

        while(currentDigit < maxDigits){
            while(!(currentQueue >= tableSize-1)){
                while (!(hashTable[previousTable][currentQueue]->isEmpty(hashTable[previousTable][currentQueue]))){

                    node = hashTable[previousTable][currentQueue]-> deleteHead();
                    digit = getdigit(node, currentDigit);

                    hashIndex = (int) digit;

                    hashTable[currentTable][hashIndex]->addTail(hashTable[currentTable][hashIndex], node);
                }
                currentQueue++;
            }
            outFile2<< printTable();
            temp = currentTable;
            currentTable = previousTable;
            previousTable = temp;
            currentDigit++;
            currentQueue = 0;
        }
    }

    void miniprint(linkedListQueue *hashTable){
        while (!(hashTable->isEmpty(hashTable))){
            listNode *t;
            int n;
            t = hashTable->deleteHead();
            n = (t->data);
            n -= offSet;
            cout<<n;
        }
    }
};

int main(int argc, char** argv){
    int maxDigits;
    int offSet;


    RadixSort *hashTable = new RadixSort();
    inFile.open(argv[1]);
    outFile1.open(argv[2]);
    outFile2.open(argv[3]);
    hashTable->firstReading();
    inFile.close();
    inFile.open(argv[1]);
    hashTable->loadStack();
    inFile.close();

    //hashTable-> radixS();
    //hashTable->miniprint(hashTable);
    outFile1.close();
    outFile2.close();
    return 0;
}

#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;
ifstream inFile1;
ifstream inFile2;
ofstream outFile;
int numNodes;
int **adjacencyMatrix;
int totalJobTimes;
int procGiven;
int temp;
int procUsed;
int availProc=0;
int orphenNode;
int currentTime;
int **scheduleTable;
int *jobMarked = new int[numNodes+1];
int *jobTimeAry = new int[numNodes + 1];
int *processJob = new int[numNodes+1];;
int *processTime = new int[numNodes+1];;
int *parentCount = new int[numNodes+1];;
int *jobDone = new int[numNodes+1];;


class Node {
    public:
        int jobID;
        int jobTime;
        Node *next;
        friend class Scheduling;

    //Node constructor
    Node(int ji, int jt){
        jobID = ji;
        jobTime = jt;
        next = NULL;
    }
    Node (Node *n){
        next = n;
    }
    Node (){}
};

string inttoString(int d){
    ostringstream tempstr;
    tempstr << d;
    return tempstr.str();
}

class LinkedList{
    public:
    Node *spot;
    Node *head;

    LinkedList(){
        head = new Node();
    }

    void insertOpen(Node *node){
        spot = head;
        if (spot !=NULL){
            while((spot->next != NULL) && (spot->next->jobTime < node->jobTime)){
                spot = spot->next;
            }
            node->next = spot->next;
            spot->next = node;
        }
    }

    void printList(LinkedList *op){
        spot = head;
        cout<<"OPEN -> ";
        while((spot != NULL)){
            cout<<"(" + inttoString(spot->jobID) + " , " + inttoString(spot->jobTime) + ")";
            spot = spot->next;
        }
    }

    Node* removeOpen(){
        Node *temp;
        temp = head->next;
        head->next = temp->next;
        return temp;
    }
};

LinkedList *OPEN;
    void loadMatrix(ifstream &inFile1) {
        int p ; int j;
        if (!inFile1) {
            cout << "Can't open file";
            exit(1);
        }
        while (!inFile1.eof()) {
            inFile1 >> p >> j;
            adjacencyMatrix[p][j] = 1;
            if(inFile1.eof()) break;
        }
        cout << "Results of LoadMatrix:"<<endl;
        for (int o = 0; o < numNodes + 1; o++) {
            for (int t = 0; t < numNodes + 1; t++) {
                cout << adjacencyMatrix[o][t];
            }
            cout << endl;
        }
    }
    int* computeTotalJobTimes(ifstream &inFile2) {
        int trash;
        int count=0;
        int* jta = new int[numNodes+1];
        inFile2 >> trash;
        while (!inFile2.eof()&& count<numNodes+1) {
            int z; int e;
            inFile2 >> z >> e;
            jta[z] = e;
            totalJobTimes += e;
            if (inFile2.eof()) break;
            count++;
        }
        cout << "TotalJobTime: " + inttoString(totalJobTimes) << endl <<endl;
        cout << "JobTimeAry:" <<endl;
        for (int i = 0; i < numNodes + 1; i++) {
            cout <<inttoString(jta[i]) + " ";
        }
        cout << endl;
        return jta;
    }

    int*  computeParents(int** adjacencyMatrix) {
        int sum = 0;
        int *cp = new int[numNodes+1];
        for (int up = 0; up < numNodes + 1; up++) {
            for (int dow = 0; dow < numNodes + 1; dow++) {
                sum += adjacencyMatrix[dow][up];
            }
            cp[up] = sum;
            sum = 0;
        }
        for (int den = 0; den < numNodes + 1; den++) {
            cout << cp[den]+ " ";
        }
        cout << endl;
        return cp;
    }

    int getUnMarkOrphen() {
        int i = 1;
        while (i < numNodes + 1) {
            if ((jobMarked[i] == 0) && (parentCount[i] == 0)) {
                temp = i;
                break;
            } else { temp = -1; }
            i++;
        }
        return temp;
    }

    void printTable() {
        cout<<endl;
        cout << "   ";
        for (int i = 0; i < totalJobTimes + 1; i++) {
            cout << "-" + inttoString(i) + "-";
        }
        cout << endl;
        for (int j = 1; j < numNodes + 1; j++) {
            cout << "P(" + inttoString(j) + ")";
            for (int k = 0; k < totalJobTimes + 1; k++) {
                if (scheduleTable[j][k] == 0) {
                    cout << "|-|";
                }
                else {
                    cout << "|" + inttoString(scheduleTable[j][k]) + "|";
                }
            }
            cout << endl;
            cout << "_____________________________________________________________________________";
            cout<<endl;
        }
    }

    int findProcessor() {
        int i= 1;
        int t =  0;
        while (i <= procGiven) {
            if (processTime[i] <= 0) {
                t = i;
                break;
            } else { t = -1; }
            i++;
        }
        return t;
    }

    bool checkProcessors() {
        int m = 0;
        bool check = false;
        for (int i = 0; i < numNodes + 1; i++) {
            if (processTime[i] > 0) m++;
        }
        if (m > 1) { check = false; }
        else if (m < 1) { check = true; }
        return check;
    }

    void updateTable(int availProc, Node *newJob, int currentTime) {
        int ct = currentTime;
        while (ct < currentTime + newJob->jobTime) {
            scheduleTable[availProc][ct] = newJob->jobID;
            ct++;
        }
    }

    bool checklist() {
        if (OPEN->head->next == NULL) return true;
        return false;
    }

    int checkCycle() {
        if ((checklist() == true) && (getUnMarkOrphen() == -1) && (checkProcessors() == true)) return 1;
        else return 0;
    }

    int findDoneJob(int i) {
        int t = 0;
        if (processTime[i] == 0) {
            t = processJob[i];
            processJob[i] = 0;
        }
        return t;
    }

    void deleteNode(int jobID) {
        jobDone[jobID] = 1;
    }
    void stringPrintTable() {
        for (int i = 0; i < totalJobTimes + 1; i++) {
            outFile << "-" + inttoString(i) + "-";
        }
        outFile << endl;
        for (int j = 1; j < numNodes + 1; j++) {
            outFile << "P(" + inttoString(j) + ")";
            for (int k = 0; k < totalJobTimes + 1; k++) {
                if (scheduleTable[j][k] == 0) outFile << "|-|";
                else { outFile << "|" + inttoString(scheduleTable[j][k]) + "|"; }
            }
            outFile << endl;
            outFile << "______________________________________________________";
            outFile <<endl;

        }
    }
    void deleteEdge(int jobID) {
        for (int i = 1; i <= numNodes; i++) {
            if (adjacencyMatrix[jobID][i] > 0) parentCount[i]--;
        }
    }


int main(int argc, char** argv){
    inFile1.open(argv[1]);
    inFile2.open(argv[2]);
    outFile.open(argv[3]);
    OPEN = new LinkedList();
    inFile1 >> numNodes;

    adjacencyMatrix= new int*[numNodes+1];
    for (int i = 0; i < numNodes + 1; i++) {
        adjacencyMatrix[i] = new int[numNodes + 1];
    }
    for (int i = 0; i < numNodes+1; i++){
        for (int j = 0; j < numNodes+1; j++){
            adjacencyMatrix[i][j] = 0;
        }
    }

    loadMatrix(inFile1);
    cout<<endl;

    for (int h = 0; h< numNodes + 1; h++) {
        jobTimeAry[h] = 0;
    }
    jobTimeAry = computeTotalJobTimes(inFile2);

    for (int i = 0; i < numNodes + 1; i++) {
        parentCount[i] = 0;
    }

    parentCount = computeParents(adjacencyMatrix);

    for (int q = 0; q < numNodes + 1; q++) {
        jobMarked[q] = 0;
    }
    cout<<"Parent Array: "<<endl;
    for (int i = 0; i < numNodes + 1; i++) {
        cout<<parentCount[i]<<" ";
    }

    cout<<endl<<endl<<"How many processors are needed? "<<endl;
    cin>>procGiven;
    cout<<endl;

    if (procGiven <= 0){
        cout<<"Error: Need at least one processor"<<endl;
        exit(0);
    }
    if (procGiven > numNodes) procGiven = numNodes;

    procUsed = 0;
    currentTime = 0;

    scheduleTable = new int *[numNodes + 1];
        for (int i = 0; i < numNodes+1; i++) {
            scheduleTable[i] = new int[totalJobTimes + 1];
        }
    for (int i = 0; i < numNodes+1; i++){
        for (int j = 0; j < totalJobTimes+1; j++){
            scheduleTable[i][j] = 0;
        }
    }

    for (int i = 0; i < numNodes + 1; i++) {
        processJob[i] = 0;
    }

    for (int i = 0; i < numNodes + 1; i++) {
        processTime[i] = 0;
    }

    for (int i = 0; i < numNodes + 1; i++) {
        jobDone[i] = 0;
    }

    //STEP 13
    //STEP 1 & STEP 2

    for (int m =1; m < numNodes+1; m++){
        while(jobDone[m] !=1){
            orphenNode = 0;
            while(orphenNode !=-1) {
                orphenNode = getUnMarkOrphen();
                if (orphenNode != -1) {
                    jobMarked[orphenNode] =1;
                    Node *newNode;
                    newNode = new Node(orphenNode, jobTimeAry[orphenNode]);
                    OPEN->insertOpen(newNode);
                }
            }

            //STEP 3
            OPEN->printList(OPEN);

            //STEP 4 & STEP 5
            while((OPEN->head->next != NULL) && (procUsed < procGiven)){
                availProc = findProcessor();
                if (availProc >0){
                    procUsed++;
                    Node *newJob = OPEN->removeOpen();
                    processJob[availProc] = newJob->jobID;
                    processTime[availProc] = newJob->jobTime;
                    updateTable(availProc,newJob, currentTime);
                }
            }

            //STEP 6
            if(checkCycle() == 0){
                cout<<endl;
            } else{
                cout<<endl<<"Error: Cycle Found";
                stringPrintTable();
                exit(0);
            }

            //STEP 7
            printTable();

            //STEP 8
            currentTime++;

            //STEP 9
            for (int i = 0; i < numNodes+1; i++){
                processTime[i] = processTime[i] -1;
            }

            //STEP 10 & 11
            for (int i = 0; i < numNodes+1; i++){
                int job;
                job = findDoneJob(i);
                if (job !=0){
                    deleteNode(job);
                    deleteEdge(job);
                    procUsed--;
                }
            }

            //STEP 12
            cout<<endl<<"Current Time: "<< currentTime <<endl;
            cout<<"Job Marked Array: ";
            for (int i = 0; i < numNodes+1; i++){
                cout<<jobMarked[i]<<" ";
            }
            cout<<endl <<"Parent Array: ";
            for (int i = 0; i < numNodes+1; i++){
                cout<<parentCount[i]<<" ";
            }
            cout<<endl <<"Process Time Array: ";
            for (int i = 0; i < numNodes+1; i++){
                cout<<processTime[i]<<" ";
            }
            cout<<endl <<"Process Job Array: ";
            for (int i = 0; i < numNodes+1; i++){
                cout<<processJob[i]<<" ";
            }
            cout<<endl <<"Job Done Array: ";
            for (int i = 0; i < numNodes+1; i++){
                cout<<jobDone[i]<<" ";
            }
            cout<<endl<<endl;
        }

    }
            //STEP 14
            stringPrintTable();

    //STEP 15
    inFile1.close();
    outFile.close();
    inFile2.close();

};


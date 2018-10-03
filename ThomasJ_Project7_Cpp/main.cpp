#include <iostream>
#include <fstream>
using namespace std;
ifstream inFile;
ofstream outFile1;
ofstream outFile2;


class DijktraSSS{
public:
    int numNodes;
    int sourceNode;
    int minNode;
    int currentNode;
    int newCost;
    int **costMatrix;
    int *fatherAry = new int[numNodes+1];
    int *markedAry = new int[numNodes+1];
    int *bestCostAry = new int [numNodes+1];

    DijktraSSS(int nn, int src){
        numNodes = nn;
        sourceNode = src;
    };
    DijktraSSS(int nn){
        numNodes = nn;
    }

    void loadCostMatrix(ifstream &inFile){
        int n1; int n2; int edgecost;
        if (!inFile) {
            cout << "Can't open file";
            exit(1);
        }
        while (!inFile.eof()) {
            inFile >>n1 >>n2 >> edgecost;
            costMatrix[n1][n2] = edgecost;
            if(inFile.eof()) break;
        }
    }

    void setBestCostAry(int sourceNode){
        for (int i = 1; i < numNodes + 1; i++){
            bestCostAry[i] = costMatrix[sourceNode][i];
        }
    }

    void setFatherAry(int sourceNode){
        for (int i = 1; i < numNodes + 1; i++){
            fatherAry[i] = sourceNode;
        }
    }

    void setMarkedAry(){
        for (int i = 1; i < numNodes + 1; i++) {
            markedAry[i] = 0;
        }
        markedAry[sourceNode] = 1;
    }

    int computeCost(int minNode, int currentNode){
        int cost;
        cost = bestCostAry[minNode] + costMatrix[minNode][currentNode];
        return cost;
    }

    void markMinNode (int minNode){
        markedAry[minNode] = 1;
    }

    void changeFather(int node, int minNode){
        fatherAry[node] = minNode;
    }

    void changeCost(int node, int newCost){
        bestCostAry[node] = newCost;
    }

    void debugPrint(){
        outFile2<<"The sourceNode is: " <<sourceNode<<endl;
        outFile2<<"fatherAry:"<<endl;
        for (int i = 1; i < numNodes + 1; i++){
            outFile2<<fatherAry[i]<<" ";
        }
        outFile2<<endl;
        outFile2<<"bestCostAry:"<<endl;
        for (int i = 1; i < numNodes + 1; i++){
            outFile2<<bestCostAry[i]<<" ";
        }
        outFile2<<endl;
        outFile2<<"markedAry:"<<endl;
        for (int i = 1; i < numNodes + 1; i++){
            outFile2<<markedAry[i]<<" ";
        }
        outFile2<<endl<<endl;
    }

    void printShortestPaths(int sourceNode, int objsourceNode) {
        outFile1<< "The path from " <<objsourceNode<< " to " << sourceNode <<" is: ";
        int papaNode = sourceNode;
        if (objsourceNode == papaNode) outFile1<<papaNode;
        int cost = bestCostAry[sourceNode];
        int count = 0;
        while (papaNode !=objsourceNode){
            if (count == 0) {outFile1<<papaNode;}
            else {outFile1 << " <- " <<papaNode;}
            count++;
            papaNode = fatherAry[papaNode];
        }
        outFile1<< " <- " << papaNode << "   cost = " <<cost <<endl;
    }
};
int main(int argc, char** argv) {
    //STEP 0
    //Open input & output files
    int nn;
    inFile.open(argv[1]);
    outFile1.open(argv[2]);
    outFile2.open(argv[3]);

    //get numNodes from input
    inFile >> nn;

    DijktraSSS *tmp = new DijktraSSS(nn);

    //Allocate & initalize all members in the DijkstraSSS class
    tmp->costMatrix = new int *[nn + 1];
    for (int i = 1; i < nn+ 1; i++) {
        tmp->costMatrix[i] = new int[nn + 1];
    }
    for (int i = 1; i < nn + 1; i++) {
        for (int j = 1; j < nn + 1; j++) {
            if (i == j) tmp->costMatrix[i][j] = 0;
            else tmp->costMatrix[i][j] = 99999;
        }
    }
    for (int i = 0; i < nn+ 1; i++) {
        tmp->fatherAry[i] = i;
    }
    for (int i = 0; i < nn + 1; i++) {
        tmp->markedAry[i] = 0;
    }
    for (int i = 0; i < nn+ 1; i++) {
        tmp->bestCostAry[i] = 9999;
    }

    //STEP 1
    //loadMatrix(inFile)
    tmp->loadCostMatrix(inFile);

    //sourceNode<-1
    tmp->sourceNode = 1;

    //STEP 11
    //repeat step 2 to 10 until sourceNode > numNodes
    while (!(tmp->sourceNode > tmp->numNodes)) {

        //STEP 2
        //setBestCostAry(sourceNode)
        tmp->setBestCostAry(tmp->sourceNode);

        //setFatherAry(sourceNode)
        tmp->setFatherAry(tmp->sourceNode);

        //setMarkedAry()
        tmp->setMarkedAry();

        //STEP 6
        //repeat step 3 to 5 until all nodes are marked
        for (int k = 1; k <= tmp->numNodes; k++){
            while (tmp->markedAry[k] == 0){

                //STEP 3
                //minNode <-find an unmarked node w. minimum cost from bestCostAry
                int mincost = 9999999;
                for (int i = 1; i < tmp->numNodes + 1; i++) {
                    if (tmp->markedAry[i] == 0) {
                        if (tmp->bestCostAry[i] < mincost) {
                            mincost = tmp->bestCostAry[i];
                            tmp->minNode = i;
                        }
                    }
                }

                //markMinNode(minNode)
                tmp->markMinNode(tmp->minNode);

                //call debugPrint
                tmp->debugPrint();

                //STEP 5
                //repeat step 4 until all unmarked nodes in markedAry evaluated
                for (int h = 1; h < tmp->numNodes + 1; h++) {

                    //STEP 4
                    //expanding the minNode
                    //currentNode <-find the next unmarked node in markedAry
                    if (tmp->markedAry[h] == 0) {
                        tmp->currentNode = h;

                        //newCost <-computeCost(minNode, currentNode)
                        tmp->newCost = tmp->computeCost(tmp->minNode, tmp->currentNode);

                        //if newCost < bestCostAry[currentNode]
                        if (tmp->newCost < tmp->bestCostAry[tmp->currentNode]) {

                            //changeFather(currentNode, minNode)
                            tmp->changeFather(tmp->currentNode, tmp->minNode);

                            //changeCost(currentNode, newCost)
                            tmp->changeCost(tmp->currentNode, tmp->newCost);
                        }
                    }

                    //call debugPrint
                    tmp->debugPrint();
                }
            }
        }

        //STEP 7
        //currentNode <-1
        tmp->currentNode = 1;

        //STEP 10
        if (tmp->sourceNode == 1) {
            outFile1 << "===============================================" << endl;
            outFile1 << "There are " << tmp->numNodes << " nodes in the input graph" << endl;
        }
        outFile1<<"==============================================="<<endl;
        outFile1<< "Source node = " << tmp->sourceNode <<endl;
        outFile1<< "The shortest paths from the source node " << tmp->sourceNode <<" are:" <<endl;

        //repeat step 8 and 9 until currentNode >=numNodes
        while (tmp->currentNode < tmp->numNodes+1) {
            //STEP 8
            //printShortestPath(currentNode)
            tmp->printShortestPaths(tmp->currentNode, tmp->sourceNode);

            //STEP 9
            //currentNode++
            tmp->currentNode++;
        }
        tmp->sourceNode++;
    }
    //STEP 12
    //close all files
    inFile.close();
    outFile1.close();
    outFile2.close();
}


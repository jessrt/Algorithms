
import java.util.Scanner;

public class Scheduling extends LinkedList {

	static int numNodes; 
	static int totalJobTimes; 
	static int procGiven; 
	static int temp; 
	static int procUsed; 
	static int currentTime; 
	
	//if adjacencyMatrix[i][j] >=1 means j is a dependent of i
	static int[][]  adjacencyMatrix; 
	
	
	//record the schedule. Dimension determined at runtime
	static int[][]  scheduleTable;
	
	//1D to store the time requirement for each job. 
	static int[]  jobTimeAry;
	
	//OPEN linked list 
	static LinkedList OPEN; 
	
	//1D to indicate which job is being processed by processor [i]
	//if processJob[i]=5, then processor i is processing job 5
	static int[]  processJob;
	
	//1D to indicate process time remaining on processor [i[
	//ProcessTime[i]>0 processor i is busy, <=0 processor i free
	static int[]  processTime;
	
	//1D to store number of parents of each job
	//O is orphan sum of each *column j* in the matrix is the total parent counts of node j
	static int[]  parentCount;
	
	//1D keep track of which jobs remain
	//jobDone[i]==1 means job i deleted from graph, ==0 means still in graph
	static int[]  jobDone;
	
	//1D to keep track of if jobs are current on OPEN/processor
	//1 means on OPEN/Processor, 0 means not
	static int[]  jobMarked;
	
	//METHODS
	
	//load the matrix from inFile1
	static void loadMatrix(Scanner inFile1){
		
		adjacencyMatrix = new int[numNodes+1][numNodes+1];
		
		while (inFile1.hasNext()){
			int i = inFile1.nextInt();
			int j = inFile1.nextInt(); 
			adjacencyMatrix[i][j] = 1;
			if(!inFile1.hasNextInt()) break; 
		}
		
		System.out.println("Results of LoadMatrix:");
		for (int o = 0; o < numNodes +1; o++){
			for (int t = 0; t <numNodes +1; t++){
				System.out.print(adjacencyMatrix[o][t]);
			}
			System.out.println("");
		}
		
	}
	
	static int computeTotalJobTimes(Scanner inFile2){
		int trash = inFile2.nextInt();
		while (inFile2.hasNext()){
			int i = inFile2.nextInt();
			int j = inFile2.nextInt();
			jobTimeAry[i] = j; 
			totalJobTimes +=j;
			if(!inFile2.hasNextInt()) break; 
		}
		System.out.println("");
		System.out.print("TotalJobTime: ");
		System.out.println(totalJobTimes);
		System.out.println("");
		System.out.println("JobTimeAry");
		
		for (int i = 0; i < numNodes+1; i++)
			System.out.print(jobTimeAry[i]+ " ");
		System.out.println("");
		return totalJobTimes;
	}
	
	static void computeParents(int [][] am){
		int sum = 0; 
		for (int t = 0; t < numNodes +1; t++){
			for (int o = 0; o <numNodes +1; o++){
				sum += adjacencyMatrix[o][t];
			}
			parentCount[t] = sum;
			sum = 0; 
			}
		System.out.println("Results of computeParents:");
		for (int k = 0; k <numNodes +1; k++){
			System.out.print(parentCount[k] + " ");
		}
		System.out.println("");
	}
	
	static int getUnMarkOrphen(){
		int i = 1; 
		while (i < numNodes+1){
			if ((jobMarked[i] == 0) && (parentCount[i] ==0)){
				temp = i;
				break; 
			}
			else {temp = -1;}
			i++; 
		}
		return temp;
	}
	
	static void printTable(){
		System.out.print("   ");
		for (int i = 0; i < totalJobTimes+1; i++){
			System.out.print("-"+i+"-");
		}
		System.out.println("");
		for (int j = 1; j < numNodes+1; j++){
			System.out.print("P(" + j + ")");
			for (int k = 0; k < totalJobTimes+1; k++){
				if (scheduleTable[j][k]==0) {System.out.print("|-|");}
				else {System.out.print("|" + scheduleTable[j][k] + "|");}
			}
			System.out.println("");
			System.out.println("________________________________________________________________________");
		}
		
	}
	
	static String stringPrintTable(){
		String chunk = "   "; 
		String sep = System.lineSeparator();
		for (int i = 0; i < totalJobTimes+1; i++){
			chunk += ("-"+i+"-");
		}
		chunk += sep; 
		for (int j = 1; j < numNodes+1; j++){
			chunk += sep; 
			chunk +=("P(" + j + ")");
			for (int k = 0; k < totalJobTimes+1; k++){
				if (scheduleTable[j][k]==0) {chunk += ("|-|");}
				else {chunk += ("|" + scheduleTable[j][k] + "|");}
			}
			chunk += sep; 
			chunk += "________________________________________________________________________\n";
			chunk += sep; 
		}
		return chunk; 
	}
	
	static int findProcessor(){
		int i = 1; 
		int t = 0;
		while (i <= procGiven){
			if (processTime[i] <=0) {
				t = i;
				break; 
			}
			else {t = -1;}
			i++;
		} 
		return t;
	}
	
	static boolean checkProcessors(){
		int m = 0; 
		boolean check = false; 
		for (int i = 0; i < numNodes+1; i++){
			if (processTime[i] > 0) m++; 
		}
		if (m > 1) {check = false;}
		else if (m < 1){ check = true;}
		return check; 
	}
	
	static void updateTable(int availProc, Node newJob, int currentTime){
		int ct = currentTime;
		while (ct < currentTime+newJob.jobTime){
			scheduleTable[availProc][ct] = newJob.jobId;
			ct++; 
		}
	}
	static boolean checklist(){
		if (OPEN.head.next == null) return true; 
		return false;
	}
	
	static int checkCycle(){
				
		if ((checklist() == true) && (getUnMarkOrphen() == -1) && (checkProcessors() == true)) return 1; 
		else return 0; 
	}
	
	static int findDoneJob(int i){
		int t = 0; 
		if (processTime[i]==0){
			t = processJob[i];  
			processJob[i] = 0; 
		}
		return t;
	}
	
	static void deleteNode(int jobId){
		jobDone[jobId] = 1; 
	}
	
	static void deleteEdge(int jobId){
		for (int i = 1; i <=numNodes; i++){
			if (adjacencyMatrix[jobId][i] > 0) parentCount[i]--; 
		}
	}
	
}

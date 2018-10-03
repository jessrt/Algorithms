
import java.io.*;
import java.util.Arrays;
import java.util.Scanner; 

public class Main extends Scheduling { 
	
	static int procUsed; 
	static int currentTime;
	static int availProc; 
	static int count; 
	static int orphenNode; 
	public static void main(String[] args){
		if (args.length<3){
			System.out.println("Error: Insufficient Arguments");
			System.exit(1);
		}
		try{
			
			//STEP 0 
			
			Scanner inFile1 = new Scanner(new FileInputStream(args[0]));
			Scanner inFile2 = new Scanner(new FileInputStream(args[1])); 
			BufferedWriter outFile = new BufferedWriter(new FileWriter(args[2]));
			
			numNodes = inFile1.nextInt();
			loadMatrix(inFile1);
			jobTimeAry = new int[numNodes+1];
			totalJobTimes = computeTotalJobTimes(inFile2);
			
			parentCount = new int[numNodes+1];
			System.out.println("");
			computeParents(adjacencyMatrix);
			
			jobMarked = new int[numNodes+1];
			System.out.println("");
			 
			Scanner reader = new Scanner(System.in);
			System.out.println("How many processors are needed?");
			procGiven = reader.nextInt(); 
			if (procGiven <= 0){
				System.out.println("Error: Need at least one processor");
				System.exit(1);
			}
			if (procGiven > numNodes){
				procGiven = numNodes; 
			}
			reader.close(); 
			
			procUsed = 0; 
			currentTime = 0; 
			scheduleTable = new int[numNodes+1][totalJobTimes+1];
			processJob = new int[numNodes+1];
			processTime= new int[numNodes+1];
			jobDone = new int[numNodes+1];
			OPEN = new LinkedList(); 
			
			//STEP 13
			//STEP 1 & STEP 2
			
			for (int m = 1; m < numNodes+1; m++){
				while (jobDone[m] !=1){
					orphenNode = 0; 
					while (orphenNode != -1){
						orphenNode = getUnMarkOrphen(); 
						if (orphenNode != -1){
							jobMarked[orphenNode] = 1; 
							Node newNode;
							newNode = new Node (orphenNode, jobTimeAry[orphenNode]); 
							insertOpen(newNode);
						}
					}	
					
					//STEP 3
					printList(OPEN); 
					
					//STEP 4 & STEP 5
					while ((OPEN.head.next != null) && (procUsed < procGiven)){
						availProc = findProcessor(); 
						if (availProc > 0){
							procUsed++; 
							Node newJob = removeOpen(); 
							processJob[availProc] = newJob.jobId; 
							processTime[availProc] = newJob.jobTime;
							updateTable(availProc, newJob, currentTime); 		
						}
					}
			
					//STEP 6
					if (checkCycle() == 0){
							System.out.println("");
					}
						else{
							System.out.println("Error: Cycle Found");
							System.exit(1);
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
					System.out.println("Current Time: " + currentTime);
					System.out.print("Job Marked Array: ");
					for (int i = 0; i < numNodes+1; i++){
						System.out.print(jobMarked[i] + " ");
					}
					System.out.println("");
					System.out.print("Parent Array: ");
					for (int i = 0; i < numNodes+1; i++){
						System.out.print(parentCount[i] + " ");
					}
					System.out.println("");
					System.out.print("Process Time Array: ");
					for (int i = 0; i < numNodes+1; i++){
						System.out.print(processTime[i] + " ");
					}
					System.out.println("");
					System.out.print("Process Job Array: ");
					for (int i = 0; i < numNodes+1; i++){
						System.out.print(processJob[i] + " ");
					}
					System.out.println("");
					System.out.print("Job Done Array: ");
					for (int i = 0; i < numNodes+1; i++){
						System.out.print(jobDone[i] + " ");
					}
					System.out.println("");
					System.out.println("");
					System.out.println("");
					System.out.println("");
				}
			}
			
			//STEP 14
			outFile.write(stringPrintTable()); 
			
			//STEP 15
			inFile1.close();
			inFile2.close();
			outFile.close();
			
		}
		catch(FileNotFoundException e){
			System.out.println("Error: Finding/Reading File");
		}
		catch(IOException e){
			System.out.println("Error: Problem in Writing");
		}	
	}
}

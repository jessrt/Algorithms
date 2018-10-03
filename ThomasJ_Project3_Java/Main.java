import java.io.*;
import java.util.Scanner; 
import java.util.Arrays; 

public class Main extends HeapSort {
	static int numItems = 0; 
	static int capacity = 0; 
	
	public static void main(String[] args) { 
		
		if (args.length <3) {
			System.out.println("Error: Insufficient Arguments");
			System.exit(1); 
		}
		
		try {
			Scanner inFile = new Scanner(new FileInputStream(args[0])); 
			BufferedWriter outFile1 = new BufferedWriter(new FileWriter(args[1]));
			BufferedWriter outFile2 = new BufferedWriter(new FileWriter(args[2]));
			
			numItems = countData(inFile); 
			capacity = numItems + 1; 
			int[] heapAry = new int[capacity]; 
			Arrays.fill(heapAry, 0);
		
			inFile.close(); 
			
			Scanner inFile_2 = new Scanner(new FileInputStream(args[0])); 
			while (inFile_2.hasNextInt()) {
				int data = inFile_2.nextInt(); 
				insertOneDataItem(data, heapAry);
				outFile1.write(printHeap(heapAry));
				if (!inFile_2.hasNextInt()) break; 
			}
			
			outFile1.write("\n");
			inFile_2.close();
			
			while (heapAry[0] !=0) {
				outFile2.write(getRoot(heapAry));
				replaceRoot(heapAry);
				outFile1.write(printHeap(heapAry));
			}
			outFile1.close();
			outFile2.close();
			
			
		}
		catch (FileNotFoundException e){
			System.out.println("Error: Finding/Reading File");
		} catch (IOException e) {
			System.out.println("Error: Problem in Writing");
		}
		
	}
}

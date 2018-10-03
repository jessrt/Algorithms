import java.util.Scanner;

public class HeapSort {
	static int numItems = 0; 
	static int currentIndex = 0; 
	static int capacity = 0; 
	static int fatherindex = 0; 
	static int leftKidIndex = 0; 
	static int rightKidIndex = 0; 
	static int rootIndex = 1; 
	static int minIndex = 0; 
	static int temp = 0; 
	static int temp2 = 0; 
	
	public static int countData(Scanner inFile) {
		while (inFile.hasNextInt()) {
			int i = inFile.nextInt();
			numItems++; 
			if (!inFile.hasNextInt()) break; 
		}
		
		return numItems; 
	}
	
	public static void insertOneDataItem(int data, int[] heapAry) {
		heapAry[0]++; 
		currentIndex = heapAry[0];
		heapAry[currentIndex] = data; 
		if (isHeapFull(heapAry, capacity)) return; 
		bubbleup(currentIndex, heapAry); 
		printHeap(heapAry);
	}
	
	public static boolean isHeapFull(int[] heapAry, int capacity) {
		if (heapAry[0]== capacity) return true;
		else return false; 
	}
	
	public static boolean isHeapEmpty(int[] heapAry) {
		if (heapAry[0]==0) return true; 
		else return false; 
	}
	
	
	public static boolean isRoot(int index) {
		if (index == 1) return true;
		else return false; 
	}
	
	public static String printHeap(int []heapAry) {
		String temparray = "";
		for (int i = 0; i < heapAry.length; i++) {
			temparray += " " + heapAry[i];
			if (i == (heapAry.length)-1) temparray = temparray + "\n";
		}
		return temparray;
	}
	
	public static void bubbleup (int kidIndex, int[] heapAry) {
		if (isRoot(kidIndex)) return; 
		else {
			fatherindex = kidIndex/2; 
			if (heapAry[kidIndex] >= heapAry[fatherindex]) return; 
			else {
				swap(fatherindex, kidIndex, heapAry);
				bubbleup(fatherindex, heapAry); 
			}
		}
	}
	
	public static String getRoot(int[] heapAry) {
		String current_root = Integer.toString(heapAry[1]) + " ";
		return current_root; 
	}
	
	public static void replaceRoot(int[] heapAry) {
		heapAry[rootIndex] = heapAry[heapAry[0]]; 
		heapAry[0]--;
		bubbleDown(rootIndex, heapAry); 
	}
	
	public static void bubbleDown(int fatherindex, int[] heapAry) {
		if (isLeaf(fatherindex, heapAry)) return; 
		else {
			leftKidIndex = fatherindex * 2; 
			rightKidIndex = (fatherindex * 2) + 1;
			minIndex = findMinKidIndex(leftKidIndex, rightKidIndex, heapAry);
			if (heapAry[minIndex] >= heapAry[fatherindex]) return;
			else {
				swap(fatherindex, minIndex, heapAry);
				bubbleDown(minIndex, heapAry); 
			}
		}
	}
	
	public static boolean isLeaf(int index, int[] heapAry) {
		if ((index * 2) > heapAry[0]) return true; 
		else return false; 
	}
	
	public static int findMinKidIndex(int leftKidIndex, int rightKidIndex, int[] heapAry) {
		if (heapAry[leftKidIndex] > heapAry[rightKidIndex]) return rightKidIndex; 
		else return leftKidIndex; 
	}
	
	public static void swap (int index1, int index2, int [] heapAry) {
		int temp = heapAry[index1];
		heapAry[index1] = heapAry[index2];
		heapAry[index2] = temp; 
	}
}

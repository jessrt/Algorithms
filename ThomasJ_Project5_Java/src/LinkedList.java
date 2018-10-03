
public class LinkedList {
	protected static  Node head; 
	protected static Node spot; 
	
	
	public LinkedList(){
		head = new Node();
	}
	
	static void insertOpen (Node node){
			spot = head; 
			if (spot !=null){
			while ((spot.next != null) && (spot.next.jobTime < node.jobTime)){
				spot = spot.next; 
			}
			node.next = spot.next; 
			spot.next = node; 
			}
	}
	
	static void printList(LinkedList op){
		spot = head; 
		System.out.print("OPEN -> ");
		while ((spot !=null)){
			System.out.print("(" + spot.jobId + " , " + spot.jobTime + ")");
			spot = spot.next;
		}
		
	}
	
	static Node removeOpen(){
		Node temp = new Node(); 
		temp = head.next;
		head.next = temp.next; 
		return temp;
	}
	
	static int counting(){
		Node t = new Node(); 
		t= head; 
		int l = 0; 
		while ((t.next !=null)){
			l++; 
			t = t.next;
		}
		return l; 
	}
	
}

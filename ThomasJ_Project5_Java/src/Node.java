
public class Node {
	public int jobId; 
	public int jobTime; 
	Node next; 
	
	public Node(int ji, int jt){
		this.jobId = ji; 
		this.jobTime = jt; 
		next = null; 
	}
	
	
	public Node (Node n){
		next = n; 
	}

	public Node() {
		
	}
	
}

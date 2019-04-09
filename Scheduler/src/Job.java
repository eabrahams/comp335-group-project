import java.util.*;
public class Job {
	
	private String[] data;
	
	public String[] getData() {
		return data;
	}
	
	public void setData(String a) {
		
		String input = a;
		
		String[] data = input.split(" ");
		
		this.data = data;
	}
	
	public Job(String d) {
		setData(d);
	}

}

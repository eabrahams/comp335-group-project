import java.util.*;
public class Job {
	
	private String submitTime;
	private String jobId;
	private String estimatedRuntime;
	private String cores;
	private String memory;
	private String disk;
	
	private boolean complete = false;
	
	private String[] data = new String[7];
	
	
	/* public Job(String d) {
		setData(d);
	} */
	
	//getter
	
	public String[] getData() {
		return data;
	}
	
	public int getSubmitTime(){
		return submitTime;
	}
	
	public int getJobId(){
		return jobId;
	}
	
	public int getEstimatedRuntime(){
		return estimatedRuntime;
	}
	
	public int getCores(){
		return cores;
	}
	
	public int getMemory(){
		return memory;
	}
	
	public int getDisk(){
		return disk;
	}
	
	public int getCompleted(){
		return complete;
	}
	
	//setter
	public void setData(String a) {
		this.data[0] = null;
		this.data[1] = submitTime;
		this.data[2] = jobId;
		this.data[3] = estimatedRuntime;
		this.data[4] = cores;
		this.data[5] = memory;
		this.data[6] = disk;
	}
	
	public void setSubmitTime(int t){
		this.submitTime = Integer.toString(t);
	}
	
	public void setJobId(int jID){
		this.jobId = Integer.toString(jID);
	}
	
	public void setEstimatedRuntime(int rt){
		this.estimatedRuntime = Integer.toString(rt);
	}
	
	public void setCores(int c){
		this.cores = Integer.toString(c);
	}
	
	public void setMemory(int m){
		this.memory = Integer.toString(m);
	}
	
	public void setDisk(int d){
		this.disk = Integer.toString(d);
	}
	
	public void setComplited(boolean true){
		this.complete = true;
	}
		
	public Job(int submitTime, int jobId, int estimatedRuntime, int cores, int memory, int disk){
		setSubmitTime(submitTime);
		setJobId(jobId);
		setEstimatedRuntime(estimatedRuntime);
		setCores(cores);
		setMemory(memory);
		setDisk(disk);
	}
	
}

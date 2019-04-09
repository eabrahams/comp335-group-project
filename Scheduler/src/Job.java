public class Job {
	
	private int submitTime;
	private int jobId;
	private int estimatedRuntime;
	private int cores;
	private int memory;
	private int disk;
	
	private boolean complete = false;
	
	
	//getter
	
	
	
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
	
	public boolean getCompleted(){
		return complete;
	}
	
	//setter

	
	public void setSubmitTime(int t){
		this.submitTime = t;
	}
	
	public void setJobId(int jID){
		this.jobId = jID;
	}
	
	public void setEstimatedRuntime(int rt){
		this.estimatedRuntime = rt;
	}
	
	public void setCores(int c){
		this.cores = c;
	}
	
	public void setMemory(int m){
		this.memory = m;
	}
	
	public void setDisk(int d){
		this.disk = d;
	}
	
	public void setComplited(){
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
	
	//to get data as an array
	public int[] getData() {
		
		int[] data = new int[7]; //{null, submitTime, jobId, estimatedRuntime, cores, memory, disk};
		
		data[1] = submitTime;
		data[2] = jobId;
		data[3] = estimatedRuntime;
		data[4] = cores;
		data[5] = memory;
		data[6] = disk;
		
		return data;
	}
}

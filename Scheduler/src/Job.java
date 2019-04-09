public class Job {
	
	private static String submitTime;
	private static String jobId;
	private static String estimatedRuntime;
	private static String cores;
	private static String memory;
	private static String disk;
	
	private boolean complete = false;
	
	
	//getter
	
	
	
	public static String getSubmitTime(){
		return submitTime;
	}
	
	public static String getJobId(){
		return jobId;
	}
	
	public static String getEstimatedRuntime(){
		return estimatedRuntime;
	}
	
	public static String getCores(){
		return cores;
	}
	
	public static String getMemory(){
		return memory;
	}
	
	public static String getDisk(){
		return disk;
	}
	
	public boolean getCompleted(){
		return complete;
	}
	
	//setter

	
	public void setSubmitTime(String t){
		this.submitTime = t;
	}
	
	public void setJobId(String jID){
		this.jobId = jID;
	}
	
	public void setEstimatedRuntime(String rt){
		this.estimatedRuntime = rt;
	}
	
	public void setCores(String c){
		this.cores = c;
	}
	
	public void setMemory(String m){
		this.memory = m;
	}
	
	public void setDisk(String d){
		this.disk = d;
	}
	
	public void setComplited(){
		this.complete = true;
	}
		
	public Job(int submitTime, int jobId, int estimatedRuntime, int cores, int memory, int disk){
		setSubmitTime(Integer.toString(submitTime));
		setJobId(Integer.toString(jobId));
		setEstimatedRuntime(Integer.toString(estimatedRuntime));
		setCores(Integer.toString(cores));
		setMemory(Integer.toString(memory));
		setDisk(Integer.toString(disk));
	}
	
	
	public static String[] getData() {
		
		String[] data = new String[7]; //{null, submitTime, jobId, estimatedRuntime, cores, memory, disk};
		data[0] = null;
		data[1] = submitTime;
		data[2] = jobId;
		data[3] = estimatedRuntime;
		data[4] = cores;
		data[5] = memory;
		data[6] = disk;
		
		return data;
	}
}

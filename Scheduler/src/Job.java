import java.util.ArrayList;

public class Job{
	int subTime;
	int jobID;
	int est;
	int core;
	int memory;
	int disk;
	
	boolean complete;
	
	public Job()
	{
		//initiate an obj with null value
	}
    
    public Job(String[] arr)
    {
    	if (arr.length != 7)
    	{
    		System.out.println("Job can not be added. Insufficient value received");
    		System.out.println("--------------------------------------------------");
    	}
		
    	this.subTime = Integer.parseInt(arr[1]);
    	this.jobID = Integer.parseInt(arr[2]);
    	this.est = Integer.parseInt(arr[3]);
    	this.core = Integer.parseInt(arr[4]);
    	this.memory = Integer.parseInt(arr[5]);
    	this.disk = Integer.parseInt(arr[6]);
		
		//assigning complete to false as default
    	this.complete = false;	
    }
    
    public String getJobRESC()
    {
    	return this.core + " " + this.memory + " " + this.disk;
    }
    
	//changing complete to true as the job gets done
    public void jobDone()
    {
        this.complete = true;
    }
    
    public String toString()
    {
    	return "--> Current Job >> id : " + this.jobID + " core : " + this.core + " memory : " + this.memory + " disk : " + this.disk;
    }
}
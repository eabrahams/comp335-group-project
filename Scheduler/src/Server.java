public class Server{
	String serverType;
	int serverID;
	int state; 			//Inactive = 0, Booting = 1, Idle = 2, Active = 3, Unavailable = 4
    int availTime;
    int core;
    int memory;
    int disk;
    
    public Server()
    {
    	//initiate object with null value
    }
    
    public Server(String[] arr)
    {
    	if (arr.length != 7)
    	{
    		System.out.println("Server can not be added. Insufficient value received");
    		System.out.println("--------------------------------------------------");
    	}
    	
    	this.serverType = arr[0];
    	this.serverID = Integer.parseInt(arr[1]);
    	this.state = Integer.parseInt(arr[2]);
    	this.availTime = Integer.parseInt(arr[3]);
    	this.core = Integer.parseInt(arr[4]);
    	this.memory = Integer.parseInt(arr[5]);
    	this.disk = Integer.parseInt(arr[6]);
    }
    
    public boolean compareJob(Job job)
    {
    	if(job.core <= this.core && job.memory <= this.memory && job.disk <= this.disk) {
    		return true;
    	}
    	return false;
    }
}


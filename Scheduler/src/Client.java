import java.awt.List;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.EOFException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.io.Reader;
import java.io.UnsupportedEncodingException;
import java.net.Socket;
import java.net.UnknownHostException;
import java.io.BufferedReader;
import java.util.*;

public class Client {
	Socket socket =  null;
	
	//list of JOB
    ArrayList<Job> listJob = new ArrayList<>();
    Job currentJob = new Job();
    
    //list of SERVER
    ArrayList<Server> listServer = new ArrayList<>();
    HashMap<String, Server[]> newListServer = new HashMap<>();
    
    //storing initial server state
    ArrayList<Server> initServer = new ArrayList<>();
    
    //getting the largest server
    public String getLargest()
    {
    	return listServer.get(listServer.size()-1).serverType;
    }
    
    //firsFit method
    public String getFirstFit()
    {
    	//using resc all
    	Server tempSer = null;
    	boolean firstServer = true;
    	
    	for(Server ser : listServer)
    	{
    		if(ser.compareJob(currentJob))
    		{
    			return ser.serverType+" "+ser.serverID;
    		}
    	}
 
    	//getting the first active server
    	tempSer = getInitServer();
    	
    	return tempSer.serverType+" "+tempSer.serverID;
    }

    //for making array of byte
    public byte[] sendToServer(String s)
    {
        String temp = s+"\n";
        byte[] bytes = temp.getBytes();
        return bytes;

    }
    
    //reading message and returning message
    public String lineReader(Socket s) throws UnsupportedEncodingException, IOException
    {
        Job j = new Job();
        
        BufferedReader input = new BufferedReader(new InputStreamReader(s.getInputStream(), "UTF-8"));//receive buffer
        
        String line = input.lineReader();

        if(line.contains(("JOBN"))) {
            addJob(line);
            System.out.println(currentJob.toString());
        }
        
        return line;
    }
    
    //for scheduling the JOB
    public void scheduleJob(PrintStream pr, Socket s, Integer useAlg) throws UnsupportedEncodingException, IOException //add parser par
    {
    	String str = null;
    	if (useAlg == null) //null or not ff
    		str = listServer.get(listServer.size()-1).serverType+" 0"; //scheduling to the largest
    	else if (useAlg == 1)
    		str = getFirstFit();
//        else if (useAlg == 2)
//            str = getBestFit();
//        else if (useAlg == 3)
//            str = worstFit();
    	
    	pr.write(sendToServer("SCHD "+currentJob.jobID+" "+str));
    	System.out.println(">>> "+currentJob.jobID+" SCHEDULED TO :"+str);
        if (lineReader(s).contains("OK"))
        {
            currentJob.jobDone();
            listServer.clear();
        }
    }
    
    //getting job IDs
    public void addJob(String str){
    	String[] jobStr = str.split(" ");
    	Job job = new Job(jobStr);
    	listJob.add(job);
    	currentJob = job;  	
    }
    
    //send OK until "."
    public void okSender(PrintStream send) throws IOException {
    	while(true)
        {
        	String serverString = lineReader(socket); //get server info or "DATA"
        	//System.out.println(serverString);
        	if(serverString.contains("DATA"))
        	{
        		send.write(sendToServer("OK"));
        	}
        	else if(serverString.equals("."))
            {
        		//Collections.sort(listServer, new CompareByCore());
                break;
            }
        	else
        	{
            	String[] serverDetails = serverString.split(" ");
                Server server = new Server(serverDetails);
                listServer.add(server);
                send.write(sendToServer("OK"));
        	}
        }
    	
    }
    
    //get server initial state
    public void getInitState(ArrayList<Server> list)
    {
    	String lookup = "";
    	for(Server ser : list)
    	{
    		if(!lookup.contains(ser.serverType))
    		{
    			lookup+=ser.serverType;
    			initServer.add(ser);
    		}
    	}
    }
    
    //get init server
    public Server getInitServer()
    {
    	for(Server ser : initServer)
    	{
    		if(ser.compareJob(currentJob))
    		{
    			return ser;
    		}
    	}
    	return null;
    }

    //constructor client
    
    public Client(String ip, int port, Integer algNumber) throws UnknownHostException, IOException
    {
        socket = new Socket(ip, port);
        
        System.out.println("Connected with server");
        System.out.println("--------------------------------------------------------------");
        System.out.println();
        
        PrintStream send = new PrintStream(socket.getOutputStream()); //send message

        //socket connection protocol
        ArrayList<String> al = new ArrayList<String>();
        
        al.add("HELO");
        al.add("AUTH user");
        al.add("REDY");
        
        for(String n : al)
        {
            send.write(sendToServer(n));
            String reply = lineReader(socket);
        }

        send.write(sendToServer("RESC All"));
        okSender(send);  //get all servers
        
        //get init state at the very beginning
        getInitState(listServer);
        
        //do the scheduling
        scheduleJob(send, socket, algNumber);
        
        while(true)
        {
        	send.write(sendToServer("REDY"));
        	String str = lineReader(socket);
        	if(str.contains("JOBN"))
        	{
        		send.write(sendToServer("RESC All"));
        		okSender(send);
        		scheduleJob(send, socket, algNumber);
        	}
        	else if(str.equals("NONE"))
        	{
        		break;
        	}
        }
        send.write(sendToServer("QUIT"));
        
        System.out.println();
		System.out.println("Connection closing");
		System.out.println("-------------------------------------------------------------");
        socket.close();
        send.close();
        
}
    public static void main(String[] args) throws UnknownHostException, IOException {
    	//for future algorithm implimantation [ff = 1; bf = 2; wf = 3]
    	
    	Integer algorithm = null;
    	if (args.length != 0)
    	{
            if(args[0].contains("-a") && args[1].contains("ff"))
                algorithm = 1;
            else if(args[0].contains("-a") && args[1].contains("bf"))
            {
            	algorithm = 2;
            }
            if(args[0].contains("-a") && args[1].contains("wf"))
                algorithm = 3;
    	}
    	

        Client cl = new Client("127.0.0.1", 8096, algorithm);

    }
}
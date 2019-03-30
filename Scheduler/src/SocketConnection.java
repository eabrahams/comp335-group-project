
import java.net.*;
import java.io.*;

public class SocketConnection {
	private static final int DEFAULT_PORT = 8096;
	private static final String LOCALHOST = "127.0.0.1";

	private Socket clientSocket;
	private PrintWriter out;
	private BufferedReader in;
	
	/* Opens a connection to a (hopefully) existing socket */
	public SocketConnection() {
		try {
			clientSocket = new Socket(LOCALHOST, DEFAULT_PORT);
			out = new PrintWriter(clientSocket.getOutputStream(), true);
			in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
		} catch(IOException error) {
			System.out.println(error);
		}
	}
	
	/* Send message, expecting a response. Return response. */
	public String sendMessage(String msg) {
		out.print(msg);
		out.flush();
		System.out.println("send: " + msg);
		StringBuffer sb = new StringBuffer();
		char[] buf = new char[1024];
		try {
			in.read(buf);
		} catch(IOException error) {
			System.err.println(error);
			System.exit(1);
		}
		sb.append(buf);
		return sb.toString().trim();
	}
}

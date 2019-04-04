
import java.net.*;
import java.io.*;

public class SocketConnection {
	private static final int DEFAULT_PORT = 8096;
	private static final String LOCALHOST = "127.0.0.1";

	private Socket clientSocket;
	private PrintWriter out;
	private BufferedReader in;
	
	/* Opens a connection to a (hopefully) existing socket */
	public SocketConnection(String host, int port) {
		try {
			clientSocket = new Socket(host, port);
			out = new PrintWriter(clientSocket.getOutputStream(), true);
			in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
		} catch(IOException error) {
			System.out.println(error);
			System.exit(1);
		}
	}

	public SocketConnection() {
		this(DEFAULT_PORT, LOCALHOST);
	}

	/* Send message, expecting a response. Return response. */
	public String sendMessage(String msg) {
		// Send message
		out.print(msg); // send characters
		out.flush(); // terminate message
		System.out.println("sent: " + msg);

		// Receive response
		StringBuffer sb = new StringBuffer();
		char[] buf = new char[1024];
		int length = 0;
		try {
			length = in.read(buf);
		} catch(IOException error) {
			System.err.println(error);
			System.exit(1);
		}
		sb.append(buf, 0, length);
		//String result = sb.toString().trim();
		String result = sb.toString();
		System.out.println("received: " + result);
		return result;
	}
}

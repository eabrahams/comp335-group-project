
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
		this(LOCALHOST, DEFAULT_PORT);
	}

	/* Send message over socket */
	public void sendMessage(String msg) {
		// Send message
		out.print(msg); // send characters
		out.flush(); // terminate message
		System.out.println("sent: " + msg);
	}

	/* receive (usually response) message over socket */
	public String receiveMessage() {
		StringBuffer sb = new StringBuffer();
		char[] buf = new char[1024];
		int length = 0;
		try {
			length = in.read(buf);
		} catch (IOException error) {
			System.err.println(error);
		}
		sb.append(buf, 0, length);
		String result = sb.toString();
		System.out.println("received: " + result);
		return result;
	}

	public boolean messageResponse(String message, String expectedResponse) {
		sendMessage(message);
		return expectedResponse.equals(receiveMessage());
	}
}

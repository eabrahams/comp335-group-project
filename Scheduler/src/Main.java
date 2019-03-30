
public class Main {
	private static boolean testResponse(String response, String expected) {
		if (!response.contentEquals(expected)) {
			System.err.println("bad response from server: " + response);
			return false;
		}
		return true;
	}
	
	/*
	 * send "HELO"
	 * receive "OK"
	 * send "AUTH comp335"
	 * receive "OK"
	 * Read system.xml
	 * 
	 * send "REDY"
	 * print response
	 * 
	 */
	public static void main(String[] args) {
		SocketConnection client = new SocketConnection();
		String resp1 = client.sendMessage("HELO");
		if (! testResponse(resp1, "OK"))
			System.exit(1);
		
		String resp2 = client.sendMessage("AUTH comp335");
		if (!testResponse(resp2, "OK"))
			System.exit(1);
		
		System.out.println("We read system.xml at this point\n");
		String resp3 = client.sendMessage("REDY");
		System.out.println("\n" + resp3);
		
		System.out.println("\n\nSUCCESS");
	}

}

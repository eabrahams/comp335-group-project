import java.io.*;
import java.util.*;

public class Main {
    public static void testConfig(String[] args)
    throws Exception {
        // testing
        SystemConfig config = SystemConfig.fromFile(new File("test-data/system.xml"));
        System.out.println(config.getServerTypes().size());
    }

	private static SocketConnection initClient() {
		SocketConnection client = new SocketConnection();
		client.messageResponse("HELO", "OK");
		client.messageResponse("AUTH comp335", "OK");

		return client;
	}

	public static void allToLargest(SocketConnection client) {
		SystemConfig config;
		try {
			config = SystemConfig.fromFile(new File("./system.xml"));
		} catch (IOException e) {
			System.err.println(e);
			return;
		}

		SystemConfig.ServerType largest = Collections.max(config.getServerTypes(), Comparator.comparing(s -> s.cores));

		while (true) {
			client.sendMessage("REDY");
			String response = client.receiveMessage();
			String[] jobData = response.split(" ");
			if (!jobData[0].equals("JOBN")) {
				if (!response.equals("NONE")) {
					System.err.println("Bad job string: " + String.join(" ", jobData));
				}
				break;
			}

			StringJoiner sj = new StringJoiner(" ");
			sj.add("SCHD");
			sj.add(jobData[2]);
			sj.add(largest.name);
			sj.add("0");
			client.sendMessage(sj.toString());
			if (!client.receiveMessage().equals("OK")) {
				break;
			}
		}
	}

	public static void main(String[] args) throws Exception {
		SocketConnection client = initClient();
		allToLargest(client);
		client.sendMessage("QUIT");
		client.close();
	}
}
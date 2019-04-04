import java.io.*;

public class Main {
    public static void main(String[] args)
    throws Exception {
        // testing
        SystemConfig config = SystemConfig.fromFile(new File("test-data/system.xml"));
        System.out.println(config.getServerTypes().size());
        config.getServer(config.getServerType("large"), 0).
    }
}

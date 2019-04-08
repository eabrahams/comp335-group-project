import org.junit.jupiter.api.*;

import java.io.*;
import java.nio.file.*;
import java.util.*;

class SystemConfigTest_Shared {
    private static final String path = "../test-data/defaultconfig-system.xml";
    private static SystemConfig config;
    
    @BeforeAll
    static void beforeAll()
    throws Exception {
        config = SystemConfig.fromFile(new File(path));
    }
    
    @Test
    void getServerTypes() {
        Collection<SystemConfig.ServerType> types = config.getServerTypes();
        Assertions.assertEquals(8, types.size());
    }
    
    @Test
    void getServerType() {
        Collection<SystemConfig.ServerType> types = config.getServerTypes();
        try {
            for(SystemConfig.ServerType type : types) {
                Assertions.assertEquals(type, config.getServerType(type.name), "Wrong server type was returned");
            }
        } catch(NoSuchElementException e) {
            Assertions.fail("Could not find own server type", e);
        }
    }
    
    @Test
    void getServers() {
        Collection<SystemConfig.ServerInfo> servers = config.getServers();
        Assertions.assertEquals(184, servers.size(), "Wrong number of servers");
    }
    
    /**
     * Verifies that getServers(ServerType) returns all servers of the correct type
     */
    @Test
    void getServers_ServerType_1() {
        for(SystemConfig.ServerType type : config.getServerTypes()) {
            for(SystemConfig.ServerInfo server : config.getServers(type)) {
                Assertions.assertEquals(type, server.type, "Returned server of wrong type");
            }
        }
    }
    
    /**
     * Verifies that getServers(ServerType) for each server type has full coverage of all servers
     */
    @Test
    void getServers_ServerType_2() {
        Collection<SystemConfig.ServerInfo> servers = config.getServers();
        int total = 0;
        for(SystemConfig.ServerType type : config.getServerTypes()) {
            List<SystemConfig.ServerInfo> group = config.getServers(type);
            Assertions.assertEquals(type.limit, group.size(), "Wrong number of server type instances");
            Assertions.assertTrue(servers.containsAll(group));
            total += group.size();
        }
        Assertions.assertEquals(servers.size(), total, "Server enumeration by type did not cover all servers");
    }
    
    @Test
    void getServer() {
        for(SystemConfig.ServerType type : config.getServerTypes()) {
            for(int i = 0; i < type.limit; ++i) {
                SystemConfig.ServerInfo server = config.getServer(type, i);
                Assertions.assertEquals(type, server.type, "Returned server of wrong type");
                Assertions.assertEquals(i, server.id, "Server-to-id mapping inconsistent");
            }
        }
    }
    
    @Test
    void fromStream() {
        try {
            SystemConfig.fromStream(new ByteArrayInputStream(Files.readAllBytes(Paths.get(path))));
        } catch(IOException e) {
            throw new RuntimeException(e);
        } catch(Exception e) {
            Assertions.fail("Could not initialize as stream from defaultconfig-system.xml", e);
        }
    }
    
    @Test
    void fromFile() {
        try {
            SystemConfig.fromFile(new File(path));
        } catch(IOException e) {
            throw new RuntimeException(e);
        } catch(Exception e) {
            Assertions.fail("Could not initialize as file from defaultconfig-system.xml", e);
        }
    }
    
    @Test
    void fromString() {
        try {
            String content = new String(Files.readAllBytes(Paths.get(path)));
            SystemConfig.fromString(content);
        } catch(IOException e) {
            throw new RuntimeException(e);
        } catch(Exception e) {
            Assertions.fail("Could not initialize as file from defaultconfig-system.xml", e);
        }
    }

    @Test
    void SystemConfig_default() {
        try {
            SystemConfig.fromFile(new File("../test-data/defaultconfig-system.xml"));
        } catch(IOException e) {
            throw new RuntimeException(e);
        } catch(Exception e) {
            Assertions.fail("Could not initialize from defaultconfig", e);
        }
    }
    
    @Test
    void SystemConfig_simple1() {
        try {
            SystemConfig.fromFile(new File("../test-data/config_simple1-system.xml"));
        } catch(IOException e) {
            throw new RuntimeException(e);
        } catch(Exception e) {
            Assertions.fail("Could not initialize from config_simple1", e);
        }
    }
    
    @Test
    void SystemConfig_simple2() {
        try {
            SystemConfig.fromFile(new File("../test-data/config_simple2-system.xml"));
        } catch(IOException e) {
            throw new RuntimeException(e);
        } catch(Exception e) {
            Assertions.fail("Could not initialize from config_simple2", e);
        }
    }
    
    @Test
    void SystemConfig_simple5() {
        try {
            SystemConfig.fromFile(new File("../test-data/config_simple5-system.xml"));
        } catch(IOException e) {
            throw new RuntimeException(e);
        } catch(Exception e) {
            Assertions.fail("Could not initialize from config_simple5", e);
        }
    }
    
    @Test
    void SystemConfig_simple6() {
        try {
            SystemConfig.fromFile(new File("../test-data/config_simple6-system.xml"));
        } catch(IOException e) {
            throw new RuntimeException(e);
        } catch(Exception e) {
            Assertions.fail("Could not initialize from config_simple6", e);
        }
    }
}
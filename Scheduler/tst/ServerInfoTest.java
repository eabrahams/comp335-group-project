import org.junit.jupiter.api.*;

import java.io.File;

import static org.junit.jupiter.api.Assertions.*;

class ServerInfoTest_Instanced {
    private static SystemConfig config;
    
    @BeforeEach
    void beforeEach()
    throws Exception {
        config = SystemConfig.fromFile(new File("../test-data/defaultconfig-system.xml"));
    }
    
    /**
     * Verify that the initial state of the ServerInfo is based off the ServerType
     */
    @Test
    void ServerInfo_1() {
        for(SystemConfig.ServerInfo server : config.getServers()) {
            Assertions.assertEquals(server.type.cores, server.getAvailableCores());
            Assertions.assertEquals(server.type.memory, server.getAvailableMemory());
            Assertions.assertEquals(server.type.disk, server.getAvailableDisk());
        }
    }
    
    /**
     * Verifies that the update method changes internal values
     */
    @Test
    void update_1() {
        SystemConfig.ServerInfo server = config.getServers().iterator().next();
        server.update(SystemConfig.ServerState.Unavailable, 9999, 0, 0, 0);
        Assertions.assertEquals(SystemConfig.ServerState.Unavailable, server.getState());
        Assertions.assertEquals(9999, server.getAvailableTime());
        Assertions.assertEquals(0, server.getAvailableCores());
        Assertions.assertEquals(0, server.getAvailableMemory());
        Assertions.assertEquals(0, server.getAvailableDisk());
    }
    
    /**
     * Verify that update rejects negative values
     */
    @Test
    void update_2() {
        SystemConfig.ServerInfo server = config.getServers().iterator().next();
        try {
            server.update(server.getState(), -4, -3, -2, -1);
        } catch(IllegalArgumentException e) {
            return;
        }
        Assertions.fail("Update failed to reject negatives");
    }
    
    /**
     * Verify that update rejects resource values out of the range of the server type
     */
    @Test
    void update_3() {
        SystemConfig.ServerInfo server = config.getServers().iterator().next();
        try {
            server.update(server.getState(),
                          server.getAvailableTime(),
                          server.type.cores + 1,
                          server.type.memory + 1,
                          server.type.disk + 1
            );
        } catch(IllegalArgumentException e) {
            return;
        }
        Assertions.fail("Update failed to reject out-of-bounds resources");
    }
    
    /**
     * Verify that releasing resources can't result in end totals above the maximum allowed by type
     */
    @Test
    void releaseResources_int_int_int_1() {
        SystemConfig.ServerInfo server = config.getServers().iterator().next();
        server.update(server.getState(), server.getAvailableTime(), 0, 0, 0);
        server.releaseResources(2 * server.type.cores, 2 * server.type.memory, 2 * server.type.disk);
        Assertions.assertEquals(server.type.cores, server.getAvailableCores());
        Assertions.assertEquals(server.type.memory, server.getAvailableMemory());
        Assertions.assertEquals(server.type.disk, server.getAvailableDisk());
    }
    
    /**
     * Verify that releasing no resources causes no change
     */
    @Test
    void releaseResources_int_int_int_2() {
        SystemConfig.ServerInfo server = config.getServers().iterator().next();
        server.releaseResources(0, 0, 0);
        Assertions.assertEquals(server.type.cores, server.getAvailableCores());
        Assertions.assertEquals(server.type.memory, server.getAvailableMemory());
        Assertions.assertEquals(server.type.disk, server.getAvailableDisk());
        server.update(server.getState(), server.getAvailableTime(), 0, 0, 0);
        server.releaseResources(0, 0, 0);
        Assertions.assertEquals(0, server.getAvailableCores());
        Assertions.assertEquals(0, server.getAvailableMemory());
        Assertions.assertEquals(0, server.getAvailableDisk());
    }
    
    /**
     * Verify that releaseResources rejects negative arguments
     */
    @Test
    void releaseResources_int_int_int_3() {
        SystemConfig.ServerInfo server = config.getServers().iterator().next();
        try {
            server.releaseResources(-1, -2, -3);
        } catch(IllegalArgumentException e) {
            return;
        }
        Assertions.fail("ReleaseResources failed to reject negatives");
    }
    
    /**
     * Verify that servers are equal to themselves
     */
    @Test
    void equals_1() {
        for(SystemConfig.ServerInfo server : config.getServers()) Assertions.assertEquals(server, server);
    }
    
    /**
     * Verify that servers only compare equality based on type and id
     */
    @Test
    void equals_2() {
        for(SystemConfig.ServerInfo server : config.getServers()) {
            Assertions.assertEquals(server, server.update(server.getState(), server.getAvailableTime(), 0, 0, 0));
        }
    }
}
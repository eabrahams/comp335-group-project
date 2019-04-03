import org.w3c.dom.*;
import org.xml.sax.SAXException;

import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import java.io.*;
import java.util.*;

public final class SystemConfig {
    public final class ServerType {
        final String name;
        final int    limit;
        final int    bootTime;
        final float  hourlyRate;
        final int    cores;
        final int    memory;
        final int    disk;
        
        /**
         * Instantiates and validates a ServerType, describing a set of instanced resources
         *
         * @param name       the name of the server type
         * @param limit      a non-negative integer indicating how many can coexist
         * @param bootTime   a non-negative integer indicating how long it takes to start an instance
         * @param hourlyRate a non-negative rational number representing the cost of running an instance for an hour
         * @param cores      a positive integer representing how many cores the server type offers
         * @param memory     a positive integer representing how much memory the server type offers
         * @param disk       a positive integer representing how much disk the server type offers
         * @throws IllegalArgumentException when any of the arguments fail to meet the specified criteria
         */
        private ServerType(String name, int limit, int bootTime, float hourlyRate, int cores, int memory, int disk)
        throws IllegalArgumentException {
            if(name == null) throw new NullPointerException("`name` cannot be null");
            if(limit < 0) throw new IllegalArgumentException("`limit` cannot be negative");
            if(bootTime < 0) throw new IllegalArgumentException("`bootTime` cannot be negative");
            if(hourlyRate < 0.0 || Float.isNaN(hourlyRate) || Float.isInfinite(hourlyRate)) {
                throw new IllegalArgumentException("`hourlyRate` must be a non-negative rational number");
            }
            if(cores <= 0) throw new IllegalArgumentException("`coreCount` must be positive");
            if(memory <= 0) throw new IllegalArgumentException("`memory` must be positive");
            if(disk <= 0) throw new IllegalArgumentException("`disk` must be positive");
            this.name = name;
            this.limit = limit;
            this.bootTime = bootTime;
            this.hourlyRate = hourlyRate;
            this.cores = cores;
            this.memory = memory;
            this.disk = disk;
        }
        
        /**
         * Instantiates a ServerType from an XML Element, used internally for parsing
         *
         * @param element an XML element
         * @throws IllegalArgumentException when the element does not represent a valid server type
         */
        private ServerType(Element element)
        throws IllegalArgumentException {
            this(element.getAttribute("type"),
                 Integer.parseInt(element.getAttribute("limit")),
                 Integer.parseInt(element.getAttribute("bootupTime")),
                 Float.parseFloat(element.getAttribute("hourlyRate")),
                 Integer.parseInt(element.getAttribute("coreCount")),
                 Integer.parseInt(element.getAttribute("memory")),
                 Integer.parseInt(element.getAttribute("disk"))
            );
        }
        
        @Override
        public boolean equals(Object obj) {
            if(this == obj) {
                return true;
            } else if(obj instanceof ServerType) {
                ServerType other = (ServerType) obj;
                return this.name.equals(other.name)
                       && this.limit == other.limit
                       && this.bootTime == other.bootTime
                       && this.hourlyRate == other.hourlyRate
                       && this.cores == other.cores
                       && this.memory == other.memory
                       && this.disk == other.disk;
            } else {
                return false;
            }
        }
        
        @Override
        public int hashCode() {
            return name.hashCode() ^ limit ^ bootTime ^ Float.hashCode(hourlyRate) ^ cores ^ memory ^ disk;
        }
    }
    
    
    public final class ServerInfo {
        final   ServerType type;
        final   int        id;
        private int        availableCores;
        private int        availableMemory;
        private int        availableDisk;
        
        private ServerInfo(ServerType type, int id) {
            this.type = type;
            this.id = id;
            this.availableCores = type.cores;
            this.availableMemory = type.memory;
            this.availableDisk = type.disk;
        }
        
        public void releaseResources(int cores, int memory, int disk) {
            if(cores >= 0) availableCores = Math.min(availableCores + cores, type.cores);
            if(memory >= 0) availableMemory = Math.min(availableMemory + memory, type.memory);
            if(disk >= 0) availableDisk = Math.min(availableDisk + disk, type.disk);
        }
        
        public boolean tryReserveResources(int cores, int memory, int disk) {
            if(0 <= cores
               && cores <= availableCores
               && 0 <= memory
               && memory <= availableMemory
               && 0 <= disk
               && disk <= availableDisk) {
                availableCores -= cores;
                availableMemory -= memory;
                availableDisk -= disk;
                return true;
            } else {
                return false;
            }
        }
        
        public int getAvailableCores() {
            return availableCores;
        }
        
        public int getAvailableMemory() {
            return availableMemory;
        }
        
        public int getAvailableDisk() {
            return availableDisk;
        }
        
        @Override
        public boolean equals(Object obj) {
            if(this == obj) {
                return true;
            } else if(obj instanceof ServerInfo) {
                ServerInfo other = (ServerInfo) obj;
                return this.type == other.type && this.id == other.id;
            } else {
                return false;
            }
        }
        
        @Override
        public int hashCode() {
            return this.type.hashCode() ^ id;
        }
    }
    
    
    public final Collection<ServerType> serverTypes;
    public final Collection<ServerInfo> servers;
    
    /**
     * Instantiates a SystemConfig from an XML Document, performing validation
     *
     * @param doc the XML Document
     * @throws IllegalArgumentException when the document does not represent a valid system configuration
     */
    public SystemConfig(Document doc)
    throws IllegalArgumentException {
        if(doc == null) throw new NullPointerException("`doc` cannot be null");
        Element root = doc.getDocumentElement();
        if(!root.getNodeName().equals("system")) {
            throw new IllegalArgumentException(String.format("Unexpected element '%s' instead of 'system'",
                                                             root.getNodeName()
            ));
        }
        NodeList nodes = root.getChildNodes();
        Collection<ServerType> serverTypes = new ArrayList<>();
        for(int i = 0; i < nodes.getLength(); ++i) {
            Node node = nodes.item(i);
            switch(node.getNodeType()) {
                case Node.ELEMENT_NODE:
                    String nodeName = node.getNodeName();
                    if(nodeName.equals("servers")) {
                        NodeList servers = node.getChildNodes();
                        for(int j = 0; j < servers.getLength(); ++j) {
                            Node server = servers.item(j);
                            switch(server.getNodeType()) {
                                case Node.ELEMENT_NODE:
                                    if(server.getNodeName().equals("server")) {
                                        serverTypes.add(new ServerType((Element) server));
                                    } else {
                                        throw new IllegalArgumentException(String.format(
                                                "Unexpected element '%s' under 'servers'",
                                                server.getNodeName()
                                        ));
                                    }
                                case Node.TEXT_NODE:
                                case Node.COMMENT_NODE:
                                    continue;
                                default:
                                    throw new IllegalArgumentException("Unexpected node type under 'servers'");
                            }
                        }
                    } else {
                        throw new IllegalArgumentException(String.format("Unexpected element '%s' under 'system'",
                                                                         nodeName
                        ));
                    }
                case Node.TEXT_NODE:
                case Node.COMMENT_NODE:
                    continue;
                default:
                    throw new IllegalArgumentException("Unexpected node type under 'system'");
            }
        }
        this.serverTypes = Collections.unmodifiableCollection(serverTypes);
        Collection<ServerInfo> servers = new ArrayList<>();
        for(ServerType type : serverTypes) {
            for(int i = 0; i < type.limit; ++i) {
                servers.add(new ServerInfo(type, i));
            }
        }
        this.servers = Collections.unmodifiableCollection(servers);
    }
    
    /**
     * Instantiates a SystemConfig from a stream containing an XML Document, performing validation
     *
     * @param stream the stream containing the XML document
     * @return a valid system configuration
     * @throws IOException              when there was a problem reading from the stream
     * @throws IllegalArgumentException when the stream does not represent a valid system configuration
     */
    public static SystemConfig fromStream(InputStream stream)
    throws IOException, IllegalArgumentException {
        try {
            Document doc = DocumentBuilderFactory.newInstance().newDocumentBuilder().parse(stream);
            return new SystemConfig(doc);
        } catch(ParserConfigurationException e) {
            // should never happen because we use the default configuration
            throw new RuntimeException("Default parser configuration failed!", e);
        } catch(SAXException e) {
            throw new IllegalArgumentException("Invalid XML format", e);
        }
    }
    
    /**
     * Instantiates a SystemConfig from a file containing an XML Document, performing validation
     *
     * @param file the file containing an XML Document
     * @return a valid system configuration
     * @throws IOException              when there was a problem reading the file
     * @throws IllegalArgumentException when the file does not represent a valid system configuration
     */
    public static SystemConfig fromFile(File file)
    throws IOException, IllegalArgumentException {
        InputStream stream = new FileInputStream(file);
        return SystemConfig.fromStream(stream);
    }
    
    /**
     * Instantiates a SystemConfig from a file containing an XML Document, performing validation
     *
     * @param path the path to a containing an XML Document
     * @return a valid system configuration
     * @throws IOException              when there was a problem reading the file
     * @throws IllegalArgumentException when the file does not represent a valid system configuration
     */
    public static SystemConfig fromFile(String path)
    throws IOException, IllegalArgumentException {
        InputStream stream = new FileInputStream(new File(path));
        return SystemConfig.fromStream(stream);
    }
    
    /**
     * Instantiates a SystemConfig from a string containing an XML Document, performing validation
     *
     * @param str the string containing the XML Document
     * @return a valid system configuration
     * @throws IllegalArgumentException when the string does not represent a valid system configuration
     */
    public static SystemConfig fromString(String str)
    throws IllegalArgumentException {
        try {
            InputStream stream = new ByteArrayInputStream(str.getBytes());
            return SystemConfig.fromStream(stream);
        } catch(IOException e) {
            // should never happen because we aren't really doing IO
            throw new RuntimeException("Could not process IO on String!", e);
        }
    }
}

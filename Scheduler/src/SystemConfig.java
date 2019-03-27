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
        final int    coreCount;
        final int    memory;
        final int    disk;
        
        public ServerType(String name, int limit, int bootTime, float hourlyRate, int coreCount, int memory, int disk)
        throws IllegalArgumentException {
            if(name == null) throw new NullPointerException("`name` cannot be null");
            if(limit < 0) throw new IllegalArgumentException("`limit` cannot be negative");
            if(bootTime < 0) throw new IllegalArgumentException("`bootTime` cannot be negative");
            if(hourlyRate < 0.0 || Float.isNaN(hourlyRate) || Float.isInfinite(hourlyRate)) {
                throw new IllegalArgumentException("`hourlyRate` must be a non-negative rational number");
            }
            if(coreCount <= 0) throw new IllegalArgumentException("`coreCount` must be positive");
            if(memory <= 0) throw new IllegalArgumentException("`memory` must be positive");
            if(disk <= 0) throw new IllegalArgumentException("`disk` must be positive");
            this.name = name;
            this.limit = limit;
            this.bootTime = bootTime;
            this.hourlyRate = hourlyRate;
            this.coreCount = coreCount;
            this.memory = memory;
            this.disk = disk;
        }
        
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
                       && this.coreCount == other.coreCount
                       && this.memory == other.memory
                       && this.disk == other.disk;
            } else {
                return false;
            }
        }
        
        @Override
        public int hashCode() {
            return name.hashCode() ^ limit ^ bootTime ^ Float.hashCode(hourlyRate) ^ coreCount ^ memory ^ disk;
        }
    }
    
    
    public final Collection<ServerType> serverTypes = new ArrayList<>();
    
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
        for(int i = 0; i < nodes.getLength(); ++i) {
            Node node = nodes.item(i);
            switch(node.getNodeType()) {
                case Node.ELEMENT_NODE:
                    Element element = (Element) node;
                    String nodeName = element.getNodeName();
                    if(nodeName.equals("servers")) {
                        NodeList servers = element.getChildNodes();
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
    }
    
    public static SystemConfig fromFile(File file)
    throws IOException, IllegalArgumentException {
        try {
            Document doc = DocumentBuilderFactory.newInstance().newDocumentBuilder().parse(file);
            return new SystemConfig(doc);
        } catch(ParserConfigurationException e) {
            throw new RuntimeException("Default parser configuration failed!", e);
        } catch(SAXException e) {
            throw new IllegalArgumentException("Invalid XML format", e);
        }
    }
    
    public static SystemConfig fromStream(InputStream stream)
    throws IOException, IllegalArgumentException {
        try {
            Document doc = DocumentBuilderFactory.newInstance().newDocumentBuilder().parse(stream);
            return new SystemConfig(doc);
        } catch(ParserConfigurationException e) {
            throw new RuntimeException("Default parser configuration failed!", e);
        } catch(SAXException e) {
            throw new IllegalArgumentException("Invalid XML format", e);
        }
    }
    
    public static SystemConfig fromString(String str)
    throws IllegalArgumentException {
        try {
            InputStream stream = new ByteArrayInputStream(str.getBytes());
            return SystemConfig.fromStream(stream);
        } catch(IOException e) {
            throw new RuntimeException("Could not process IO on String!", e);
        }
    }
}

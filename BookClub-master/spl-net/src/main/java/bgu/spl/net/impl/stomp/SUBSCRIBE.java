package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.ConnectionsImpl;

public class SUBSCRIBE implements Command{
    private String destination;
    private String id;
    private String receipt;
    private Integer connectionId;

    public SUBSCRIBE (String _destination ,String _id, String _receipt,Integer _connectionId ){
        destination = _destination;
        id=_id;
        receipt = _receipt;
        connectionId=_connectionId;
    }

    @Override
    public String execute(ConnectionsImpl connections,int connectionId) {
        connections.addUserToTopic(destination,id,receipt,connectionId);
        return "OK";
    }
}

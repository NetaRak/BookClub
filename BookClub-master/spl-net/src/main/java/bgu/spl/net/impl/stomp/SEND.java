package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.ConnectionsImpl;

public class SEND implements Command{
    private String destination;
    private String body;
    private MessageProtocolImp protocol;

    public SEND (String _destination ,String _body ,MessageProtocolImp _protocol ){
        destination = _destination;
        body=_body;
        protocol=_protocol;
    }

    @Override
    public String execute(ConnectionsImpl connections,int connectionId) {
        connections.send(destination,body,connectionId);
        return "OK";
    }
}
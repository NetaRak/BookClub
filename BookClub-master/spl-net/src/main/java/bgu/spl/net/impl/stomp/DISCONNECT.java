package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.ConnectionsImpl;

import java.io.IOException;

public class DISCONNECT implements Command{
    private String receipt;
    private MessageProtocolImp protocol;

    public DISCONNECT (String _receiptD ,MessageProtocolImp _protocol ){
        receipt = _receiptD;
        protocol=_protocol;
    }

    @Override
    public String execute(ConnectionsImpl connections,int connectionId) throws IOException {
        connections.disconnect(connectionId,receipt);
        return "OK";
    }
}
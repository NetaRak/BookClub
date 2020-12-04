package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.ConnectionsImpl;

public class UNSUBSCRIBE  implements Command {
    private String subId;
    private MessageProtocolImp protocol;
    private String receipt;

    public UNSUBSCRIBE(String _id ,MessageProtocolImp _protocol ,String _receipt){
        subId=_id;
        protocol=_protocol;
        receipt=_receipt;
    }
    @Override
    public String execute(ConnectionsImpl connections, int connectionId) {
        connections.unSubscribe(subId,connectionId,receipt);
        return "OK";
    }
}

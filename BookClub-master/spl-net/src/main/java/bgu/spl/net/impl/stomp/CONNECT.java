package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.ConnectionsImpl;

import java.io.IOException;

public class CONNECT implements Command {
    private String userName;
    private String passCode;
    private String version;
    private MessageProtocolImp protocol;

    public CONNECT (String _version,String _userName , String _passcode, MessageProtocolImp _protocol){
        version=_version;
        userName = _userName;
        passCode=_passcode;
        protocol=_protocol;
    }

    //TODO - figure out the RETURN!
    @Override
    public String execute(ConnectionsImpl connections,int connectionId) throws IOException {
        if (connections.addUser(version,userName,passCode,connectionId)){
            return "OK";
        }
        else {
            return "ERROR";
        }

    }
}

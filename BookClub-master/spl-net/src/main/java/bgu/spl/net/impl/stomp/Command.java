package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.ConnectionsImpl;

import java.io.IOException;

public interface Command {

    public String execute(ConnectionsImpl connections, int connectionId) throws IOException;
}

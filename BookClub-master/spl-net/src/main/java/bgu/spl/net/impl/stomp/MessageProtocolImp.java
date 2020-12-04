package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.impl.stomp.Command;
import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.ConnectionsImpl;
import bgu.spl.net.impl.stomp.Pair;

import java.io.IOException;
import java.util.LinkedList;

public class MessageProtocolImp implements StompMessagingProtocol {
    private boolean isTerminate=false;
    private int connectionId;
    private ConnectionsImpl<String> connections;
    private LinkedList<Pair<Integer,String>> topics;

    @Override
    public void start(int _connectionId, ConnectionsImpl _connections) {
        connectionId=_connectionId;
        connections=_connections;
        topics=new LinkedList<>();
    }
    public int getConnectionId(){
        return connectionId;
    }
    @Override
    public void process(Object message) throws IOException {
        try {
            System.out.println("Server has entered process with messagse: " + message);
            String[] messages = ((String) message).split("\n");
            String command = messages[0];
            switch (command) {
                case "CONNECT":
                    String version = messages[1].split(":")[1];
                    String userName = messages[3].split(":")[1];
                    String passcode = messages[4].split(":")[1];

                    Command connect = new CONNECT(version, userName, passcode, this);
                    String toSendCON = connect.execute(connections, connectionId);
                    if (toSendCON.equals("ERROR")) {
                        isTerminate = true;
                    }

                    break;

                case "SUBSCRIBE":
                    String destination = messages[1].split(":")[1];
                    String id = messages[2].split(":")[1];
                    String[] receiptArr = messages[3].split(":");
                    String receipt = receiptArr[1];
                    Command subscribe = (Command) new SUBSCRIBE(destination, id, receipt, connectionId);
                    subscribe.execute(connections, connectionId);
                    break;

                case "SEND":
                    String destination1 = messages[1].split(":")[1];
                    String body = messages[3];
                    System.out.println("got SEND:" + body);
                    Command send = new SEND(destination1, body, this);
                    send.execute(connections, connectionId);
                    break;

                case "DISCONNECT":
                    String receiptD = messages[1].split(":")[1];
                    Command disConnect = new DISCONNECT(receiptD, this);
                    disConnect.execute(connections, connectionId);
                    isTerminate = true;
                    break;

                case "UNSUBSCRIBE":
                    String subId = messages[1].split(":")[1];
                    String receiptUn = messages[2].split(":")[1];
                    Command unSub = new UNSUBSCRIBE(subId, this, receiptUn);
                    unSub.execute(connections, connectionId);
                    break;

            }
        }
        catch (Exception ignored){}
        if(connections.getErrorHasBeenSent())
            isTerminate=true;
    }

    @Override
    public boolean shouldTerminate() {
        return isTerminate;
    }
}

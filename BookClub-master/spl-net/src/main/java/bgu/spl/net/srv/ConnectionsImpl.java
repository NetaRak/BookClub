package bgu.spl.net.srv;

import bgu.spl.net.impl.stomp.dataBase;

import java.io.IOException;

public class ConnectionsImpl<T> implements Connections<T> {
    dataBase dataBase = bgu.spl.net.impl.stomp.dataBase.getInstance();

    private static class SingleHolder {
        private static ConnectionsImpl instance = new ConnectionsImpl();
    }

    public static ConnectionsImpl getInstance() {
        return SingleHolder.instance;
    }

    //TODO-return ????!!!!
    @Override
    public boolean send(int connectionId, T msg) {
        return dataBase.send(connectionId, msg);
    }

    @Override
    public void send(String channel, T msg, int connectionId) {
        dataBase.sendToTopic(channel, msg, connectionId);
    }

    public void unSubscribe(String subId, int connectionId, String receiptUn) {
        dataBase.unSubscribe(subId, connectionId, receiptUn);
    }

    @SuppressWarnings("unchecked")
    public int push(ConnectionHandler<T> handler) {
        return dataBase.addToConMap((ConnectionHandler<String>) handler);
    }

    @Override
    public void disconnect(int connectionId, String receipt) throws IOException {
        dataBase.disconnect(connectionId, receipt);
    }

    public boolean addUser(String version, String userName, String passCode, int connectionId) throws IOException {
        return dataBase.addUser(version, userName, passCode, connectionId);

    }

    public void addUserToTopic(String topic, String subId, String receipt, int connectionId) {
        dataBase.addUserToTopic(topic, subId, receipt, connectionId);
    }

    public int getconId() {
        return dataBase.getConId();
    }

    public boolean getErrorHasBeenSent() {
        return dataBase.isErrorHasBeenSent();
    }

}

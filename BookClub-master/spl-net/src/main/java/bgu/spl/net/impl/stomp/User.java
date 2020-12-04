package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.ConnectionHandler;

import java.util.concurrent.ConcurrentHashMap;

public class User<T> {
    private String name;
    private String passCode;
    private Integer connectionId;
    private ConnectionHandler<T> connectionHandler;
    private boolean isActive;
    private ConcurrentHashMap<String,String> topicsAndSubscribeId;

    public User (String _name, String _passCode, Integer _connectionId, ConnectionHandler<T> _connectionHandler){
        name=_name;
        passCode=_passCode;
        connectionId=_connectionId;
        connectionHandler=_connectionHandler;
        isActive=true;
        topicsAndSubscribeId = new ConcurrentHashMap<>();
    }

    public String getName() {
        return name;
    }

    public String getPassCode() {
        return passCode;
    }

    public Integer getConnectionId() {
        return connectionId;
    }

    public void setConnectionId(Integer connectionId) {
        this.connectionId = connectionId;
    }
    public void setConnectionHandler(ConnectionHandler handler){
        connectionHandler=handler;
    }
    public ConnectionHandler<T> getConnectionHandler() {
        return connectionHandler;
    }
    public String findTopicBySubId(String id){
        for (String topic : topicsAndSubscribeId.keySet()){
            if (topicsAndSubscribeId.get(topic).compareTo(id)==0){
                return topic;
            }
        }
        return null;
    }
    public void removeSub(String topic){
        topicsAndSubscribeId.remove(topic);
    }

    public boolean isActive() {
        return isActive;
    }
    public String getSubIdByTopic (String topic){
        return topicsAndSubscribeId.get(topic);
    }
    public void setIsActive(boolean _isActive) {
        isActive=_isActive;
    }
    public ConcurrentHashMap<String, String> getTopicsAndSubscribeId() {
        return topicsAndSubscribeId;
    }
    public void addSubToMap(String topic,String subId){
        topicsAndSubscribeId.putIfAbsent(topic,subId);
    }
    public void resetTopicsMap (){
        topicsAndSubscribeId = new ConcurrentHashMap<>();
    }

}

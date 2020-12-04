package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.ConnectionHandler;

import java.io.IOException;
import java.util.LinkedList;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;

public class dataBase {
    private AtomicInteger id = new AtomicInteger(0);
    private AtomicInteger messageId = new AtomicInteger(0);
    private ConcurrentHashMap<Integer, ConnectionHandler<String>> connectionHandlersMap = new ConcurrentHashMap<>();
    private ConcurrentHashMap<String, LinkedList<Pair<User<String>, String>>> topicsAndUsersAndSub = new ConcurrentHashMap<>();
    private Set<User<String>> users = ConcurrentHashMap.newKeySet();
    private ConcurrentHashMap<Integer, User<String>> UsersByConnectionId = new ConcurrentHashMap<>();
    private boolean errorHasBeenSent = false;

    public boolean isErrorHasBeenSent() {
        return errorHasBeenSent;
    }

    private static class SingleHolder {
        private static dataBase instance = new dataBase();
    }


    public int addToConMap(ConnectionHandler<String> handler) {
        int newId = id.getAndIncrement();
        connectionHandlersMap.put(newId, handler);
        return newId;
    }

    public int getConId() {
        return id.intValue();
    }

    public void disconnect(int connectionId, String receipt) {
        User<String> user = UsersByConnectionId.get(connectionId);
        if (user == null) {
            return;
        }

        removeUsers(connectionId, user);

        String response;
        response = "RECEIPT\nreceipt-id:" + receipt + "\n\n\0";
        connectionHandlersMap.get(connectionId).send(response);
        connectionHandlersMap.remove(connectionId);
        UsersByConnectionId.remove(connectionId);
        user.setConnectionHandler(null);
        user.setConnectionId(-1);
        user.resetTopicsMap();
    }

    private void removeUsers(int connectionId, User<String> user) {
        user.setIsActive(false);
        for (Object topic : user.getTopicsAndSubscribeId().keySet()) {
            String topicToRemove = (String) topic;
            for (Pair<User<String>, String> userTo : topicsAndUsersAndSub.get(topicToRemove)) {
                if (userTo.getFirst().getConnectionId() == connectionId) {
                    topicsAndUsersAndSub.get(topicToRemove).remove(userTo);
                }
                if (topicsAndUsersAndSub.get(topicToRemove).size() == 0) {
                    topicsAndUsersAndSub.remove(topicToRemove);
                }
            }
        }
    }

    public static dataBase getInstance() {
        return SingleHolder.instance;
    }

    ConcurrentHashMap<String, Object> locksmap = new ConcurrentHashMap<>();

    public boolean addUser(String version, String userName, String passCode, int connectionId) throws IOException {

        locksmap.putIfAbsent(userName, new Object());

        synchronized (locksmap.get(userName)) {
            final AtomicBoolean isExist = new AtomicBoolean(false);
            for (User<String> user : users) {
                if (user.getName().equals(userName))
                    isExist.set(true);
            }

            if (!isExist.get()) { //if user does not exist
                User<String> userToAdd = new User<>(userName, passCode, connectionId, connectionHandlersMap.get(connectionId));
                UsersByConnectionId.put(connectionId, userToAdd);
                users.add(userToAdd);
                String msg = "CONNECTED\nversion:" + version + "\n\n\0";
                System.out.println(userToAdd.getName() + " is CONNECTED. sending a msg: " + msg);
                send(connectionId, msg);
                return true;
            }

        }
        for (User<String> user : users) {
            if (user.getName().equals(userName)) {
                if (user.getPassCode().equals(passCode)) { //if the password is correct
                    boolean didit = false;
                    synchronized (locksmap.get(userName)) {
                        if (didit = !user.isActive()) {
                            user.setIsActive(true);
                        }
                    }
                    if (didit) {
                        user.setConnectionId(connectionId);
                        user.setConnectionHandler(connectionHandlersMap.get(connectionId));
                        UsersByConnectionId.put(connectionId, user);
                        String msg = "CONNECTED\nversion:" + version + "\n\n\0";
                        System.out.println(user.getName() + " is CONNECTED. sending a msg: " + msg);
                        send(connectionId, msg);
                        return true;
                    }
                    sendError(connectionId, null, "User already logged in");
                    return false;
                } else {
                    sendError(connectionId, null, "Wrong password");
                    return false;
                }
//                    break;
            }
        }

        //if the connection was successful
        return false;
    }

    public void sendError(int connectionId, String receipt, String message) {
        errorHasBeenSent = true;
        String error;
        if (receipt != null) {
            error = "ERROR\nreceipt-id:" + receipt + "\n" + "message:" + message + "\n\n\0";
        }else{
                error = "ERROR\nmessage:" + message + "\n\n\0";
            }
//            synchronized (locksmap.get(UsersByConnectionId.get(connectionId))){
                connectionHandlersMap.get(connectionId).send(error);
//            }
        User<String> user = UsersByConnectionId.get(connectionId);
        if (user != null) {
            removeUsers(connectionId, user);
            connectionHandlersMap.remove(connectionId);
            UsersByConnectionId.remove(connectionId);
            user.setConnectionHandler(null);
            user.setConnectionId(-1);
            user.resetTopicsMap();
        }
    }

    public void addUserToTopic(String topic, String subId, String receipt, int connectionId) {

        if (topicsAndUsersAndSub.containsKey(topic)) { //if the topic(genere) already exist
            boolean isUserThere = false;
            for (Pair<User<String>, String> pair : topicsAndUsersAndSub.get(topic)) {
                if (pair.getFirst() == UsersByConnectionId.get(connectionId))
                    isUserThere = true;
            }
            if (!isUserThere) {
                Pair<User<String>, String> newPair = new Pair<>(UsersByConnectionId.get(connectionId), subId);
                topicsAndUsersAndSub.get(topic).add(newPair);
            }
        } else { //the topic does not exist -> we need to add it
            LinkedList<Pair<User<String>, String>> listToTopic = new LinkedList<>();
            Pair<User<String>, String> newPair = new Pair<>(UsersByConnectionId.get(connectionId), subId);
            listToTopic.add(newPair);
            topicsAndUsersAndSub.put(topic, listToTopic);
        }
        UsersByConnectionId.get(connectionId).addSubToMap(topic, subId);

        if (receipt != null) {
            String response;
            response = "RECEIPT\nreceipt-id:" + receipt + "\n\n\u0000";
            connectionHandlersMap.get(connectionId).send(response);
        } else {
            System.out.println("ERROR: RECEIPT IS NULL");
        }
    }

    public void sendToTopic(String destination, Object body, int connectionId) {
        String senderSubId = UsersByConnectionId.get(connectionId).getSubIdByTopic(destination);
        for (Pair<User<String>, String> userAndSub : topicsAndUsersAndSub.get(destination)) {
            String msg = "MESSAGE\nsubscription:" + senderSubId + "\nMessage-id:" + messageId + "\ndestination:" + destination + "\n\n" + body + "\n\u0000";
            userAndSub.getFirst().getConnectionHandler().send(msg);
            messageId.incrementAndGet();
        }
    }

    public boolean send(int connectionId, Object msg) {
        ConnectionHandler<String> handler = connectionHandlersMap.get(connectionId);
        if (handler != null) {
            synchronized (handler) {
                handler.send(msg.toString());
            }
        }
        return handler != null;
    }

    public void unSubscribe(String subId, int connectionId, String receiptUn) {
        User<String> user = UsersByConnectionId.get(connectionId);
        if (user != null) {
            String topic = user.findTopicBySubId(subId);
            if (topic != null && topicsAndUsersAndSub.get(topic) != null) {
                String msg = "RECEIPT\nreceipt-id:" + receiptUn + "\n\n\u0000";
                connectionHandlersMap.get(connectionId).send(msg);

                for (Pair<User<String>, String> userAndSub : topicsAndUsersAndSub.get(topic)) {
                    if (userAndSub.getFirst().equals(user) && userAndSub.getSecond().equals(subId)) {
                        System.out.println(userAndSub.getFirst().getName() + " " + userAndSub.getSecond());
                        boolean isT = topicsAndUsersAndSub.get(topic).remove(userAndSub);
                        System.out.println(isT);
                        break;
                    }
                }
                synchronized (locksmap.get(user.getName())) {
                    user.removeSub(topic);
                }
            } else {
                System.out.println("ERROR: USER WANTED TO UNSUBSCRIBE TO NULL TOPIC OR TOPIC DOESNT EXIST AT MAP");
            }
        } else {
            System.out.println("ERROR: USER WHO WANTED TO SUBSCRIBE IS NULL");
        }
    }

}

package bgu.spl.net.srv;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.api.MessagingProtocol;
import bgu.spl.net.api.StompMessagingProtocol;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;
import java.net.Socket;

public class BlockingConnectionHandler<T> implements Runnable, ConnectionHandler<T> {

    private final StompMessagingProtocol protocol;
    private final MessageEncoderDecoder<T> encdec;
    private final Socket sock;
    private BufferedInputStream in;
    private BufferedOutputStream out;
    private volatile boolean connected = true;
    private ConnectionsImpl<T> connections;

    @SuppressWarnings("unchecked")
    public BlockingConnectionHandler(Socket sock, MessageEncoderDecoder<T> reader, StompMessagingProtocol protocol) {
        this.sock = sock;
        this.encdec = reader;
        this.protocol = protocol;
        connections = (ConnectionsImpl<T>)ConnectionsImpl.getInstance();
    }

    @Override
    public void run() {
        try (Socket sock = this.sock) { //just for automatic closing
            int read;
            try {
                in = new BufferedInputStream(sock.getInputStream());
                out = new BufferedOutputStream(sock.getOutputStream());
            }catch(Exception e){
                System.out.println("first try");
            }
            while (!protocol.shouldTerminate() && connected && (read = in.read()) >= 0) {
                T nextMessage = encdec.decodeNextByte((byte) read);
                if (nextMessage != null) {
                    protocol.process(nextMessage);
                }

            }
        } catch (IOException ex) {
            System.out.println(" could not read from socket");
        }
    }

    @Override
    public void close() {
        connected = false;
        //sock.close();
    }

    @Override
    public void send(T msg) {
        byte[] message = encdec.encode(msg);
        try{
            out.write(message);
            out.flush();
            System.out.println("sending from blocking"+msg);
        } catch (IOException e) {
            e.printStackTrace();
        }

    }
}

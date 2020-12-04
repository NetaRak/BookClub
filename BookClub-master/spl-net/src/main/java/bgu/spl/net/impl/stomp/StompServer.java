package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.Server;

public class StompServer {

    public static void main(String[] args) {
    int port= Integer.parseInt(args[0]);
// you can use any server...
        if (args[1].equals("tpc")) {

            Server.threadPerClient(
                    port, //port
                    () -> new MessageProtocolImp(),
                    MessageEncoderDecoderImpl::new //message encoder decoder factory
            ).serve();
        } else if (args[1].equals("reactor")) {
            Server.reactor(
                    Runtime.getRuntime().availableProcessors(),
                    port, //port
                    () -> new MessageProtocolImp(), //protocol factory
                    MessageEncoderDecoderImpl::new //message encoder decoder factory
            ).serve();

        }
    }
}





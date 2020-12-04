package bgu.spl.net.impl.stomp;

public class Pair<T,N> {
    T first;
    N second;

    public Pair(T first, N second){
        this.first=first;
        this.second=second;
    }
    public void setFirst(T f){first=f;}
    public T getFirst(){return first;}

    public void setSecond(N n){second=n;}
    public N getSecond(){return second;}

}

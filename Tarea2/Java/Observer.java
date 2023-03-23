package Tarea2.Java;

import java.util.LinkedList;
import java.util.*;

public class Observer{
    private static Observer Instance = new Observer();
    private static Queue<Integer> Queue = new LinkedList<>();
    static boolean[] forks = {true,true,true,true,true};

    private Observer(){}

    public static Observer getObserver(){
        return Instance;
    }

    public static synchronized int requestToEat(int PhilosopherId){
        if(forks[PhilosopherId] & forks[(PhilosopherId+1)%5]){
            forks[PhilosopherId] = false;
            forks[(PhilosopherId+1)%5] = false;
            return 0;
        }else{
            if(!Queue.contains(PhilosopherId)){
                Queue.add(PhilosopherId);
            }
            return 1;
        }
    }
}

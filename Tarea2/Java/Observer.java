package Tarea2.Java;

import java.util.*;

public class Observer{
    private static Observer Instance = new Observer();
    private static ArrayList<Integer> queue = new ArrayList<>();
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
            if(!queue.contains(PhilosopherId)){
                queue.add(PhilosopherId);
            }
            return 1;
        }
    }
    public static synchronized void returnForks(int PhilosopherId){
        forks[PhilosopherId] = true;
        forks[(PhilosopherId+1)%5] = true;

        for(int i = 0; i<5; i++){
            if(forks[queue.get(i)] & forks[(queue.get(i)+1)%5]){
                // notify phil
                javaPhil.philosophers[i].notify();
                break;
            }
        }
    }
}

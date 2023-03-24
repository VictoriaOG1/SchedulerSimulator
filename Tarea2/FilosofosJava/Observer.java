package FilosofosJava;

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
            System.out.println("Phil: " + PhilosopherId + " started eating now");
            forks[PhilosopherId] = false;
            forks[(PhilosopherId+1)%5] = false;
            return 0;
        }else{
            if(!queue.contains(PhilosopherId)){
                queue.add(PhilosopherId);
            }
            System.out.println("Phil: " + PhilosopherId + " has to wait to eat and it's on queue");
            do{
                try {Observer.class.wait();}
                catch (InterruptedException e) {e.printStackTrace();}
            }while(queue.get(0) != PhilosopherId);
            return 1;
        }
    }
    public static synchronized void returnForks(int PhilosopherId){
        forks[PhilosopherId] = true;
        forks[(PhilosopherId+1)%5] = true;
        System.out.println("Phil: " + PhilosopherId + " returned their forks");
        for(int i = 0; i<queue.size(); i++){
            if(forks[queue.get(i)] & forks[(queue.get(i)+1)%5]){
                System.out.println("--Phil: " + queue.get(i) + " has been notified");
                queue.add(0,queue.remove(i));
                Observer.class.notifyAll();
                queue.remove(0);
                break;
            }
        }
    }
}

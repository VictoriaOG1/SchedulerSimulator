package FilosofosJava;

import java.util.*;

public class Monitor{
    private static int dequeue;
    private static ArrayList<Integer> queue = new ArrayList<>();
    static boolean[] forks = {true,true,true,true,true};

    private Monitor(){}

    public static synchronized int requestToEat(int PhilosopherId){
        if(forks[PhilosopherId] & forks[(PhilosopherId+1)%5]){
            System.out.println("Filosofo " + PhilosopherId + " empieza a comer");
            forks[PhilosopherId] = false;
            forks[(PhilosopherId+1)%5] = false;
            return 0;
        }else{
            if(!queue.contains(PhilosopherId)){
                queue.add(PhilosopherId);
            }
            System.out.println("Filosofo " + PhilosopherId + " tiene que esperar y esta en la cola");
            do{
                try {Monitor.class.wait();}
                catch (InterruptedException e) {e.printStackTrace();}
            }while(dequeue != PhilosopherId);
            return 1;
        }
    }
    public static synchronized void returnForks(int PhilosopherId){
        forks[PhilosopherId] = true;
        forks[(PhilosopherId+1)%5] = true;
        System.out.println("Filosofo " + PhilosopherId + " libera los palillos");
        for(int i = 0; i<queue.size(); i++){
            if(forks[queue.get(i)] & forks[(queue.get(i)+1)%5]){
                System.out.println("--Filosofo " + queue.get(i) + " ha sido notificado");
                dequeue = queue.remove(i);
                Monitor.class.notifyAll();
                break;
            }
        }
    }
    public static void printQueue(){
        System.out.println(queue);
    }
}

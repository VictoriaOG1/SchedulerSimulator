package Tarea2.Java;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class javaPhil {
    public static void main(String[] args) throws Exception {
        Philosopher[] philosophers = new Philosopher[5];
        ExecutorService exe = Executors.newFixedThreadPool(5);

        for(int i = 0;i<5;i++){
            philosophers[i] = new Philosopher(i);
            exe.submit(philosophers[i].task);
        }

        exe.shutdown();
    }
}


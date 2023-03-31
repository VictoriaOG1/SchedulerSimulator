package FilosofosJava;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class javaPhil {
     
   public static Philosopher[] philosophers = new Philosopher[5];

   public static void main(String[] args) throws Exception {   
               
      ExecutorService exe = Executors.newFixedThreadPool(5);
      long startTime = System.nanoTime();

      for(int i = 0;i<5;i++){
         philosophers[i] = new Philosopher(i);
         exe.submit(philosophers[i].task);
      }
     
      exe.shutdown();
             
   }
      
}


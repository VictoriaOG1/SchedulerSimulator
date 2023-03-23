import java.util.Arrays;
import java.util.List;
import java.util.Scanner;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class javaPhil {

    public class Philosopher{
        private int _id;
        private int _waitTime = 0;
        private boolean _eating = false;
        private int _mealsLeft = 3;

        public boolean FinishedEating(){
            if(_mealsLeft == 0){return true;}
            else{return false;}
        }
        public boolean isEating(){
            return _eating;
        }
        public int getId(){
            return _id;
        }

        Philosopher(int id){_id = id;}

        Callable<String> task = () -> {
            if(!_eating){
                if(Observer.requestToEat(getId()) == 0){
                    _eating = true;
                    return "Is eating now";
                }else{
                    return "It's on queue";
                }
            }else{
                return "It's Eating";
            }
        };
    }

    public static class Observer{
        static int time = 0;
        private static Integer[] _init = {-1,-1,-1,-1,-1};
        static List<Integer> Queue = Arrays.asList(_init);
        static boolean[] forks = {true,true,true,true,true};

        public static synchronized int requestToEat(int PhilosopherId){
            if(forks[PhilosopherId] & forks[(PhilosopherId+1)%5]){
                forks[PhilosopherId] = false;
                forks[(PhilosopherId+1)%5] = false;
                return 0;
            }else{
                if(true){

                }
                return 1;
            }
        }
    }
     
    public static void main(String[] args) throws Exception {

        ExecutorService exe = Executors.newFixedThreadPool(5);

        Scanner sc= new Scanner(System.in); //System.in is a standard input stream.
        System.out.print("Enter a string: ");
        String str= sc.nextLine(); //reads string.
        str+= sc.nextLine();
        str+= sc.nextLine();
        System.out.println(str);
    }
}


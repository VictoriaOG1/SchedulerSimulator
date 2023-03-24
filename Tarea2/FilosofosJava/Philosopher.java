package FilosofosJava;

public class Philosopher{
    private int _id;
    // private Observer observer = Observer.getObserver();
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

    public Philosopher(int id){_id = id;}

    private synchronized void run(){
        while(!FinishedEating()){
            if(!_eating){
                if(Observer.requestToEat(getId()) == 0){
                    _eating = true;
                    _mealsLeft--;
                    try {Thread.sleep(500);}
                    catch (InterruptedException e) {e.printStackTrace();}
                    Observer.returnForks(_id);
                    System.out.println("Phil: " + _id + " finished eating, now thinking");
                    try {Thread.sleep(500);}
                    catch (InterruptedException e) {e.printStackTrace();}
                    _eating = false;
                    if(FinishedEating()){System.out.println("Phil: " + _id + " is done Eating for the day!");Observer.printQueue();}
                    else{System.out.println("Phil: " + _id + " is hungry now");}
                }
            }else{
                System.out.println("Phil: " + _id + " it's still Eating");
            }
        }
    }

    Runnable task = () -> {
        run();
    };
}

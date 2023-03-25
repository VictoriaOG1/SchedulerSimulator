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
                if(Monitor.requestToEat(getId()) == 0){
                    _eating = true;
                    _mealsLeft--;
                    try {Thread.sleep(500);}
                    catch (InterruptedException e) {e.printStackTrace();}
                    Monitor.returnForks(_id);
                    System.out.println("Filosofo " + _id + " termina de comer, esta pensando");
                    try {Thread.sleep(500);}
                    catch (InterruptedException e) {e.printStackTrace();}
                    _eating = false;
                    if(FinishedEating()){System.out.println("Filósofo " + _id + " Ha terminado de comer por hoy!");Monitor.printQueue();}
                    else{System.out.println("Filosofo " + _id + " esta hambriento");}
                }
            }else{
                System.out.println("Filosofo " + _id + " todavia esta comiendo");
            }
        }
    }

    Runnable task = () -> {
        run();
    };
}

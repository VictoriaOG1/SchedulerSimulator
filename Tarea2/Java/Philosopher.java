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

    Runnable task = () -> {
        while(!FinishedEating()){
            if(!_eating){
                if(Observer.requestToEat(getId()) == 0){
                    _eating = true;
                    _mealsLeft--;
                    System.out.println("Phil: " + _id + " started eating now");
                    try {wait(500);}
                    catch (InterruptedException e) {e.printStackTrace();}
                    Observer.returnForks(_id);
                    System.out.println("Phil: " + _id + " finished eating, now thinking");
                    try {wait(500);}
                    catch (InterruptedException e) {e.printStackTrace();}
                    _eating = false;
                    System.out.println("Phil: " + _id + " is hungry now");
                }else{
                    System.out.println("Phil: " + _id + " has to wait to eat and it's on queue");
                    try {
                        wait();
                    } catch (InterruptedException e) {
                        // TODO Auto-generated catch block
                        e.printStackTrace();
                    }
                }
            }else{
                System.out.println("Phil: " + _id + " it's still Eating");
            }
        }
    };
}

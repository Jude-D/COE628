/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package lab9;

/**
 *
 * @author jd936
 */
class DinnerTable {

    public Philosopher[] philosophers;
    public final semaphore mutex;

    public DinnerTable(int numPhilosophers) {
        mutex = new semaphore(0);
        philosophers = new Philosopher[numPhilosophers];
        for (int i = 0; i < numPhilosophers; i++) {
            philosophers[i] = new Philosopher(i, this);
        }
    }

    public void start() {
        for (Philosopher philosopher : philosophers) {
            philosopher.start();
        }
    }
}

class Philosopher extends Thread {

    private enum phil_state {
        THINKING, HUNGRY, EATING
    }
    public int id;
    private phil_state state;
    private final semaphore s;
    private DinnerTable dinnerTable;

    Philosopher(int id, DinnerTable dinnerTable) {
        this.id = id;
        s = new semaphore(0);
        state = phil_state.THINKING;
        this.dinnerTable = dinnerTable;
    }

    private int right_index(int i, int n) {
        return (((i) + 1) % n);
    }

    private int left_index(int i, int n) {
        return (((i) == n) ? 0 : (i) + 1);
    }

    private void think() {
        try {
            state = phil_state.THINKING;
            Thread.sleep(1000);
        } catch (InterruptedException e) {
            e.printStackTrace();
            System.exit(1);
        }
        state = phil_state.HUNGRY;
    }

    private void eat() {
        try {
            state = phil_state.EATING;
            Thread.sleep(1000);
        } catch (InterruptedException e) {
            e.printStackTrace();
            System.exit(1);
        }
        state = phil_state.THINKING;
    }

    public void get_forks() {
        int left = left_index(id, dinnerTable.philosophers.length), right = right_index(id, dinnerTable.philosophers.length);
        this.state = phil_state.HUNGRY;
        while (state == phil_state.HUNGRY) {
            dinnerTable.mutex.down();
            if (this.state == phil_state.HUNGRY && dinnerTable.philosophers[left].state != phil_state.EATING && dinnerTable.philosophers[right].state != phil_state.EATING) {
                this.state = phil_state.EATING;
                s.up();
            }
            dinnerTable.mutex.up();
            s.down();
        }
    }

    public void put_forks() {
        dinnerTable.mutex.up();
        int left = left_index(id, dinnerTable.philosophers.length), right = right_index(id, dinnerTable.philosophers.length);
        state = phil_state.THINKING;
        if (dinnerTable.philosophers[left].state == phil_state.HUNGRY) {
            dinnerTable.philosophers[left].s.up();
        } else if (dinnerTable.philosophers[right].state == phil_state.HUNGRY) {
            dinnerTable.philosophers[right].s.up();
        }
        dinnerTable.mutex.down();
    }

    public void status() {
        System.out.print("Philosopher " + id + " is currently " + state + "\n");
    }

    public void run() {
        while (true) {
            status();
            think();
            get_forks();
            status();
            eat();
            put_forks();
        }
    }

}

public class Lab9 {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        DinnerTable dinnerTable = new DinnerTable(4);
        dinnerTable.start();
    }

}

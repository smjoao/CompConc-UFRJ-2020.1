import java.util.LinkedList;
import java.util.Queue;

class MonitorQuickSort {
    int list_size;
    int max_cap;
    int ordenados;
    public boolean logs;
    Queue<Args> buffer;

    public MonitorQuickSort(Args init_args, int max_cap, int list_size, boolean logs) {
        this.buffer = new LinkedList<>();
        this.buffer.add(init_args);
        this.max_cap = max_cap;
        this.list_size = list_size;
        this.ordenados = 0;
        this.logs = logs;
    }

    public synchronized void InserirBuffer(int id, Args arg) {
        try {
            while(buffer.size() == max_cap) {
                if(logs) System.out.println("Thread[" + id + "] bloqueada ao inserir!");
                wait();
                if(logs) System.out.println("Thread[" + id + "] desbloqueada para inserir!");
            }
        } catch (InterruptedException e) { }
        this.buffer.add(arg);
        if(logs) System.out.println("Thread[" + id + "] inseriu [" + arg.inicio + ", " + arg.fim + "] no buffer.");
        notifyAll();
    }

    public synchronized Args RemoverBuffer(int id) {
        try {
            while(buffer.size() == 0 && ordenados != list_size) {
                if(logs) System.out.println("Thread[" + id + "] bloqueada ao remover!");
                wait();
                if(logs) System.out.println("Thread[" + id + "] desbloqueada para remover!");
            }
        } catch (InterruptedException e) { }

        if(ordenados == list_size) return null;

        Args arg = this.buffer.remove();
        notifyAll();
        if(logs) System.out.println("Thread[" + id + "] removeu [" + arg.inicio + ", " + arg.fim + "] do buffer.");
        return arg;
    }

    public synchronized void AddOrdenados() {
        this.ordenados++;
        if(this.ordenados == list_size) notifyAll();
    }

    public synchronized int GetOrdenados() {
        return this.ordenados;
    }
}
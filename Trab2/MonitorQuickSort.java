import java.util.LinkedList;
import java.util.Queue;

class MonitorQuickSort {
    int max_cap;
    int ordenados;
    Queue<Args> buffer;

    public MonitorQuickSort(Args init_args, int max_cap) {
        this.buffer = new LinkedList<>();
        this.buffer.add(init_args);
        this.max_cap = max_cap;
        this.ordenados = 0;
    }

    public synchronized void InserirBuffer(Args arg) {
        try {
            while(buffer.size() == max_cap) {
                wait();
            }
        } catch (InterruptedException e) { }
        this.buffer.add(arg);
        System.out.println("[" + arg.inicio + ", " + arg.fim + "] inserido no buffer.");
        notifyAll();
    }

    public synchronized Args RemoverBuffer() {
        try {
            while(buffer.size() == 0) {
                wait();
            }
        } catch (InterruptedException e) { }

        Args arg = this.buffer.remove();
        notifyAll();
        System.out.println("[" + arg.inicio + ", " + arg.fim + "] removido do buffer.");
        return arg;
    }

    public synchronized void AddOrdenados() {
        this.ordenados++;
        notifyAll();
    }

    public synchronized int GetOrdenados() {
        return this.ordenados;
    }
}
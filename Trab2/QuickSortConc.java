import java.util.ArrayList;
import java.util.Collections;

class Args {
    public int inicio, fim;

    public Args(int inicio, int fim) {
        this.inicio = inicio;
        this.fim = fim;
    }

}

class QuickSortThread extends Thread {
    ArrayList<Integer> list;
    MonitorQuickSort monitor;

    public QuickSortThread(ArrayList<Integer> list, MonitorQuickSort monitor) {
        this.list = list;
        this.monitor = monitor;
    }

    public void run() {
        while(monitor.GetOrdenados() < list.size()) {
            Args arg = monitor.RemoverBuffer();

            if(arg.inicio < arg.fim) {
                int pivot = list.get(arg.fim);
                int i = arg.inicio;

                for(int j = arg.inicio; j < arg.fim; j++) {
                    if(list.get(j) < pivot) {
                        Collections.swap(list, i, j);
                        i++;
                    }
                }
                Collections.swap(list, i, arg.fim);

                monitor.AddOrdenados();

                Args arg1 = new Args(arg.inicio, i - 1);
                Args arg2 = new Args(i + 1, arg.fim);
                if(arg1.inicio <= arg1.fim) monitor.InserirBuffer(arg1);
                if(arg2.inicio <= arg2.fim) monitor.InserirBuffer(arg2);
                System.out.println("Lista: " + list);
            } else if (arg.inicio == arg.fim) {
                monitor.AddOrdenados();
            }
        }
        System.out.println("Thread terminada!");
    }
}

class QuickSortConc {
    public static void main(String[] args) {
        
        if(args.length < 2) {
            System.out.println("Uso correto do programa:");
            System.out.println("java Main <Capacidade do buffer> <Nº de threads>");
            System.exit(1);
        }

        int max_cap = Integer.parseInt(args[0]);
        int num_threads = Integer.parseInt(args[1]);

        ArrayList<Integer> list = new ArrayList<>();
        for(int i = 10; i > 0; i--) {
            list.add(i);
        }
        Args init_args = new Args(0, list.size() - 1);

        MonitorQuickSort monitor = new MonitorQuickSort(init_args, max_cap);
        QuickSortThread[] threads = new QuickSortThread[num_threads];

        for(int i = 0; i < num_threads; i++) {
            threads[i] = new QuickSortThread(list, monitor);
            threads[i].start();
        }

        for(int i = 0; i < num_threads; i++) {
            try { threads[i].join(); }
            catch (InterruptedException e) { }
        }

        System.out.println(list);
    }
}
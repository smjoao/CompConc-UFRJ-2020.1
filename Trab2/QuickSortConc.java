import java.util.ArrayList;
import java.util.Collections;
import java.io.BufferedReader;
import java.io.FileReader;

class Args {
    public int inicio, fim;

    public Args(int inicio, int fim) {
        this.inicio = inicio;
        this.fim = fim;
    }

}

class QuickSortThread extends Thread {
    int id;
    ArrayList<Integer> list;
    MonitorQuickSort monitor;

    public QuickSortThread(int id, ArrayList<Integer> list, MonitorQuickSort monitor) {
        this.id = id;
        this.list = list;
        this.monitor = monitor;
    }

    public void run() {
        Args arg = monitor.RemoverBuffer(this.id);
        while(monitor.GetOrdenados() < list.size()) {

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
            
            if(arg1.inicio <= arg1.fim) monitor.InserirBuffer(this.id, arg1);
            if(arg2.inicio <= arg2.fim) monitor.InserirBuffer(this.id, arg2);
            
            if(monitor.GetOrdenados() < list.size()) arg = monitor.RemoverBuffer(this.id);
        }
        if(monitor.logs) System.out.println("Thread[" + this.id + "] terminada!");
    }
}

class QuickSortConc {
    public static void main(String[] args) {
        boolean logs = false;
        
        if(args.length < 2) {
            System.out.println("Uso correto do programa:");
            System.out.println("java Main <Nº de threads> <s ou n: Mostrar logs?(sim ou não)>");
            System.exit(1);
        }

        int num_threads = Integer.parseInt(args[0]);

        if(args[1].equals("s")) logs = true;
        else if(!args[1].equals("n")) {
            System.out.println("java Main <Nº de threads> <s ou n: Mostrar logs?(sim ou não)>");
            System.exit(1);
        }

        ArrayList<Integer> list = new ArrayList<>();
        try {
            BufferedReader br = new BufferedReader(new FileReader("in.txt"));
            String line = br.readLine();
            while(line != null) {
                list.add(Integer.parseInt(line));
                line = br.readLine();
            }
        } catch(Exception e) { System.out.println(e.getMessage()); System.exit(-1); }

        System.out.println("Lista original: " + list);

        Args init_args = new Args(0, list.size() - 1);

        MonitorQuickSort monitor = new MonitorQuickSort(init_args, Integer.MAX_VALUE, list.size(), logs);
        QuickSortThread[] threads = new QuickSortThread[num_threads];

        long tempo_ini = System.currentTimeMillis();

        for(int i = 0; i < num_threads; i++) {
            threads[i] = new QuickSortThread(i, list, monitor);
            threads[i].start();
        }

        for(int i = 0; i < num_threads; i++) {
            try { threads[i].join(); }
            catch (InterruptedException e) { }
        }

        long tempo_fin = System.currentTimeMillis();

        System.out.println("Lista ordenada: " + list);

//        for(int i = 0; i < list.size(); i++) {
//            System.out.println(list.get(i));
//        }

        System.out.println("Tempo de execução: " + (tempo_fin - tempo_ini) + "ms");
    }
}
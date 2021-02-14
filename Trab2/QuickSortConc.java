import java.util.ArrayList;
import java.util.Collections;
import java.io.BufferedReader;
import java.io.FileReader;

// Objeto que será inserido no buffer
class Args {
    public int inicio, fim;

    public Args(int inicio, int fim) {
        this.inicio = inicio;
        this.fim = fim;
    }
}

// Classe das Threads
class QuickSortThread extends Thread {
    int id; // Identificador das threads
    ArrayList<Integer> list; // Vetor que será ordenado
    MonitorQuickSort monitor; // Monitor das threads

    // Construtor
    public QuickSortThread(int id, ArrayList<Integer> list, MonitorQuickSort monitor) {
        this.id = id;
        this.list = list;
        this.monitor = monitor;
    }

    // Tarefa executada pelas threads
    public void run() {
        // Tenta tirar um objeto do buffer
        Args arg = monitor.RemoverBuffer(this.id);

        // Enquanto o vetor ainda não estiver totalmente ordenado
        while(monitor.GetOrdenados() < list.size()) {

            // Aplica o Quick Sort na seção do vetor representado pelo arg

            int pivot = list.get(arg.fim); // Escolhe o pivô como o último item da seção do vetor
            int i = arg.inicio; // Inicia o índice dos elementos menores que o pivô

            // Passa por todos os itens da seção
            for(int j = arg.inicio; j < arg.fim; j++) {

                // Se o item for menor que o pivô troca com o item no índice i,
                // que representa o índice pros elementos menores que o pivô, e incrementa i
                if(list.get(j) < pivot) {
                    Collections.swap(list, i, j);
                    i++;
                }
            }

            // No final troca o pivô com o item no índice i, 
            // já que todos os elementos antes desse índice são menores que o pivô
            Collections.swap(list, i, arg.fim);

            // Incrementa ordenados
            monitor.AddOrdenados();

            // Inicia os objetos que serão inseridos no buffer
            Args arg1 = new Args(arg.inicio, i - 1);
            Args arg2 = new Args(i + 1, arg.fim);
            
            // Insere os objetos no buffer se eles forem válidos (inicio <= fim)
            if(arg1.inicio <= arg1.fim) monitor.InserirBuffer(this.id, arg1);
            if(arg2.inicio <= arg2.fim) monitor.InserirBuffer(this.id, arg2);
            
            // Se a ordenação ainda não tiver terminado, remove mais um objeto do buffer
            if(monitor.GetOrdenados() < list.size()) arg = monitor.RemoverBuffer(this.id);
        }
        if(monitor.logs) System.out.println("Thread[" + this.id + "] terminada!");
    }
}

class QuickSortConc {
    public static void main(String[] args) {
        boolean logs = false; // Define se exibe logs das threads ou não
        
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

        // Inicia o vetor e preenche ele com o arquivo in.txt
        ArrayList<Integer> list = new ArrayList<>();
        try {
            BufferedReader br = new BufferedReader(new FileReader("in.txt"));
            String line = br.readLine();
            while(line != null) {
                list.add(Integer.parseInt(line));
                line = br.readLine();
            }
            br.close();
        } catch(Exception e) { System.out.println(e.getMessage()); System.exit(-1); }

        // Imprime o vetor original
        System.out.println("Vetor original: " + list);

        // Inicia o buffer com o primeiro elemento representando o vetor inteiro
        Args init_args = new Args(0, list.size() - 1);

        // Inicia o monitor e o vetor de threads, define capacidade 
        // máx. do buffer como maior valor possível para inteiro
        MonitorQuickSort monitor = new MonitorQuickSort(init_args, Integer.MAX_VALUE, list.size(), logs);
        QuickSortThread[] threads = new QuickSortThread[num_threads];

        // Começa a contar o tempo de execução
        long tempo_ini = System.currentTimeMillis();

        // Cria e inicia as threads
        for(int i = 0; i < num_threads; i++) {
            threads[i] = new QuickSortThread(i, list, monitor);
            threads[i].start();
        }

        // Espera todas as threads terminarem
        for(int i = 0; i < num_threads; i++) {
            try { threads[i].join(); }
            catch (InterruptedException e) { }
        }

        // Termina a contagem de tempo
        long tempo_fin = System.currentTimeMillis();

        // Imprime o vetor ordenado
        System.out.println("Vetor ordenado: " + list);
        
        // Código para a verificação da corretude
//        for(int i = 0; i < list.size(); i++) {
//            System.out.println(list.get(i));
//        }

        // Imprime o tempo de execução
        System.out.println("Tempo de execução: " + (tempo_fin - tempo_ini) + "ms");
    }
}
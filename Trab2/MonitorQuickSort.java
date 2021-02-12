import java.util.LinkedList;
import java.util.Queue;

class MonitorQuickSort {
    int list_size; // Tamanho do vetor que será ordenado
    int max_cap; // Capacidade máxima do buffer
    int ordenados; // Quantidade de posições ordenadas
    public boolean logs; // Define se imprime os logs das threads ou não
    Queue<Args> buffer; // Buffer do monitor

    // Construtor
    public MonitorQuickSort(Args init_args, int max_cap, int list_size, boolean logs) {
        this.buffer = new LinkedList<>();
        this.buffer.add(init_args);
        this.max_cap = max_cap;
        this.list_size = list_size;
        this.ordenados = 0;
        this.logs = logs;
    }

    // Método para inserir itens no buffer
    public synchronized void InserirBuffer(int id, Args arg) {
        try {
            // Se o buffer estiver cheio, bloqueia a thread
            while(buffer.size() == max_cap) {
                if(logs) System.out.println("Thread[" + id + "] bloqueada ao inserir!");
                wait();
                if(logs) System.out.println("Thread[" + id + "] desbloqueada para inserir!");
            }
        } catch (InterruptedException e) { }

        // Adiciona o objeto ao buffer
        this.buffer.add(arg);

        if(logs) System.out.println("Thread[" + id + "] inseriu [" + arg.inicio + ", " + arg.fim + "] no buffer.");
        
        // Desbloqueia uma thrad para o caso do buffer estar vazio anteriormente 
        notify();
    }

    // Método para remover itens do buffer
    public synchronized Args RemoverBuffer(int id) {
        try {
            // Se o buffer estiver vazio e a ordenação ainda não tiver terminado, bloqueia a thread
            while(buffer.size() == 0 && ordenados != list_size) {
                if(logs) System.out.println("Thread[" + id + "] bloqueada ao remover!");
                wait();
                if(logs) System.out.println("Thread[" + id + "] desbloqueada para remover!");
            }
        } catch (InterruptedException e) { }

        // Se a ordenação já tiver terminado, não retorna nada
        // Essa verificação serve para o caso de algumas threads
        // ainda estiverem bloqueadas após o término da ordenação
        if(ordenados == list_size) return null;

        // Remove um item do buffer
        Args arg = this.buffer.remove();

        // Desbloqueia uma outra thread para o caso do buffer estar cheio anteriormente
        notify();
        
        if(logs) System.out.println("Thread[" + id + "] removeu [" + arg.inicio + ", " + arg.fim + "] do buffer.");
        
        // Retorna o objeto removido do buffer
        return arg;
    }

    public synchronized void AddOrdenados() {
        // Incrementa a variável ordenadas
        this.ordenados++;

        // Se todas as posições estiverem ordenadas, notifica todas
        // as threads para que elas possam finalizar a execução
        if(this.ordenados == list_size) notifyAll();
    }

    public synchronized int GetOrdenados() {
        // Retorn ordenados
        return this.ordenados;
    }
}
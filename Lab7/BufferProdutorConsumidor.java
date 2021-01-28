import java.util.ArrayList;
import java.lang.StringBuilder;

class BufferProdutorConsumidor {
    int capacidade;
    ArrayList<Integer> buff;

    public BufferProdutorConsumidor(int capacidade) {
        this.capacidade = capacidade;
        this.buff = new ArrayList<>(capacidade);
    }

    public synchronized void print() {
        StringBuilder str = new StringBuilder("Buffer = [");
        for(int i = 0; i < capacidade; i++) {
            if(i != 0) str.append(", ");

            // Se existir item no índice i, retorna o item
            // Senão, retorna null
            Integer item = this.buff.size() > i ? this.buff.get(i) : null;
            
            if(item == null) str.append("vazio");
            else str.append(item);
        }
        System.out.println(str + "]");
    }

    public synchronized void Insert(int id, int item) {
        try {
            // Bloqueia a thread caso o buffer esteja cheio
            while(this.buff.size() == this.capacidade) {
                System.out.println("Produtor " + id + " bloqueado!(Buffer cheio)");
                wait();
                System.out.println("Produtor " + id + " desbloqueado!");
            }
        } catch (InterruptedException e) { }

        int index = this.buff.size();
        this.buff.add(index, item);
        System.out.println("Produtor " + id + " produziu item " + item + "!");
        this.print();
        notifyAll();
    }

    public synchronized int Remove(int id) {
        try {
            // Bloqueia a thread caso o buffer esteja vazio
            while(this.buff.size() == 0) {
                System.out.println("Consumidor " + id + " bloqueado!(Buffer vazio)");
                wait();
                System.out.println("Consumidor " + id + " desbloqueado!");
            }
        } catch (InterruptedException e) { }

        int index = this.buff.size() - 1;
        int item = this.buff.remove(index);
        System.out.println("Consumidor " + id + " consumiu item " + item + "!");
        this.print();
        notifyAll();
    }
}
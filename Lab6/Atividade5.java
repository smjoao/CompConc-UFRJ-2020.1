class V {
    private int[] vetor;

    public V(int size) {
        this.vetor = new int[size];
    }

    public void print() {
        for(int i = 0; i < this.vetor.length; i++) {
            if(i == 0) System.out.print("[" + this.vetor[i]);
            else System.out.print(", " + this.vetor[i]);
        }
        System.out.println("]");
    }

    public int size() {
        return this.vetor.length;
    }

    public void set(int index, int value) {
        this.vetor[index] = value;
    }

    public int get(int index) {
        return this.vetor[index];
    }
}

class T extends Thread {
    private int id, num_threads;
    V a, b, c;

    public T(int id, int num_threads, V a, V b, V c) {
        this.id = id;
        this.num_threads = num_threads;
        this.a = a;
        this.b = b;
        this.c = c;
    }

    public void run() {
        System.out.println("Thread " + this.id + ": Iniciada");
        for(int i = id; i < a.size(); i += num_threads) {
            c.set(i, a.get(i) + b.get(i));
        }
        System.out.println("Thread " + this.id + ": Terminada");
    }
}

class Atividade5 {
    

    public static void main (String[] args) {
        if(args.length < 2) {
            System.out.println("Uso correto do programa:");
            System.out.println("java Atividade5 <Tamanho dos vetores> <Nº de threads>");
            System.exit(1);
        }

        int size = Integer.parseInt(args[0]);
        int num_threads = Integer.parseInt(args[1]);

        Thread[] threads = new Thread[num_threads];

        V a = new V(size);
        V b = new V(size);
        V c = new V(size);

        for(int i = 0; i < size; i++) {
            a.set(i, 1);
            b.set(i, 2);
        }

        System.out.print("Valor de A:");
        a.print();
        System.out.print("Valor de B:");
        b.print();

        // Cria as threads
        for (int i=0; i<num_threads; i++) {
            threads[i] = new T(i, num_threads, a, b, c);
        }

        // Inicia cada thread
        for (int i=0; i<num_threads; i++) {
            threads[i].start();
        }

        // Espera todas as threads terminarem
        for (int i=0; i<num_threads; i++) {
            try { threads[i].join(); } 
            catch (InterruptedException e) { System.exit(-1); }
        }

        System.out.print("Valor de C:");
        c.print();
    }
}
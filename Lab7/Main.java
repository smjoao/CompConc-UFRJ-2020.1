// Thread produtora
class P extends Thread {
    int id;
    BufferProdutorConsumidor buff;

    public P(int id, BufferProdutorConsumidor buff) {
        this.id = id;
        this.buff = buff;
    }

    public void run() {
        int item = 0;
        try {
            while(true) {
                // Produz um multiplo do seu id a cada loop
                item += this.id;
                buff.Insert(id, item);
                sleep(500);
            }
        } catch(InterruptedException e) { return; }
    }
}

// Thread consumidora
class C extends Thread {
    int id;
    BufferProdutorConsumidor buff;

    public C(int id, BufferProdutorConsumidor buff) {
        this.id = id;
        this.buff = buff;
    }

    public void run() {
        try {
            while(true) {
                int item = buff.Remove(id);
                sleep(500);
            }
        } catch(InterruptedException e) { return; }
    }
}

class Main {
    public static void main (String[] args) { 

        if(args.length < 2) {
            System.out.println("Uso correto do programa:");
            System.out.println("java Main <Capacidade do buffer> <Nº de threads produtoras> <Nº de threads consumidoras>");
            System.exit(1);
        }

        int capacidade = Integer.parseInt(args[0]);
        int num_prod = Integer.parseInt(args[1]);
        int num_cons = Integer.parseInt(args[2]);

        BufferProdutorConsumidor buff = new BufferProdutorConsumidor(capacidade);
        P[] prods = new P[num_prod];
        C[] cons = new C[num_cons];

        for(int i = 0; i < num_prod; i++) {
            prods[i] = new P(i + 1, buff);
            prods[i].start();
        }

        for(int i = 0; i < num_cons; i++) {
            cons[i] = new C(i + 1, buff);
            cons[i].start();
        }
    }
}

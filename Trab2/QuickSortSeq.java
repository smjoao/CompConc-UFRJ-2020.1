import java.io.BufferedReader;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.Collections;

class QuickSortSeq {

    public static void QuickSort(ArrayList<Integer> list, int inicio, int fim) {
        // Aplica o Quick Sort na seção do vetor entre inicio e fim

        int pivot = list.get(fim); // Escolhe o pivô como o último item da seção do vetor
        int i = inicio; // Inicia o índice dos elementos menores que o pivô

        // Passa por todos os itens da seção
        for(int j = inicio; j < fim; j++) {

            // Se o item for menor que o pivô troca com o item no índice i,
            // que representa o índice pros elementos menores que o pivô, e incrementa i
            if(list.get(j) < pivot) {
                Collections.swap(list, i, j);
                i++;
            }
        }

        // No final troca o pivô com o item no índice i, 
        // já que todos os elementos antes desse índice são menores que o pivô
        Collections.swap(list, i, fim);

        if(inicio <= i - 1) QuickSort(list, inicio, i - 1);
        if(i + 1 <= fim) QuickSort(list, i + 1, fim);
    }

    public static void main(String[] args) {

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
//        System.out.println("Vetor original: " + list);

        // Começa a contar o tempo de execução
        long tempo_ini = System.currentTimeMillis();

        // Aplica o quick sort
        QuickSort(list, 0, list.size() - 1);

        // Termina a contagem de tempo
        long tempo_fin = System.currentTimeMillis();

        // Imprime o vetor ordenado
//        System.out.println("Vetor ordenado: " + list);
        
        // Código para a verificação da corretude
//        for(int i = 0; i < list.size(); i++) {
//            System.out.println(list.get(i));
//        }

        // Imprime o tempo de execução
        System.out.println("Tempo de execução: " + (tempo_fin - tempo_ini) + "ms");
    }
}
import java.util.*;

public class prime {
    private int[] PrimeTable;

    public prime(){ }
    public static void main(String[] args) {
        Scanner sc1 = new Scanner(System.in);
        Scanner sc2 = new Scanner(System.in);

        int inputNum;
        String inputString;
        String[] result;
        ArrayList<Integer> numList;
        boolean InRange = true;
        while (true){
            System.out.print(">> Input the number of numbers to process: ");
            inputNum = sc1.nextInt();

            if(inputNum < 2 || inputNum > 30){
                System.out.println("Input count range should be  \"1 < [INPUT] < 31\" \n");
                continue;
            }
            System.out.print(">> Input the numbers to be processed:\n");

            inputString = sc2.nextLine();
            result = inputString.split(" ");
            if(result.length != inputNum) {
                System.out.println("Input count Error!");
                continue;
            }


            numList = new ArrayList();
            for(int i=0; i<inputNum; i++){
                numList.add(Integer.parseInt(result[i]));
            }


            for(int i=0; i<numList.size(); i++){
                if(numList.get(i) < 1 || numList.get(i) > 100000){
                    InRange = false;
                    break;
                }
            }

            if(!InRange){
                System.out.println("Range : 1 ~ 100,000");
                InRange = true;
                numList.clear();
                continue;
            }

            long startTime = System.nanoTime();
            HashSet<Integer> uniqueSet = new HashSet<>(numList);
            ArrayList<Integer> uniqueList = new ArrayList<>(uniqueSet);
            Collections.sort(uniqueList); // uniqueList : already sorted


            int GCD = uniqueList.get(0);
            prime pr = new prime();
            for(int i=0; i<uniqueList.size() - 1; i++){
                GCD = pr.get_GCD(GCD, uniqueList.get(i));
            }
            System.out.println("GCD of input number is " + GCD + "\n");

            pr.PrimeTable = new int[uniqueList.get(uniqueList.size()-1) + 1];
            for(int i=0; i<uniqueList.size()-1; i++)
                pr.count_Prime(uniqueList.get(i), uniqueList.get(i+1));

            long endTime = System.nanoTime();
            double diffTime = (endTime - startTime) / 1000000000.0;
            System.out.println("Total execution time using Java is " + diffTime + " seconds!");
            break;


        }
    }
    int get_GCD(int a, int b){
        int n;
        while(a!=0){
            n = b%a;
            b = a;
            a = n;
        }
        return b;
    }
    void count_Prime(int min, int max){
        int cnt = 0;

        for(int i=2; i<=max; i++){
            PrimeTable[i] = 1;
        }

        for(int i=2; i<=max; i++){
            if(PrimeTable[i] == 0)
                continue;
            for(int j=2*i; j<=max; j+=i)
                PrimeTable[j] = 0;
        }

        for(int i=min; i<=max; i++){
            if(PrimeTable[i] != 0)
                cnt++;
        }

        System.out.println("Number of prime numbers between " + min + ", " + max + ": " + cnt);
    }
}

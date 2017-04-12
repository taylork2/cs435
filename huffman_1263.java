//TU TAYLOR cs435 1263 mp

import java.io.*;

public class huffman_1263 {
    public huffman_1263(){}

    //read in the file contents 
    private String readFile(String fileName){
        String message = null;

        try {
            FileReader fr = new FileReader(fileName);
            BufferedReader br = new BufferedReader(fr);

            String line = null;
            message = "";
            while ((line = br.readLine()) != null ){
                message += line; 
            }

            br.close();
        }
        catch(FileNotFoundException ex) {
            System.out.println("Unable to open file " + fileName + ".");
        }
        catch(IOException ex) {
            ex.printStackTrace();
        }

        return message;
    }

    public static void main (String args[]){
        if (args.length > 0){
            System.out.println("Hello world!"); 
            huffman_1263 test = new huffman_1263();
            System.out.println(test.readFile(args[0]));
        }
        
    }
}
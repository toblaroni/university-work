package comp1721.cwk1;

import java.util.List;
import java.util.Scanner;
import java.io.IOException;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;


public class WordList {
  // MAKE AN ARRAY LIST TO STORE THE LSIT OF WORDS  
  private List<String> words = new ArrayList<>();
 
  public WordList(String filename) throws IOException {
    Path path = Paths.get(filename);
    Scanner input = new Scanner(path);

    while(input.hasNextLine()){
      words.add(input.nextLine());
    }

    input.close();
  }

  public int size(){
    return words.size();
  }

  public String getWord(int n){
    if(n < 0 || n >= size()){
      throw new GameException("Word Index out of bounds.");
    }
    return words.get(n);
  }
}

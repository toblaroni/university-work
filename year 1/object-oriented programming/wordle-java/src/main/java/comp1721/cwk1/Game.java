package comp1721.cwk1;

import java.time.*;
import java.util.ArrayList;
import java.util.List;
import java.io.IOException;
import java.io.PrintWriter;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;


public class Game {
  private int gameNumber;
  private String target;

  private List<String> outputStrings = new ArrayList<>();
  
  // ONE PARAMETER CONSTRUCTOR 
  public Game(String filename) throws IOException { 
    WordList words = new WordList(filename);

    LocalDate firstDate = LocalDate.of(2021, 6, 19);
    LocalDate dateNow = LocalDate.now();

    int difference = Period.between(firstDate, dateNow).getDays();

    this.target = words.getWord(difference);
  }

  // TWO PARAMETER CONSTRUCTOR
  public Game(int num, String filename) throws IOException {
    WordList words = new WordList(filename);

    this.gameNumber = num;
    this.target = words.getWord(this.gameNumber);
  }

  public void play(){
    // SET THE GUESS NUMBER TO 1
    int guessNumber = 1;
    // MAKE THE GAME LOOP

    while(true){
      // IF THE MAX AMOUNT OF GUESSES HAS BEEN EXCEEDED BREAK OUT THE LOOP
      if(guessNumber > 6){
        System.out.println("Nope - Better luck next time!\n" + target);
        break;

      } 

      // CREATE NEW GUESS OBJECT      
      Guess guess = new Guess(guessNumber);

      System.out.printf("Enter guess (%d/6): ", guessNumber);
      guess.readFromPlayer();

      // PRINT THE FORMATTED STRING
      String outputString = guess.compareWith(target);
      System.out.println(outputString);

      // SAVE THE OUTPUT STRING TO THE ARRAYLIST TO USE IN THE SAVE() FUNCTION
      outputStrings.add(outputString);

      // IF THE WORDS ARE THE SAME PRINT ONE OF THE MESSAGES AND EXIT THE LOOP
      if(guess.matches(target)){
        if(guessNumber == 1){
          System.out.println("Superb - Got it in one!");
        } else if(guessNumber >=2 && guessNumber <=5){
          System.out.println("Well done!");
        } else {
          System.out.println("That was a close call!");
        }
        break;
      } 

      // INCREMENT THE GUESS NUMBER AT THE END OF EVERY UNSUCCESSFUL GUESS      
      guessNumber ++;
    }
  }

  public void save(String filename){

    Path path = Paths.get(filename);

    try (PrintWriter out = new PrintWriter(Files.newBufferedWriter(path))){
      for(String s : outputStrings){
        out.println(s);
      }
    } catch (IOException e){
      System.err.println("ERROR: Failed to print to file.");
      System.exit(1);
    }
  }
}

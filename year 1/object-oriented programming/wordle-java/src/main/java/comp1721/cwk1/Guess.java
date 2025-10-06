package comp1721.cwk1;

import java.util.Scanner;


public class Guess {
  // Use this to get player input in readFromPlayer()
  private static final Scanner INPUT = new Scanner(System.in);

  private int guessNumber; 
  private String chosenWord; // WORD INPUTTED BY THE USER

  public Guess(int num){
    if(validateNum(num)){
      this.guessNumber = num;
    } else {
      throw new GameException("Guess number out of range.");
    }
  }

  public Guess(int num, String word){
    if(validateNum(num)){
      this.guessNumber = num;
    } else {
      throw new GameException("Guess number out of range.");
    }

    if(validateWord(word)){
      this.chosenWord = word.toUpperCase();
    } else {
      throw new GameException("Invalid word.");
    }
  }

  // METHOD FOR VALIDATING GUESS NUMBER
  private boolean validateNum(int n){
    if(n >= 1 && n <=6){
      return true;
    } else return false;
  }

  private boolean validateWord(String w){
    if(w.matches("[a-zA-Z]+") && w.length() == 5){
      return true;
    } else return false;
  }

  // GETTER FUNCTIONS
  public int getGuessNumber(){
    return this.guessNumber;
  }

  public String getChosenWord(){
    return this.chosenWord;
  }
  
  
  // IF THE NEXT STRING FROM THE INPUT IS VALID, ASSIGN IT TO THE CHOSEN WORD VALUE
  public void readFromPlayer(){
    String input = INPUT.next();
    
    while(!validateWord(input)){
      System.out.println("Invalid word, try again.");
      System.out.printf("Enter guess (%d/6): ", guessNumber);

      input = INPUT.next();
    }

    this.chosenWord = input.toUpperCase();
  }

  public String compareWith(String target){
    StringBuilder sb = new StringBuilder("");

    // TURN THE TARGET AND CHOSEN WORDS TO AN ARRAY OF CHARS    
    char[] targetArray = target.toCharArray();
    char[] chosenArray = this.chosenWord.toCharArray();

    // LOOP THROUGH EACH LETTER OF THE CHOSEN ARRAY AND COMPARE WITH TARGET
    for(int i = 0; i < 5; ++ i){

      // COUNT THE OCCURENCES OF THE CURRENT CHAR
      int charCount = 1;
      for(int j = 0; j < 5; ++j){
        if(chosenArray[i] == target.charAt(j)) 
          ++ charCount;
      }

      // COUNT HOW MANY TIMES THE GIVEN LETTER HAS ALREADY OCCURED
      int currentCharOccurences = 0;
      for(int j = 0; j <= i; ++j){
        if(chosenArray[i] == chosenWord.charAt(j)) 
          ++ currentCharOccurences;
      }

      if(targetArray[i] == chosenArray[i]){
        // OUTPUT THE LETTER IN GREEN IF THEY MATCH
        sb.append("\033[30;102m " + chosenArray[i] + " \033[0m");

      } else if(target.contains(Character.toString(chosenArray[i])) && currentCharOccurences < charCount){
        // IF THE AMOUNT OF TIMES THE LETTER OCCURS HAS NOT BEEN REACHED 
        // OUTPUT THE LETTER IN YELLOW 
          sb.append("\033[30;103m " + chosenArray[i] + " \033[0m");

      } else {
        // ELSE OUTPUT IN WHITE
        sb.append("\033[30;107m " + chosenArray[i] + " \033[0m");
      }
    }
    // TURN THE ARRAY LIST INTO A STRING
    String outputString = sb.toString();
    return outputString;
  }

  public boolean matches(String target){
    if(target.equals(chosenWord)){
      return true;
    } else return false;
  }

}

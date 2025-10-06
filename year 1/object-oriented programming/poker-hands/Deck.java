package comp1721.cwk2;

import java.util.Collections;

/**
 * Represents a deck of playing cards.
 * 
 * <p>Made as a solution to OOP coursework 2.</p>
 * 
 * @author Toby Hutchinson
 */
public class Deck extends CardCollection{

  /**
   * Creates a full deck of cards in the right order.
   * 
   */
  public Deck(){
    for(Card.Suit s : Card.Suit.values()){
      for(Card.Rank r : Card.Rank.values()){
        Card c = new Card(r, s);
        this.cards.add(c);
      }
    }
    this.sort();
  }

  /**
   * Returns the first card in the deck and removes that card from the deck.
   * 
   * @return returns the first card in the deck
   */
  public Card deal(){
    if(this.cards.size() > 0){
      Card c = this.cards.get(0);
      this.cards.remove(0);
      return c;
    } else {
       throw new CardException("Cannot deal from an empty deck.");
    }
  }

  /**
   * Shuffles the deck randomly.
   */
  public void shuffle(){
    Collections.shuffle(this.cards);
  }
}
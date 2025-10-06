package comp1721.cwk2;

import java.util.Collections;

/**
 * A class that represents a hand in 5-card draw poker.
 *
 * <p>A class that represents a hand in 5 card poker.
 * Essentially it's a set of five cards, using the Card class.
 * Contains methods to test for different card hands.</p>
 *  
 * @author Toby Hutchinson
 */

public class PokerHand extends CardCollection{
  final static int FULL_SIZE = 5;

  /**
   * Creates an empty card.
   */
  public PokerHand(){
    super();
  }

  /**
   * Creates a hand of cards from the cards given as arguments.
   * 
   * <p> Allows the user to specify the cards in a hand. </p>
   * 
   * @param c Should be in two-character form, separated by spaces. AS = ACE OF SPADES Should be in two-character form, separated by spaces. AS = ACE OF SPADES Should be in two-character form, separated by spaces. AS = ACE OF SPADES Should be in two-character form, separated by spaces. AS = ACE OF SPADES.
   */
  public PokerHand(String c){
    String s[] = c.split(" ");

    if(s.length > FULL_SIZE){
      throw new CardException("Cannot have more than 5 cards in a hand.");
    }
    for(String str : s){
      Card card = new Card(str);

      this.add(card);
    }
  }

  /**
   * Adds a card to the hand
   * 
   * @param card The card to be added.
   */
  public void add(Card card){
    if(this.cards.size() == FULL_SIZE){
      throw new CardException("Cannot have more than 5 cards in 1 hand.");
    }
    // If The card is already in the hand throw an exception.
    for(Card c : this.cards){
      if(c.equals(card)){
        throw new CardException("Card is already in this hand.");
      }
    }
    this.cards.add(card);
  }

  /**
   * Discards all the cards in the hand.
   */
  public void discard(){
    if(this.cards.size() > 0){
      this.cards.clear();
    } else {
      throw new CardException("Hand is empty.");
    }
  }

  /**
   * Discards the cards in the hand to a specified deck.
   * 
   * @param d The deck to be discarded to.
   */
  public void discardTo(Deck d){
    if(this.cards.size() > 0){
      for(Card c : this.cards){
        d.add(c);
      }
    } else {
      throw new CardException("Hand is empty.");
    }
    this.discard();
  }

  /**
   * Tests if the hand contains a pair.
   * 
   * @return returns true if there's a pair and false if not.
   */
  public boolean isPair(){
    if(cards.size() != FULL_SIZE) return false;

    int same;
    int pairs = 0;

    for(Card c : this.cards){
      same = 0;
      for(Card c2 : this.cards){
        if(c.getRank() == c2.getRank() && !c.equals(c2)){
          same ++;
        }
      }
      if(same == 1){
        pairs ++;
      }
    }

    if(pairs == 2){
      return true;
    } 
    return false;
  }

  /**
   * Tests if there's two pairs in a hand.
   * 
   * @return returns true if there's two pairs and false if not.
   */
  public boolean isTwoPairs(){
    if(cards.size() != FULL_SIZE) return false;

    int same;
    int pairs = 0;

    for(Card c : this.cards){
      same = 0;
      for(Card c2 : this.cards){
        if(c.getRank() == c2.getRank() && !c.equals(c2)){
          same ++;
        }
      }
      if(same == 1){
        pairs ++;
      }
    }

    if(pairs == 4){
      return true;
    } 
    return false;
  }

  /**
   * Tests if there's three of a kind in a hand.
   * 
   * @return returns true if there's three of a kind and false if not.
   */
  public boolean isThreeOfAKind(){
    if(cards.size() != FULL_SIZE) return false;

    int same;
    int triples = 0;

    for(Card c : this.cards){
      same = 0;
      for(Card c2 : this.cards){
        if(c.getRank() == c2.getRank() && !c.equals(c2)){
          same ++;
        }
      }
      if(same == 2){
        triples ++;
      }
    }

    if(triples == 3 && !this.isFullHouse()){
      return true;
    } 
    return false;
  }

  /**
   * Tests if there's four of a kind
   * 
   * @return returns true if there's four of a kind and false if not.
   */
  public boolean isFourOfAKind(){
    if(cards.size() != FULL_SIZE) return false;

    int same;

    for(Card c : this.cards){
      same = 0;
      for(Card c2 : this.cards){
        if(c.getRank() == c2.getRank() && !c.equals(c2)){
          same ++;
        }
      }
      if(same == 3){
        return true;
      }
    }
    return false;
  }

  /**
   * Tests if there's a full house in a hand.
   * 
   * @return returns true if there's a full house and false if not.
   */
  public boolean isFullHouse(){
    if(cards.size() != FULL_SIZE) return false;

    int same;
    int doubles = 0;
    int triples = 0;

    for(Card c : this.cards){
      same = 0;
      for(Card c2 : this.cards){
        if(c.getRank() == c2.getRank() && !c.equals(c2)){
          same ++;
        }
      }
      if(same == 2){
        triples ++;
      } else if(same == 1){
        doubles ++;
      }
    }

    if(triples == 3 && doubles == 2){
      return true;
    } 
    return false;
  }

  /**
   * Tests if there's a flush in a hand.
   * 
   * @return returns true if there's a flush and false if not.
   */
  public boolean isFlush(){
    if(cards.size() != FULL_SIZE) return false;

    int same;

    for(Card c : this.cards){
      same = 0;
      for(Card c2 : this.cards){
        if(c.getSuit() == c2.getSuit() && !c.equals(c2)){
          same ++;
        }
      }
      if(same == 4){
        return true;
      }
    }
    return false;
  }

  /**
   * Tests if there's a straight in a hand.
   * 
   * @return returns true if there's a straight in a hand and false if not.
   */
  public boolean isStraight(){
    return false;
  }

  @Override
  public String toString(){
    String s = ""; 
    for(Card c : this.cards){
      s += c.toString() + " ";
    }
    return s.trim();
  }
}
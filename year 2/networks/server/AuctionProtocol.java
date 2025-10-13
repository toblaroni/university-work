import java.util.ArrayList;


/* Class that handles all the auctio logic.
 * Contains functions for showing, adding and bidding items in the auction.
 * The items are passed from the server to the client handler and to the protocol. */
public class AuctionProtocol {

    // Array list of string arrays
    // Items will be stored as [item, bid, IP]
    private ArrayList<String[]> items = null;

    private String request, IP = null;
    private String[]reqArr     = null;

    private StringBuilder retStr = null;

    private String itemName = null;
    private String bidItem  = null;
    private float newBid    = 0;
    private float oldBid    = 0;

    public AuctionProtocol( String request, String IP, ArrayList<String[]> items ) {
        this.request = request;
        this.IP      = IP;
        this.reqArr  = request.split(" ");
        this.items   = items;

    }

    // Function for showing everything in the auction;
    private String show() {

        if ( items.isEmpty() ) 
            return "There are currently no items in this auction.";
        
        retStr = new StringBuilder();

        // Loop throught the items and add them to the stringbuilder
        for ( int i = 0; i < items.size(); ++i ) {
            // Add the string as <item> : <bid> : <IP>
            retStr.append( items.get(i)[0] );  // Item
            retStr.append( " : " );
            retStr.append( items.get(i)[1] );  // Bid
            retStr.append( " : " );
            retStr.append( items.get(i)[2] );  // Bidder
            retStr.append( "\n" );  // Bidder
        }

        return retStr.toString(); // Return the stringbuilder as a string

    }

    
    // Function for adding an item to the auction
    private String addItem() {
        if ( reqArr.length != 2 ) 
            return "Error: Usage 'item <string>'";

        itemName = reqArr[1];

        // Check the item doesn't already exist
        for ( String[] item : items )
            if ( item[0].equals(itemName) ) 
                return "Failure.";
            

        // Add the item to items
        String[] item = { itemName, "0.0", "<no bids>" };
        items.add( item ); 

        return "Success.";
    }

    
    // A function for making a bid to the auction
    private String makeBid() {
        if ( reqArr.length != 3 )
            return "Error: Usage 'bid <item> <value>'";
        
        // Get the item to bid on
        bidItem = reqArr[1];

        // Try and turn the value string into an int
        try {
            newBid = Float.parseFloat(reqArr[2]);
        } catch ( NumberFormatException e ) {
            return "Error: Bad bid Usage 'bid <item> <value>'";    
        }

        // Update the bid
        for ( String[] item : items ) {
            if ( !item[0].equals(bidItem) )
                continue;

            oldBid = Float.parseFloat(item[1]);

            if ( oldBid >= newBid )
                return "Rejected.";

            // Update the bid for the item and set the bidder
            item[1] = reqArr[2];
            item[2] = IP;
            return "Accepted.";
        }

        return "Failure.";

    }

    
    // Function that handles the request and decides what to do with it.
    public String processRequest() {

        // If the request is show call the show() function
        if ( request.equalsIgnoreCase("show") )
            return show();

        // For item <string> we want to add the item to the array list and return success / failure
        if ( reqArr[0].equalsIgnoreCase("item") )
            return addItem();

        // For bid <item> <value> we want to update the current bid and return accepted or rejected
        if ( reqArr[0].equalsIgnoreCase("bid") )
            return makeBid();

        return "Error: Invalid request.";

    }
}

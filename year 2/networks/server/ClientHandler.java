import java.net.*;
import java.io.*;
import java.util.*;
import java.time.LocalDate;
import java.time.LocalTime;


// Handles each request on a new thread.
// Each thread is added to the fixed Size executor.
public class ClientHandler extends Thread {

    // The client socket that's passed in from the server
    private Socket socket = null;
    private ArrayList<String[]> items = null;

    // Logging
    private InetAddress inet  = null;
    private String clientAddr = null;
    private LocalDate date    = null;
    private LocalTime time    = null;
    private File lgFile       = null;
    private FileWriter fw     = null;

    private PrintWriter out   = null;
    private BufferedReader in = null;

    private String inputLine, outputLine = null;

    private AuctionProtocol p = null;


    public ClientHandler( Socket socket, ArrayList<String[]> items ) {
        super( "ClientHandler" );
        this.socket = socket;
        this.items = items;
    }


    // Function for logging each successful request to the log.txt file
    private void logRequest( String req ) {

        try {
            // Logging
            inet       = socket.getInetAddress();  // IP
            clientAddr = inet.getHostAddress();
            date       = java.time.LocalDate.now();
            time       = java.time.LocalTime.now();  // ** FORMAT **
                                                            //
            lgFile = new File("log.txt");
            fw = new FileWriter(lgFile, true);

            fw.write(date + " | " + time + " | " + clientAddr + " | " + req + "\n" );
            fw.close();

        } catch ( IOException e ) {
            System.err.println( "Error: Couldn't log request" );
            System.exit(1);
        }
    }


    // This is where all our code is ran when expanding thread.
    public void run() {

        try {

            // IO streams to/from the client
            out = new PrintWriter( socket.getOutputStream(), true );
            in = new BufferedReader( new InputStreamReader( socket.getInputStream() ));


            inputLine = in.readLine();  // read the line from the client

            // Process the input with the protocol
            p = new AuctionProtocol( inputLine, socket.getInetAddress().getHostAddress(), items );
            outputLine = p.processRequest();

            out.println( outputLine );

            // Only log if the request is successful.
            if ( outputLine.equalsIgnoreCase("accepted.") || outputLine.equalsIgnoreCase("rejected.") || 
                 outputLine.equalsIgnoreCase("Success.")  || outputLine.equalsIgnoreCase("Failure."))
                logRequest(inputLine);

            // Free up resources
            out.close();
            in.close();
            socket.close();

        } catch ( IOException e ) {
            e.printStackTrace();
        }
    }
}

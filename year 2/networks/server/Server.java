import java.net.*;
import java.io.*;
import java.util.ArrayList;
import java.util.concurrent.*;

// Server class that listens on port 6969 and then creates a new client handler for every new client.
public class Server {
	private ServerSocket server = null;
	private ExecutorService service = null;
	private ArrayList<String[]> items = new ArrayList<String[]>();
	private Socket client = null;

	public Server() {
		// Listen on port 6969
		try {
			server = new ServerSocket(6969);
			// Initialise the fixed size executor
			service = Executors.newFixedThreadPool(30);
		} catch ( IOException e ) {
			System.err.println( "Could not listen on port 6969. Port may be being used by another process." );
			System.exit(-1);
		}
	}

	public void runServer () {
		// For every new client we submit a new handler to the thread pool
		while ( true ) {
			try {
				client = server.accept();  // Blocks until connection is made
				service.submit( new ClientHandler(client, items) );
			} catch ( IOException e ) {
				e.printStackTrace();
				System.exit(1);
			}
		}
	}

	public static void main(String[] args) {
		Server s = new Server();
		s.runServer();
	}
}

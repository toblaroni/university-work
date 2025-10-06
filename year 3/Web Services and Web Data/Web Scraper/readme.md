# Scraper (Web Services Coursework 2)
Search tool that builds an inverted index by crawling a web page and allows users to find pages containing certain search terms.

### Usage
1. Make a virtual environment and activate it (Mac or Linux):
```
python -m venv venv
source venv/bin/activate
```
2. Install dependencies:
```
python -m pip install requirements.txt
```
3. Run the command line app:
```
python scraper.py
```

#### Features
Once the command line app is running, type:
- `build` - Build the inverted index and URL index
- `load` - Load the inverted index JSON file to memory (inverted index must be built)
- `print <word>` - Print the inverted index for `<word>` (inverted index must be loaded to memory)
- `find <query>` - Return all pages containing the word(s) in the query, ranked by relevance (inverted index must be loaded to memory)
- `exit` - Quit the app
- `help` - Print the available commands.



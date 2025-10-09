import requests
import time
import re
import json
import os
import copy

from bs4 import BeautifulSoup
from urllib.parse import urlparse, urljoin

from rich.console import Console
from rich.table import Table

class Crawler:
    def __init__(self, base_url):
        self.base_url = base_url
        self.base_domain = urlparse(base_url).netloc

        self.frontier = [ base_url ]  # List of URLs
        self.visited = set()

        self.politeness_window = 6  # Seconds
        self.last_request_time = 0

        self.inverted_index = {}
        self.url_dict = {}         # For storing the indices of the URLs themselves
        self.url_count = 1

        self.console = Console()
        self.is_loaded = False

        self.success_colour = "#28a745"
        self.error_colour = "#dc3545"


    def _fetch_page_contents(self, url):
        elapsed_time = time.time() - self.last_request_time
        if elapsed_time < self.politeness_window:
            wait_for = self.politeness_window - elapsed_time
            time.sleep(wait_for)

        try:
            res = requests.get(url=url)
            res.raise_for_status()
        except requests.exceptions.RequestException as e:
            self.console.print(f"Could not fetch {url}: {e}")
            return None

        self.last_request_time = time.time()
        self.visited.add(url)
        
        self.url_dict[self.url_count] = url        
        self.url_count += 1

        return res.text


    def _update_frontier(self, parsed_content):
        # Get all links in the current page, excluding ones to other websites
        for link in parsed_content.find_all('a'):
            link_url = link.get('href')

            if link_url:  # Ensure the href attribute exists
                parsed_link = urlparse(link_url)

                if parsed_link.netloc == self.base_domain:
                    absolute_url = link_url
                elif not parsed_link.netloc:
                    absolute_url = urljoin(self.base_url, link_url)
                else:
                    continue    # leads to different website

            if absolute_url not in self.visited and absolute_url not in self.frontier:
                self.frontier.append(absolute_url)


    def _update_index(self, parsed_content):
        # This stores the positions of each word on each page
        page_text = parsed_content.get_text().lower()

        words = re.findall(r'\w+', page_text)

        for i, word in enumerate(words):
            if len(word) < 3:
                continue
            
            self.inverted_index.setdefault(word, {})
            self.inverted_index[word].setdefault(self.url_count-1, [])

            # Store the positions in the page where the word is
            self.inverted_index[word][self.url_count-1].append(i) 


    def create(self):

        urls_processed = 0

        # BFS of pages
        with self.console.status("Creating Inverted Index...") as status:
            while self.frontier:
                current_url = self.frontier.pop(0)

                status.update(
                     f"[bold]Crawling:[/] [cyan]{current_url}[/] "
                     f"[dim]({urls_processed} pages indexed)[/]"
                 )

                # First we fetch the next page
                contents = self._fetch_page_contents(current_url)
                # Parse using BS4
                soup = BeautifulSoup(contents, "html.parser")
                # Update frontier
                self._update_frontier(soup)
                # Update index 
                self._update_index(soup)

                urls_processed += 1

        self.console.print(
            "\n========== Successfully Built Index ==========", 
            style=self.success_colour
        )
        self.is_loaded = True


    def save(self, output_dir="./"):
        if self.inverted_index == {}:
            self.console.print("Nothing to save. Build the index first...")
            return

        try:
            with open(os.path.join(output_dir, "index.json"), "w") as fp:
                json.dump(self.inverted_index, fp, indent=4)
            self.console.print("Successfully saved inverted index.", style=self.success_colour)
        except IOError as e:
            self.console.print(f"Error occurred while saving inverted index: {e}", style=self.error_colour)

        try:
            with open(os.path.join(output_dir, "url.json"), "w") as fp:
                json.dump(self.url_dict, fp, indent=4)
            self.console.print("Successfully saved URL index.", style=self.success_colour)
        except IOError as e:
            self.console.print(f"Error occurred while saving URL index: {e}", style=self.error_colour)


    def load(self, index_file="./index.json", url_file="./url.json"):
        if not os.path.exists(index_file):
            self.console.print(
                f"Error: No such file {index_file}. Inverted index could not be loaded.", 
                style=self.error_colour
            )
            return
        elif not os.path.exists(url_file):
            self.console.print(
                f"Error: No such file {url_file}. URL index could not be loaded.",
                style=self.error_colour
            )
            return

        try:
            with open(index_file, "r") as fp:
                self.inverted_index = json.load(fp)
            self.console.print("Successfully loaded inverted index.", style=self.success_colour)
        except IOError as e:
            self.console.print(f"Error occurred while loading inverted index: {e}", style=self.error_colour)

        try:
            with open(url_file, "r") as fp:
                self.url_dict = json.load(fp)
            self.console.print("Successfully loaded URL index.", style=self.success_colour)
        except IOError as e:
            self.console.print(f"Error occurred while loading URL index: {e}", style=self.error_colour)

        self.is_loaded = True


    def print(self, word):
        if not self.is_loaded:
            self.console.print("Error: Inverted index must be loaded or built.", style=self.error_colour)
            return

        word = word.lower()
        index = self.inverted_index.get(word)

        if not index:
            self.console.print(f"'{word}' does not exist in the inverted index")
            return

        table = Table(title=f"Index of '{word}'", show_lines=True)

        table.add_column("URL", justify="left", overflow="fold")
        table.add_column("Position(s)", justify="center")
        table.add_column("Occurrence Count", justify="center")
        
        for page in index:
            url = self.url_dict[page]
            positions = self.inverted_index[word][page]
            count = len(positions)

            table.add_row(url, ", ".join(map(str, positions)), str(count))

        self.console.print(table, justify="center")
        

    def find(self, query):
        if not self.is_loaded:
            self.console.print("Error: Inverted index must be loaded or built.", style=self.error_colour)
            return

        if not query:
            self.console.print("Usage: find <query>")
            return

        query = [ word.lower() for word in query ]

        page_sets = { word: {} for word in query }
        missing_words = []
        results = []

        start_time = time.time()

        # Grabbing the inverted index for the query words
        for word in query:
            if word.lower() in self.inverted_index:
                page_sets[word] = copy.deepcopy(self.inverted_index[word])
            else:
                missing_words.append(word)

        if len(missing_words) == len(query):
            self.console.print(f"No results could be found for [bold]'{' '.join(query)}'[/]")
            return

        url_lists = [ set(page_sets[page].keys()) for page in page_sets ]  # List of lists of URLs for each word
        common_pages = set.intersection(*url_lists) if not missing_words else None

        potential_exact_matches = None
        if common_pages:
            potential_exact_matches = { word: {} for word in page_sets } # Holds all 

            # Snip the page_set to contain only URLs that contain all words
            for word in page_sets:
                for url in page_sets[word]:
                    if url in common_pages:
                        potential_exact_matches[word][url] = page_sets[word][url]

        # Search for exact matches. We can skip this if there are missing words or 
        if common_pages and len(query) != 1:
            exact_matches = self._find_exact_matches(potential_exact_matches)
            results.extend(
                sorted(exact_matches, key=exact_matches.get, reverse=True)
            )

        # Remove all URL's that contain exact matches from potential_exact_matches
        if potential_exact_matches:
            for word, url_positions in potential_exact_matches.items():
                for url in results & url_positions.keys():
                    url_positions.pop(url)

        # Rank the pages that contain all words based on occurrences
        if potential_exact_matches:
            contains_all_words = self._count_occurrences(potential_exact_matches)
            results.extend(
                sorted(contains_all_words, key=contains_all_words.get, reverse=True)
            )
            
        # Remove all URLs that have already been added to results
        for word, url_positions in page_sets.items():
            for url in results & url_positions.keys():
                url_positions.pop(url)

        contains_some_words = self._count_occurrences(page_sets)
        results.extend(
            sorted(contains_some_words, key=contains_some_words.get, reverse=True)
        )

        elapsed_time = time.time() - start_time

        table = Table(
            title=f"Results for '{' '.join(query)}'", 
            show_lines=True,
            caption=f"{len(results)} Result(s) found ({elapsed_time:.2} seconds)",
            show_header=False
        )

        table.add_column("Position", justify="center")
        table.add_column("URL", justify="center")

        for i, result in enumerate(results):
            url = self.url_dict[result]
            table.add_row(str(i+1), url)

        self.console.print(table, justify="center")


    def _find_exact_matches(self, potential_exact_matches):
        # we know that the urls in potential_exact_matches for each word are all shared by every word.
        # therefore we can just loop through the first word's urls
        first_word = list(potential_exact_matches.keys())[0]
        exact_matches = { url: 0 for url in potential_exact_matches[first_word] }     # dictionary of urls and number of exact matches

        for url in potential_exact_matches[first_word]:
            for first_word_position in potential_exact_matches[first_word][url]:
                exact_match = True
                for i, word in enumerate(potential_exact_matches):
                    if word == first_word:
                        continue

                    found = False
                    for position in potential_exact_matches[word][url]:
                        if position == first_word_position+i:
                            found = True
                            break

                    if not found:
                        exact_match = False
                        break

                if exact_match:
                    exact_matches[url] += 1 

        # Filter out urls with no matches
        exact_matches = {
            url: exact_matches[url] 
                for url in exact_matches if exact_matches[url] != 0 
        }

        return exact_matches


    def _count_occurrences(self, index):
        # Counts the occurrences of times words occur in the pages where all words are present
        
        occurrence_count = {    # dictionary of urls and number of exact matches 
            url: 0 
            for _, urls in index.items() 
            for url in urls 
        } 

        for url in occurrence_count:
            for word in index:
                if url in index[word]:
                    occurrence_count[url] += len(index[word][url])

        return occurrence_count
            

def main():
    crawler = Crawler(base_url="https://quotes.toscrape.com/")

    while True:
        user_input = input(" > ").strip().split()

        if not user_input:
            continue

        cmd = user_input[0]
        args = user_input[1:]

        if cmd == "build":
            crawler.create()
            crawler.save()
        elif cmd == "load":
            crawler.load()
        elif cmd == "print":
            if len(args) != 1:
                crawler.console.print("Usage: 'print <word>'")
                continue
            crawler.print(args[0])
        elif cmd == "find":
            if len(args) == 0:
                crawler.console.print("Usage: 'find <query>'")
                continue
            crawler.find(args)
        elif cmd == "exit":
            break
        elif cmd == "help":
            print("Commands:")
            print("build - Create inverted index.")
            print("load - Load inverted index.")
            print("print <word> - Print out the inverted index for <word>")
            print("find <query> - Search the inverted index for <query>")
            print("exit - Quit program")
            print("help - Show help message")
        else:
            crawler.console.print(f"Command not found '{cmd}'.")


if __name__ == "__main__":
    main()

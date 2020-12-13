// Document::to_lower() is currently not working

#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>

using namespace std;

class Document
{
    private:
	//creates the vectors words and stop_words to store words in, creates the int stop_words_count
	//these are private, only to be used by class Document
        vector<string> words;
        vector<string> stopWords;
        int stopWordsCount;
    public:
    //creates a Document and reads every word on it (separated by whitespace) into vector words
        Document(string filename)
        {
            fstream file;
            file.open(filename);
            string word;
            while(file >> word)
            {
                if(!(none_of(word.begin(), word.end(), [](unsigned char c) {return isalpha(c);})))
                    words.push_back(word);
            }
        }
	//returns words
        vector<string> getWords()
        {
            return words;
        }
    //puts all of the words to lowercase
        void to_lower()
        {
            for (string& word : words)
            {
                int i = 0;
                char c;
                for (int i=0; i < word.length(); i++)
                    putchar(tolower(word[i]));
            }
        }
	//sets the stopwords
        void setStopWords(vector<string> stopWordList)
        {
            stopWords = stopWordList;
        }
	//sees if a word is a stopword
        bool isStopWord(string word)
        {
            return find(stopWords.begin(), stopWords.end(), word) != stopWords.end();
        }
	//removes the stop words and creates int count to count how many stopwords there are
        void removeStopWords()
        {
            int count = words.size();
            words.erase(remove_if(words.begin(), words.end(), [this] (string s) {return this -> isStopWord(s);}), words.end());
            stopWordsCount = count - words.size();
        }
	//returns the final count of stopwords
        int getStopWordsCount()
        {
            return stopWordsCount;
        }
	//sees if a char is a punctuation
        bool isPunc(char c)
        {
            return !(isalpha(c));
        }
	//removes chars that are punctuations
        void removePunc()
        {
            for(string& word : words)
            {
                word.erase(remove_if(word.begin(), word.end(), [this] (char c) {return this -> isPunc(c);}), word.end());
            }
        }
};
class WordCount
{
	//creates map wordCount and the vector words these are private to be used by class WordCount
    private:
        map<string,int> wordCount;
        vector<string> words;
    public:
	//counts the words in wordList
        WordCount(vector<string> wordList)
        {
            words = wordList;		//sets words equal to wordList
            vector<string> uniqueList;	//creates a uniqueList vector
            for(string word : wordList)
                if(find(uniqueList.begin(), uniqueList.end(), word) == uniqueList.end()) uniqueList.push_back(word);
            for(string word : words)
            {
                int numWords = count(wordList.begin(), wordList.end(), word);
                wordCount.insert(pair<string, int> (word, numWords));
            }
        }
	//prints out WordCount
        void printWordCount()
        {
            for(auto const& item : wordCount) cout << "\t" << item.first << " : " << item.second << endl;
        }
	//gets total amount of each word
        int getTotal()
        {
            int total = 0;
            for(auto const& item : wordCount) total += item.second;
            return total;
        }
};
class NGram
{
    private:
	//creates the vectors words and ngrams, these are only used by class NGram
        vector<string> words;
        vector<string> ngrams;
    public:
        NGram(Document document, int n)
        {
            words = document.getWords();
            if(n==1) return;
            for(int i = 0; i < words.size(); i++)
            {
                string ngram = "";
                for(int j = i; j < i + n; j++)
                    if(j < words.size())
                        ngram = ngram + " " + words[j];
                ngrams.push_back(ngram);
            }
        }
	//removes NGrams
        vector<string> getNGrams()
        {
            return ngrams;
        }
};
int main()
{
	//user inputs file to which is then processed by the program to word count
    cout << "Enter the filename of words to read: ";
    string filename;
    cin >> filename;
    Document document(filename);
    document.removePunc();

	//user enters a number to be used by NGram
    cout << "Enter a number n for the n-grams: ";
    int n;
    cin >> n;
    NGram ngram(document, n);

	//user enters a stopwords file which will remove certain words from the first user-input file
    cout << "Enter the stop words filename: ";
    cin >> filename;
    Document stopWords(filename);
    document.setStopWords(stopWords.getWords());
    document.removeStopWords();

	//prints out n-gram word count and the total number of n-grams
    cout << "\n" << n << "-gram word count: \n";
    WordCount ngramCount(ngram.getNGrams());
    ngramCount.printWordCount();
    cout << "Total number of " << n << "-gram counts are " << ngramCount.getTotal();

	//prints out results of running WordCount on the first user-input file
    cout << "\nWord Count Results: \n";
    WordCount wordcount(document.getWords());
    wordcount.printWordCount();
    cout << "Total words in the document is " << wordcount.getTotal();
    cout << "\nNumber of removed stopwords is " << document.getStopWordsCount() << "\n";
}

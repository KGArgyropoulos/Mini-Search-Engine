**Mini search engine implementation** \
This application is using an inverted index, in order to store a set of documents and respond with keyword queries, returning to the user the top-k texts related to the query. 


**Compile & Execute** \
Compilation command: make \
Execution command: ./minisearch -i docfile -k K \
where docfile is the name of the file which contains the documents to be stored in the program \
and K is the parameter which indicates the number of results that should be returned in each query.

**Interface** \
Options that can be given when the program is in running mode
- /search q1 q2 ... q10 \
    search for given words into documents 
- /df \
    print the document frequency of the vector 
- /tf id word \
    print the term frequency of the specific word in document with specific id 
- /exit \
    exit from the application 

**Data Structures** 
- Map of the content of each text.\
    * Form: [id | Text] \
- Inverted index, which is the matching between every word appearing in the texts and the texts \
* Form: [word | [text_1,tf_1] [text_2,tf_2] ...] \
- Posting List, which is the list on the left of the inverted index ([text_1,tf_1] [text_2,tf_2]).\
- Trie, which is implemented is order to accelerate our search inside the inverted index. \
* Trie stores the words of the texts. Its leaves hold the corresponding posting list. [trie.png]

**Score Calculation** \
score.png
- qi : query's word\
- f(qi, D) : term frequency qi, in the text\
- |D| : number of words in the text\
- avgdl : average number of words from every text of the index\
- IDF(qi) : inverse document frequency (shows in how many texts the word qi appears) and is calculated as:\
* N : number of texts in the index\
* n(qi) : number of texts containing the word qi.\
* k1, b : parameters, helpinf as to optimize the search engine. For this project, let k1=1.2 and b=0.75.\

After we calculate the score for every text, we find the k-highest scores and present them to the user.

**Implementation** \
- minisearch.c \
* The program reads the input file and stores its texts (lines) to the map array.\
* Then the texts enter the inverted index structure(and trie), word-by-word.\
* User commands are stored in a string array,of size 11 (max. 10 word-queries+the command)\
- insert.c \
* Trie and posting list initialization + trie insertion \
- Trie Struct includes \
* Pointer to the next level (downwards the tree) \
* Pointer to the same level (right at the tree) \
* Pointer to posting list, initialized as NULL for every node and gets value when the last letter of the word is added \
* A string containing the whole word that gets its value when the last letter of the word is added \
* The content of each node (stored letter) \
- Posting List Struct includes \
* A pointer to a new posting list, in case the word appears in more than one texts \
* Content of each node, which is the id of the text in which the word is found and the term frequency \
- search.c \
* Calculates the score of each text \
* Prints the results of each query \
* Other auxiliary functions \
- t_df.c \
* Finds and prints the results of the df,df [word],tf [id] [word] and exit user's commands, using the structures we saw above.

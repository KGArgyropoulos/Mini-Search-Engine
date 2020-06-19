**Mini search engine implementation** \
This application is using an inverted index, in order to store a set of documents and respond with keyword queries, returning t to the user the top-k texts related to the query. 


**Compile & Execute** \
Compilation command: make
Execution command: ./minisearch -i docfile -k K \
where docfile is the name of the file which contains the documents to be stored in the program \
and K is the parameter which indicates the number of results that should be returned in each query.

**Interface** \
Options that can be given when the program is in running mode\
- /search q1 q2 ... q10 \
    search for given words into documents 
- /df \
    print the document frequency of the vector 
- /tf id word \
    print the term frequency of the specific word in document with specific id 
- /exit \
    exit from the application 

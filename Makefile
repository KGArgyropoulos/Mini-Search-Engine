all: minisearch

minisearch: minisearch.o insert.o search.o t_df.o
	gcc minisearch.c insert.c search.c t_df.c -o minisearch -lm

minisearch.o: minisearch.c
	gcc -c minisearch.c

insert.o: insert.c
	gcc -c insert.c

search.o: search.c
	gcc -c search.c

t_df.o: t_df.c
	gcc -c t_df.c

clean:
	rm -rf *o minisearch

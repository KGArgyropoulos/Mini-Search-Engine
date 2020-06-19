#include "minisearch.h"

void df(trie *root,int flag){

	trie *temp;
	if(flag==0)
		temp=root->next_level;
	else
		temp=root->same_level;

	if(temp->eoString){
		printf("%s ",temp->leaf);
		posting_list *pl=temp->eoString;
		int freq=1;
		while(pl->next){
			pl=pl->next;
			freq++;
		}
		printf(" %d\n", freq);
	}
	if(temp->next_level){
		df(temp,0);
		if(temp->same_level){
			df(temp,1);
		}
	}
	else if(temp->same_level){
		df(temp,1);
	}

}

int df_word(trie *root,char *word){

	trie *temp=root->next_level;
	int i=0;
	int wfreq=0;
	while(i<strlen(word)){
		if(temp->letter==word[i]){
			i++;
			if(i==strlen(word)){
				//found
				if(temp->eoString){
					posting_list *pl=temp->eoString;
					wfreq++;
					while(pl->next){
						pl=pl->next;
						wfreq++;
					}				
				}
			}
			else if(temp->next_level){
				temp=temp->next_level;
			}
			else{
				return 0;
			}
		}
		else{
			if(temp->same_level){
				temp=temp->same_level;
			}
			else{
				return 0;
			}
		}
	}
	return wfreq;
}

int tf(struct Node *root,int id,char *word){

	trie *temp=root->next_level;
	int i=0;
	int wfreq=0;
	while(i<strlen(word)){
		if(temp->letter==word[i]){
			i++;
			if(i==strlen(word)){
				//found
				if(temp->eoString){
					posting_list *pl=temp->eoString;
					if(pl->id==id){
						wfreq+=pl->tfs;
					}
					else{
						while(pl->next){
							pl=pl->next;
							if(pl->id==id){
								wfreq+=pl->tfs;
								break;
							}
						}
					}			
				}
			}
			else if(temp->next_level){
				temp=temp->next_level;
			}
			else{
				return 0;
			}
		}
		else{
			if(temp->same_level){
				temp=temp->same_level;
			}
			else{
				return 0;
			}
		}
	}
	return wfreq;
}

void destroyNodes(trie **root,int flag){

	trie *temp;
	if(flag==0){
		temp=(*root)->next_level;
	}
	else{
		temp=(*root)->same_level;
	}


	if(temp->next_level){
		destroyNodes(&temp,0);
		if(temp->same_level){
			destroyNodes(&temp,1);
			if(temp->eoString){
				free(temp->leaf);
				posting_list *pl=temp->eoString;
				while(pl->next){
					posting_list *temp1=pl;
					pl=pl->next;
					free(temp1);
				}
				free(pl);
			}
			free(temp);
			return;
		}
	}
	else if(temp->same_level){
		destroyNodes(&temp,1);
	}

	if(temp->eoString){
		free(temp->leaf);
		posting_list *pl=temp->eoString;
		while(pl->next){
			posting_list *temp1=pl;
			pl=pl->next;
			free(temp1);
		}
		free(pl);
	}
	free(temp);
}
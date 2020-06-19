#include "minisearch.h"

int main( int argc, char *argv[]){

	//////////////////open file//////////////////
	FILE *f;
	int resultsToBeReturned;
	int numOfLines;
	char *filename;
	if(argc == 3){
		if(strcmp(argv[1],"-i")==0){
			f = fopen(argv[2], "r");
			filename=argv[2];
			resultsToBeReturned=10;
		}
		else{
			printf("Error in passing the options\n");
			return -1;
		}
	}
	else if(argc == 5){
		if(strcmp(argv[1],"-i")==0){
			f = fopen(argv[2], "r");
			filename=argv[2];
			if(strcmp(argv[3],"-k")==0){
				resultsToBeReturned=atoi(argv[4]);
			}
			else{
				printf("Error in passing the options\n");
				return -1;
			}
		}
		else if(strcmp(argv[1],"-k")==0){
			resultsToBeReturned=atoi(argv[2]);
			if(strcmp(argv[3],"-i")==0){
				f = fopen(argv[4],"r");
				filename=argv[4];
			}
			else{
				printf("Error in passing the options\n");
				return -1;
			}
		}
		else{
			printf("Error in passing the options\n");
			return -1;
		}
	}
	else{
		printf("Error in passing the options\n");
		return -1;
	}
	if(resultsToBeReturned<1){
		printf("Error in results you want to return\n");
		return -1;
	}

	///////////check if the file is found///////////
	if (f == NULL){
		printf("Failed to open the file you typed\n");
		return -2;
	}

	///find # of lines and # of max string's length///
	numOfLines=countLines(filename);
	int maxStringLength=mSL(filename,numOfLines);

	///////////check if the ids are right///////////
	if(validIdCheck(filename,numOfLines)==0){
		return -3;
	}

	///////////////////map array///////////////////
	int i=0,ch=0,index=0;
	char **map_array;
	char temp[maxStringLength];
	map_array=malloc(numOfLines*sizeof(char*));
	int *SOELine=malloc(numOfLines*sizeof(int));
	while(i<numOfLines){
		ch=fgetc(f);
		if(ch=='\n'){
			temp[index]='\0';
			SOELine[i]=strlen(temp);
			map_array[i]=malloc(SOELine[i]*sizeof(char )+1);
			strcpy(map_array[i],temp);
			index=0;
			i++;
		}
		else if(index==0 && (ch==' ' || ch=='\t')){
			continue;
		}
		else if(ch=='\t'){
			temp[index++]=' ';
		}
		else{
			temp[index++]=ch;
		}
	}

	fclose(f);//close the file,now that we have everything in memory

	////////////////inverted index////////////////
	trie *root=init();
	int *wordsPerText=malloc(numOfLines*sizeof(int));
	for(i=0;i<numOfLines;i++){
		int chcounter=0;
		int textWords=0;
		char *content=malloc(SOELine[i]*sizeof(char));
		char *frcontent=content;
		content=strchr(map_array[i],' ');
		content++;
		do{
			while(content[chcounter]!=' ' && content[chcounter]!='\0'){
				chcounter++;
			}
			char *word=malloc(chcounter*sizeof(char)+1);
			word[chcounter]='\0';
			strncpy(word,content,chcounter);
			content+=chcounter;
			if(content[0]==' '){
				content++;
			}
			insertion(&root,i,word);
			textWords++;
			free(word);
			chcounter=0;
		}while(content[0]!='\0');
		wordsPerText[i]=textWords;
		free(frcontent);
	}

	/////////////command to be given/////////////
	char **input;
	int notUsed,time=1;
	do{
		if(time>1){
			for(i=0;i<11;i++){
				free(input[i]);
			}
			free(input);
		}
		printf("minisearch:\n");
		input=malloc(11*sizeof(char *));
		for(i=0;i<11;i++)
			input[i]=NULL;

		ch=0;
		index=0;
		while(ch!='\n' && index<=10){
			scanf("%ms",&input[index++]);
			ch=getchar();
		}
		if(index>10){
			while(ch!='\n'){
				ch=getchar();
			}
		}

		if(strcmp(input[0],"/search")==0){
			search(input,root,index,numOfLines,wordsPerText,resultsToBeReturned,map_array,SOELine);
		}
		else if(strcmp(input[0],"/df")==0){
			if(input[1]!=NULL && input[2]==NULL){
				int freq=df_word(root,input[1]);
				printf("%s %d\n",input[1],freq);
			}
			else if(input[1]==NULL){
				df(root,0);
			}
			else{
				printf("Error typing arguments for df command\n");
			}
		}
		else if(strcmp(input[0],"/tf")==0){
			if(input[1]!=NULL && input[2]!=NULL && input[3]==NULL){
				int id=atoi(input[1]);
				if(id==0 && input[1][0]!='0'){
					printf("Tf's first argument should be an integer\n");
				}
				else{
					int freq=tf(root,id,input[2]);
					printf("%s %d\n",input[2],freq);
				}
			}
			else{
				printf("Error typing arguments for tf command\n");
			}
		}
		else if(strcmp(input[0],"/exit")!=0){
			printf("Error typing the command\n");
		}

		time++;
	}while(strcmp(input[0],"/exit")!=0);

	for(i=0;i<11;i++){
		free(input[i]);
	}
	free(input);
	/////////////////////////////////////////////

	for(i=0;i<numOfLines;i++){
		free(map_array[i]);
	}
	free(map_array);
	free(SOELine);
	free(wordsPerText);

	destroyNodes(&root,0);
	free(root);

	return 0;
}

int countLines(char *filename){

	FILE *fp = fopen(filename,"r");
	int ch=0,lines=0;
	while(!feof(fp)){
		ch = fgetc(fp);
		if(ch == '\n'){
			lines++;
		}
	}
	
	fclose(fp);
	return lines;
}

int validIdCheck(char *filename,int numOfLines){

	FILE *fp = fopen(filename,"r");
	int *ids=malloc(numOfLines*sizeof(int));
	int i,ch=0;
	for(i=0;i<numOfLines;i++)
		ids[i]=0;

	for(i=0;i<numOfLines;i++){
		do{
			ch=fgetc(fp);
		}while(ch==' ' || ch=='\t');
		while(ch!=' ' && ch!='\t'){
			ids[i]*=10;
			ids[i]+=(ch-'0');
			ch=fgetc(fp);
		}
		while(ch!='\n'){
			ch=fgetc(fp);
		}
	}
	int *test_ids=malloc(numOfLines*sizeof(int));
	for(i=0;i<numOfLines;i++){
		test_ids[i]=i;
	}
	for(i=0;i<numOfLines;i++){
		if(test_ids[i]!=ids[i]){
			printf("Document's ids are invalid\n");
			free(ids);
			free(test_ids);
			return 0;
		}
	}

	free(ids);
	free(test_ids);
	fclose(fp);	
	return 1;
}

int mSL(char *filename,int numOfLines){

	FILE *fp = fopen(filename,"r");	
	int i,ch=0;
	int maxLengthOfLine=0;
	int lengthOfEachLine=0;
	for(i=0;i<numOfLines;i++){
		do{
			ch=fgetc(fp);
			lengthOfEachLine++;
		}while(ch!='\n');

		if(lengthOfEachLine>maxLengthOfLine){
			maxLengthOfLine=lengthOfEachLine;
		}
		lengthOfEachLine=0;
	}

	fclose(fp);
	return maxLengthOfLine;
}
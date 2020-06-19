#include "minisearch.h"

void search(char **input,trie *root,int index,int numOfLines,int *wordsPerText,
			int resultsToBeReturned,char **map_array,int *SOELine){
	//IDF
	double *IDF=malloc((index-1)*sizeof(double));
	int i,j,numOfTexts;
	double x,y,z;
	for(i=1;i<index;i++){
		numOfTexts=df_word(root,input[i]);
		x=numOfLines-numOfTexts+0.5;
		y=numOfTexts+0.5;
		z=x/y;
		IDF[i-1]=log(z);
	}
	//f(q,id)
	int **f=malloc(numOfLines*sizeof(int* ));
	for(i=0;i<numOfLines;i++){
		f[i]=malloc((index-1)*sizeof(int ));
	}
	//we will create an array to find the texts that don't include any of the given words
	int *flag_arr=malloc(numOfLines*sizeof(int));
	for(i=0;i<numOfLines;i++){
		for(j=1;j<index;j++){
			//f[i][j-1]=freq(root,input[j],i);
			f[i][j-1]=tf(root,i,input[j]);
		}
		int found=0;
		for(j=0;j<index-1;j++){
			if(f[i][j]>0){
				found=1;
				flag_arr[i]=1;
				break;
			}
		}
		if(found==0){
			flag_arr[i]=0;
		}
	}
	//rest of the variables in BM25 equation
	const double k1=1.2;
	const double b=0.75;
	int sum=0;
	for(i=0;i<numOfLines;i++){
		sum+=wordsPerText[i];
	}
	double avgdl=(double)(sum)/(double)numOfLines;
	//calculate score
	double **score=malloc(numOfLines*sizeof(double* ));
	for(i=0;i<numOfLines;i++){
		score[i]=malloc((index-1)*sizeof(double ));
	}
	for(i=0;i<numOfLines;i++){
		for(j=0;j<index-1;j++){
			x=f[i][j]*(k1+1);
			y=(double)((b*wordsPerText[i])/avgdl);
			y+=(1-b);
			y*=k1;
			y+=f[i][j];
			z=(double)(x/y);
			score[i][j]=IDF[j]*z;
		}
	}

	double *QScore=malloc(numOfLines*sizeof(double));
	for(i=0;i<numOfLines;i++){
		QScore[i]=0;
		for(j=0;j<index-1;j++){
			QScore[i]+=score[i][j];
		}
		//printf("%d :%.4f\n",i,QScore[i]);
	}
	//free everything we won't need from now on
	free(IDF);
	for(i=0;i<numOfLines;i++){
		free(f[i]);
		free(score[i]);
	}
	free(f);
	free(score);
	//now lets sort our scores
	int *id_sorting=malloc(numOfLines*sizeof(int ));
	for(i=0;i<numOfLines;i++){
		id_sorting[i]=i;
	}
	bubbleSort(id_sorting,QScore,flag_arr,numOfLines);
	//and finally print the results
	searchResults(id_sorting,QScore,map_array,resultsToBeReturned,SOELine,flag_arr,numOfLines,input,index);
	free(flag_arr);
	free(QScore);
	free(id_sorting);
}

void searchResults(int *id_sorting,double *QScore,char **map_array,
	int resultsToBeReturned,int *SOELine,int *flag_arr,int numOfLines,char **input,int index){

	int i,argsize,rank=1;
	if(resultsToBeReturned>numOfLines){
		resultsToBeReturned=numOfLines;
	}

	for(i=0;i<resultsToBeReturned;i++){
		if(flag_arr[i]==1){
			char *content=malloc(SOELine[id_sorting[i]]*sizeof(char));
			char *frcontent=content;
			content=strchr(map_array[id_sorting[i]],' ');
			content++;
			printf("%d.( %d)[ %.4f] %s\n",rank,id_sorting[i],QScore[i],content);
			argsize=findsize(QScore[i],rank,id_sorting[i]);
			align(content,input,index,argsize);
			free(frcontent);
			rank++;
		}
		else{
			if(resultsToBeReturned<numOfLines){
				resultsToBeReturned++;
			}
		}
	}
}

int findsize(int neg_or_pos,int rank_size,int id_size){

	int argsize=16;
	if(neg_or_pos<0.0){
		argsize++;
	}
	int temp,counter=-1;
	temp=rank_size;
	while(temp){
		temp/=10;
		counter++;
	}
	argsize+=counter;
	temp=id_size;
	if(id_size!=0)
		counter=-1;
	else
		counter=0;
	while(temp){
		temp/=10;
		counter++;
	}
	argsize+=counter;

	return argsize;
}

void align(char *content,char **input,int index,int argsize){

	char *array=malloc(argsize*sizeof(char)+1);
	int i,j;
	for(i=0;i<argsize;i++){
		array[i]=' ';
	}
	array[argsize]='\0';
	printf("%s",array);
	free(array);
	///////////////////////////////////////////////////
	i=0;
	int chcounter=0;
	do{
		while(content[chcounter]!=' ' && content[chcounter]!='\0'){
			chcounter++;
		}
		char *word=malloc(chcounter*sizeof(char)+1);
		word[chcounter]='\0';
		strncpy(word,content,chcounter);
		content+=chcounter;

		int found=0;
		for(j=0;j<index;j++){
			if(strcmp(word,input[j])==0){
				int k=0;
				char *array=malloc(strlen(word)*sizeof(char)+1);
				while(k<strlen(word)){
					array[k++]='^';
				}
				array[k]='\0';
				printf("%s",array);
				free(array);
				found=1;
				break;
			}
		}
		if(found==0){
			int k=0;
			char *array=malloc(strlen(word)*sizeof(char)+1);
			while(k<strlen(word)){
				array[k++]=' ';
			}
			array[k]='\0';
			printf("%s",array);
			free(array);
		}
		if(content[0]==' '){
			printf(" ");
			content++;
		}
		chcounter=0;
		free(word);
	}while(content[0]!='\0');
	printf("\n");
}

void bubbleSort(int *id_sorting,double *QScore,int *flag_arr,int numOfLines){

	int i,j,swap2,swap3;
	double swap1;
	for(i=0;i<numOfLines-1;i++){
		for(j=0;j<numOfLines-i-1;j++){
			if(QScore[j]<QScore[j+1]){
				swap1=QScore[j];
				swap2=id_sorting[j];
				swap3=flag_arr[j];
				QScore[j]=QScore[j+1];
				id_sorting[j]=id_sorting[j+1];
				flag_arr[j]=flag_arr[j+1];
				QScore[j+1]=swap1;
				id_sorting[j+1]=swap2;
				flag_arr[j+1]=swap3;
			}
		}
	}
}
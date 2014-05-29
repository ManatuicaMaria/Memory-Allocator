/* Manatuica Maria 313CA */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Antet functii */
void dump( unsigned char *arena, int N);	//Functie pentru comanda DUMP

int alloc( unsigned char *arena, int N,		//Functia aloca in arena un bloc
			int size);						//de size elemente

void my_free( unsigned char *arena,			//Functia elibereaza blocul de memorie
				int index);					//cu adresa de inceput index

void fill( unsigned char *arena, int N);	//Functie pentru comanda FILL

void showfree( unsigned char *arena, int N);//Functie pentru comanda SHOW FREE

void showusage( unsigned char *arena, int N);//Functie pentru comanda SHOW USAGE

void showallocations( unsigned char * arena, //Functie pentru comanda  
						int N);				 // SHOW ALLOCATIONS

int allocaligned( unsigned char *arena, 	//Functie pentru comanda ALLOCALIGNED
					int N);

int my_realloc( unsigned char *arena,		//Functie pentru comanda REALLOC
				 int N);

void arenabusy(	unsigned char *arena,		//Functia determina bitii ocupati
				int N,	char *busy);		//din arena, prin intermediului 
											//vectorului busy, care ia valoarea
											// 1 daca bitul este ocupat si 0
											// in caz contrar
												
void showmap( unsigned char *arena, int N); //Functie pentru comanda SHOW MAP
	
/* Functia main */

int main(){
	unsigned char *arena;				//Vectorul ce reprezinta arena
	char com[12],info[12];				
	int N,index,size;
	scanf("%s %d",com,&N);				//Citesc comanda INITIALIZE 
										//si numarul de octeti ai arenei
	arena=(unsigned char*)calloc(N,sizeof(unsigned char));
										//Initializez bitii arenei cu 0
	scanf("%s",com);					//Citesc urmatoarea comanda
	while(strcmp(com,"FINALIZE") != 0){	
		if(strcmp(com,"DUMP") == 0){		
			dump(arena,N);				//Execut comenzile pana cand se introduce 
		}								//comanda FINALIZE
		if(strcmp(com,"ALLOC") == 0){
			scanf("%d",&size);
			printf("%d\n",alloc(arena,N,size));
		}
		if(strcmp(com,"FREE") == 0){
			scanf("%d",&index);
			my_free(arena,index);
		}
		if(strcmp(com,"FILL") == 0){
			fill(arena,N);
		}
		if(strcmp(com,"SHOW") == 0){
			scanf("%s",info);
			if(strcmp(info,"FREE") == 0){
				showfree(arena,N);
			}
			if(strcmp(info,"USAGE") == 0){
				showusage(arena,N);
			}
			if(strcmp(info,"ALLOCATIONS") == 0){
				showallocations(arena,N);
			}
			if(strcmp(info,"MAP") == 0){
				showmap(arena,N);
			}
		}
		if(strcmp(com,"ALLOCALIGNED") == 0){
			printf("%d\n",allocaligned(arena,N));
		}
		if(strcmp(com,"REALLOC") == 0){
			printf("%d\n",my_realloc(arena,N));
		}
		scanf("%s",com);
	}
	
	free(arena);						//La primirea comenzii FINALIZE
	return 0;							// eliberez memoria utilizata
}

/*Definirea functiilor */
void dump(unsigned char *arena,int N){
	int idx;							//Cu idx(index) parcurg vectorul arena
	idx=0;								//si afisez harta memoriei conform cerintelor
	while(idx < N){
		if(idx%16==0){
			printf("%08X\t %02X",idx,arena[idx]);
		}else{
			if((idx%8==0)&&(idx!=0)){
				printf(" ");
			}
			printf(" %02X",arena[idx]);
			if((idx%16==15) || (idx==N-1)){
				printf("\n");
			}
		}
		idx++;
	}
	printf("%08X\n",N);
}

int alloc( unsigned char *arena, int N, int size){	
	int i=0;
	int *start,*next,*blsz,*actnext,*actprev,*nextpr;
	size+=12;
	start=(int *)&arena[0];				//Intai verific daca arena este goala
	if(*start == 0){					
		if( N-4 >= size){
			i=16;						//In caz afirmativ, aloc blocul de size 
			actnext=(int *)&arena[4];	//octeti incepand cu indexul 16
			*actnext=0;					//daca este loc sufiecient in arena
			actprev=(int *)&arena[8];	//si fac inlocuirile aferente
			*actprev=0;					//in sectiunile de gestiune
			blsz=(int *)&arena[12];
			*blsz=size;
			*start=4;
		}
	}else{
		if(*start-4 >= size){
			actnext=(int *)&arena[4];	//Daca arena are cel putin un bloc de date
			*actnext=*start;			//caut in restul arenei spatiu pentru
			actprev=(int *)&arena[8];	//blocul de size octeti
			*actprev=0;					//Prima data verific sectiunea de octeti 
			blsz=(int *)&arena[12];		//dintre indicele de start si primul bloc
			*blsz=size;					
			nextpr=(int *)&arena[*start+4];
			*nextpr=4;
			*start=4;
			i=16;
		}else{
			next=(int *)&arena[*start];		//Daca nu este suficient de mare
			blsz=(int *)&arena[*start+8];	//caut in restul arenei
			while((*next-*start-*blsz < size)// in functie de indicii de next
						 && (*next !=0)){	 // si ma opresc atunci cand am gasit
				start=next;					 // suficienti octeti liberi sau atunci
				next=(int *)&arena[*start];	 // cand am ajuns la ultimul bloc de date
				blsz=(int *)&arena[*start+8];
			}
			if(*next-*start-*blsz >= size){
				i=*start+*blsz;
				actnext=(int *)&arena[i];
				*actnext=*next;
				actprev=(int *)&arena[i+4];		//Verific de ce s-a sfarsit cautarea
				*actprev=*start;				//si fac inlocuirile aferente
				blsz=(int *)&arena[i+8];		//daca a avut succes
				*blsz=size;
				nextpr=(int *)&arena[*next+4];
				*nextpr=i;
				*next=i;
				i=i+12;
			}else{
				if(N-*start-*blsz >= size ){
					i=*start+*blsz;
					actnext=(int *)&arena[i];
					*actnext=0;
					actprev=(int *)&arena[i+4];
					*actprev=*start;
					blsz=(int *)&arena[i+8];	
					*blsz=size;
					*next=i;
					i=i+12;
				}
			}
		}
	}
	return i;	// Returnez indicele de start al blocului sau 0,daca nu s-a reusit
				// alocarea
}

void my_free( unsigned char *arena, int index){
	int i,*next,*prev,*nextpr,*prevnext;
	i=index-12;						//Eliberez memoria prin modificarea in 
	next=(int *)&arena[i];			//zonele de gestiune a blocului precedent(sau  
	prev=(int *)&arena[i+4];		//indicele de start), si a blocului urmator(daca
	nextpr=(int *)&arena[*prev];	// exista).
	prevnext=(int *)&arena[*next+4];
	if (*next != 0){
		*prevnext=*prev;
	}
	*nextpr=*next;
}

void fill(unsigned char *arena,int N){
	int index,size,value,i;					//Incepand cu octetul de pe pozitia 
	scanf("%d %d %d",&index,&size,&value);	//"index" din arena atribui valoarea  
	for( i=index; i<index+size; i++){		//"value" unui nr. de "size" octeti
		arena[i]=value;
	}
}

void showfree( unsigned char *arena, int N){
	int nrbl=0, nrby=0;
	int *start, *next, *size;				
	start=(int *)&arena[0];
	nrby=*start!=0?*start-4:0;				//Incepand cu indicele de start al arenei
	nrbl=nrby!=0?1:0;						//verific daca exista octeti liberi intre
	size=(int *)&arena[*start+8];			//blocurile de date vecine,respectiv
	next=start;								//intre indicele de start si primul bloc
	start=(int *)&arena[*start];			// si dupa ultimul bloc si ii numar.
	while(*start != 0){						//Daca in arena nu sunt blocuri de date 
		if( *start-*next-*size > 0 ){		//atunci numarul de octeti liberi va fi
			nrby+=(*start-*next-*size);		// egal cu numarul de octeti ai arenei 
			nrbl+=1;						// - 4(indicele de start),iar numarul
		}									// de blocuri libere egal cu 1.
		next=start;
		size=(int *)&arena[*start+8];		
		start=(int *)&arena[*start];
	}
	start=(int *)&arena[*next+8];
	nrby+=(N-*next-*start);
	if(N-*next-*start != 0){
		nrbl+=1;
	}
	start=(int *)&arena[0];
	if(*start == 0){
		nrby-=4;
	}
	printf("%d blocks (%d bytes) free\n",nrbl,nrby);
} 

void showusage( unsigned char *arena, int N){
	int nrblf=0, nrbyf=0, nrblu=0, nrbyu=0;
	int eff, fragm;
	int *start, *next, *size;
	start=(int *)&arena[0];
	nrblu=*start!=0?1:0;				//Determin cati octeti sunt liberi,respectiv
	nrbyf=*start!=0?*start-4:0;			//cate blocuri de octeti, folosind acelasi
	nrblf=nrbyf!=0?1:0;					//algoritm ca la functia showfree.In paralel
	size=(int *)&arena[*start+8];		//calculez blocurile ocupate.
	next=start;
	start=(int *)&arena[*start];		//Dupa care din totalul de octeti ii scad pe
	while(*start != 0){					//cei liberi si pe cei rezervati pentru 
		if( *start-*next-*size > 0 ){	// gestiune si aflu cati sunt ocupati.
			nrbyf+=(*start-*next-*size);
			nrblf+=1;
		}
		nrblu++;
		next=start;
		size=(int *)&arena[*start+8];
		start=(int *)&arena[*start];
	}
	start=(int *)&arena[*next+8];
	nrbyf+=(N-*next-*start);
	if(N-*next-*start != 0){
		nrblf+=1;
	}
	start=(int *)&arena[0];
	if(*start == 0){
		nrbyf-=4;
	}
	nrbyu=N-nrbyf-(nrblu*12)-4;			//In functie de datele calculate determin
	printf("%d blocks (%d bytes) used\n",nrblu,nrbyu);	//eficienta utilizarii si
	eff=(100*nrbyu)/(nrbyu+4+(nrblu*12));				//fragmentarea arenei.
	fragm=nrblu==0?0:(100*(nrblf-1))/nrblu;
	printf("%d%c efficiency\n%d%c fragmentation\n",eff,'%',fragm,'%');
}

void showallocations( unsigned char * arena, int N){
	int *start, *next, *blsz;
	printf("OCCUPIED 4 bytes\n");		//Primii 4 octeti sunt ocupati de indicele
	start=(int *)&arena[0];				//de start
	if(*start == 0){
		printf("FREE %d bytes\n",N-4);	//Daca indicele de start este 0 rezulta ca 
	}else{								//restul de N-4 octeti sunt liberi
		if(*start-4 > 0){				//Daca este diferit de 0, parcurg arena pana
			printf("FREE %d bytes\n",*start-4);	// la ultimul bloc de date
		}										
		blsz=(int *)&arena[*start+8];		//Pentru fiecare bloc de date existent
		printf("OCCUPIED %d bytes\n",*blsz);//sunt "blsz"(blocksize) octeti ocupati
		next=(int *)&arena[*start];			//Pentru octetii liberi, pe masura ce 
		while(*next != 0){					//parcurg blocurile din arena verific
			if(*next-*start-*blsz > 0){		//cati octeti sunt liberi intre 2 blocuri
				printf("FREE %d bytes\n",*next-*start-*blsz);	//vecine, respectiv
			}								// intre indicele de start si primul bloc
			start=next;						// si intre ultimul bloc de date si 
			blsz=(int *)&arena[*start+8];	//ultimul octet.
			printf("OCCUPIED %d bytes\n",*blsz);
			next=(int *)&arena[*start];		
		}
		if(N-*start-*blsz > 0){
			printf("FREE %d bytes\n",N-*start-*blsz);
		}
	}
}

int allocaligned( unsigned char *arena, int N){
	int size, align, index;			//Functia functioneaza asemanator functiei alloc
	int i=0;						
	int *start, *next, *prev, 
		*blsz, *actnext, *actprev;
	scanf("%d %d",&size,&align);	//Daca arena este goala, caut primul indice care 
	size+=12;						//indeplineste conditia(i%align == 0) si verific
	start=(int *)&arena[0];			// daca este spatiu suficient pt blocul cerut
	if(*start == 0){				// si il aloc in caz afirmativ
		if(N-4 >= size){
			i=16;
			while(i%align != 0){ i++; }
			if(N-i >= size-12){
				actnext=(int *)&arena[i-12];
				*actnext=0;
				actprev=(int *)&arena[i-8];
				*actprev=0;
				blsz=(int *)&arena[i-4];
				*blsz=size;
				*start=i-12;
			}else{
				i=0;
			}
		}
	}else{
		index=16;					//Daca arena nu este goala pentru fiecare bloc
		next=start;					//de octeti liberi gasit verific daca exista
		prev=(int *)&arena[*next+4];//un indice corespunzator intre acei octeti 
		while(*next != 0){			// si daca este suficient loc pentru noul bloc
			while(index%align != 0){//Aceasta cautare are loc pana se ajunge la 
				index++;			//ultimul bloc de date din arena
			}						
			if(*next-index >= size-12){
				actnext=(int *)&arena[index-12];
				*actnext=*next;
				actprev=(int *)&arena[index-8];
				*actprev=*prev;
				blsz=(int *)&arena[index-4];
				*blsz=size;
				*next=index-12;
				*prev=index-12;
				i=index;
				break;
			}
			start=next;
			blsz=(int *)&arena[*next+8];
			next=(int *)&arena[*next];
			prev=(int *)&arena[*next+4];
			index=*start+*blsz+12;
		}
		if(*next == 0){				//Daca a luat sfarsit cautarea fara un rezultat
			while(index%align != 0){// verific daca este posibila alocarea intre 
				index++;			// ultimul bloc de date existent si ultimul octet
			}
			if(N-index >= size-12){
				actnext=(int *)&arena[index-12];
				*actnext=*next;
				actprev=(int *)&arena[index-8];
				*actprev=*start;
				blsz=(int *)&arena[index-4];
				*blsz=size;
				*next=index-12;
				i=index;
			}
		}
	}
	return i;					//Daca nu a fost gasit un indice corespunzator
}								//functia returneaza 0.(i este initializat cu 0)

int my_realloc( unsigned char *arena, int N){
	int index, size, size1, *blsz, i;
	unsigned char copy[N];
	scanf("%d %d",&index,&size);
	blsz=(int *)&arena[index-4];	//Retin valoarea initiala a marimii blocului
	size1=*blsz-12;
	for( i=0; i<size1; i++){		//Creez un vector copie pentru datele din bloc
		copy[i]=arena[i+index];
	}
	my_free(arena,index);			//Eliberez memoria de la adresa initiala 
	index=alloc(arena,N,size);		//Aloc blocul pentru noul size si retin indexul
	if(size1 > size){				//intors de functia alloc.
		size1=size;					//Daca size-ul initial este mai mic decat size-ul
	}								//cerut, copiem doar size(cel nou) octeti din
	for( i=0; i<size1; i++){		//bloc
		arena[i+index]=copy[i];	
	}
	return index;				//Functia intoarce noul index.
}

void arenabusy( unsigned char *arena, int N, char *busy){
	int i,*start,*blsz;
	for( i=0; i<4; i++){
		busy[i]=1;				//Vectorul busy are lungimea N si determina pentru 
	}							//fiecare octet din arena daca este ocupat sau nu	
	start=(int *)&arena[0];
	blsz=(int *)&arena[*start+8];	//Daca octetul i este ocupat busy[i]==1, altfel
	while(*start != 0){				//busy[i]==0
		for( i=*start; i<*start+*blsz; i++){
			busy[i]=1;
		}
		start=(int *)&arena[*start];
		blsz=(int *)&arena[*start+8];
	}
}
	
void showmap( unsigned char *arena, int N){
	int length,poz,j,ok,i;	//x reprezinta numarul de octeti care intra in zona
	double x,y,d;			//unui caracter
	char *busy;				//d este limita superioara a zonei unui caracter
	scanf("%d",&length);	//iar y este limita inferioara
	busy=(char *)calloc(N,sizeof(char));
	arenabusy(arena,N,busy);
	x=(double)N/length;		 
	d=x;y=0.0;				
	for( i=0; i<length; i++){    //i este contorul pentru caractere
		if( (i != 0) && (i%80 == 0) ){ // daca s-au afisat 80 de caractere se trece
			printf("\n");				//la o noua linie
		}
		ok=0;		// Initial presupunem ca in zona caracterului toti octetii sunt
					//si caut cazul contrar
		if(floor(y) == floor(d)){
			poz=floor(y);		//Daca valoarea intreaga a limitei inferioare este
			printf("%c",busy[poz]==1?'*':'.');
			y=d;				//egala cu valoarea intreaga a limitei superioare
			d=d+x;				//inseamna ca in zona caracterului se afla doar
			continue;			//un octet care este verificat si se trece la 
		}						// urmatorul octet
		
		if(floor(d) == d){		//Daca limita superioara este un numar intreg,fara 
			for( j=floor(y); j<floor(d); j++){	//zecimale octetul determinat de 
				if(busy[j] == 1){	//aceasta limita nu intra in zona caracterului
					ok=1;
				}
			}
		}else{					//In caz contrar, si acest octet intra in zona 
			for( j=floor(y); j<=floor(d); j++){	//caracterului si este verificat si
				if(busy[j] == 1){	//acesta
					ok=1;
				}
			}
		}
		printf("%c",ok==0?'.':'*');	//Dupa ce octetii din zona descrisa au fost
		y=d;				// verificati se afiseaza caracterul corespunzator
		d=d+x;
	}
	printf("\n");
	free(busy);
}

Manatuica Maria 313CA

	Arena este reprezentata de vectorul de N  octeti de tip unsigned char.
	Pentru lucrul cu sectiunile de gestiune din arena, am folosit pointeri de tip int la fiecare din indicii specificati.
	
	Comanda data de utilizator o citesc in variabila sir de caractere "com" iar celelalte informatii (ex: <N> de la ALLOC, <info> de la SHOW, etc.) in alte variabile.
	
	Prima comanda o consider INITIALIZE <N>, iar in urma acestei comenzi aloc pentru vectorul arena un spatiu de memorie de N octeti, si ii initializez cu 0, folosind functia calloc. Citesc urmatoarea comanda si execut comenzile intr-un ciclu pana cand 
se introduce comanda FINALIZE, la introducere careia eliberez memoria folosita cu ajutorul lui free. In afara de comenzile INITIALIZE si FINALIZE am atribuit fiecarei comenzi o anumita functie, cu un nume sugestiv.

	Pentru comanda DUMP functia dump afiseaza harta memoriei intocmai cerintelor din enunt.
	
	Pentru comanda ALLOC, citesc parametrul <SIZE> introdus de la tastatura, iar apoi apelez functia alloc cu parametrii corespunzatori. Functia cauta in arena o sectiune de memorie mai mare sau egala cu (size+12) octeti si daca o gaseste face inlocuirile aferente in sectiunea de gestiune a blocului precedent( sau indice de start), a blocului curent si a blocului urmator( daca exista) si intoarce adresa de inceput a sectiunii de date a utilizatorului sau 0 daca nu s-a gasit o astfel de zona.
	
	Pentru comanda FREE, citesc index-ul si apelez functia my_free cu parametrii aferenti, printre care si index. Functia elibereaza memoria de la adresa index astfel:modifica datele in sectiunile de gestiune vecine blocului care trebuie eliberat.Blocul anterior(sau indicele de start) primeste ca adresa pentru blocul urmator adresa blocului succesor al celui actual,iar blocul succesor(daca exista) primeste ca adresa de bloc precedent blocul anterior celui actual.
	
	Pentru comanda SHOW, citesc info, iar in functie de acest parametru apelez una din functiile showfree,showusage,showallocations sau showmap. Primele trei functioneaza pe baza aceluiasi principiu. Parcurg sectiunile de gestiune din arena si calculeaza datele cerute. 
	
	Functia showmap afiseaza informatiile cerute folosindu-se de un vector suplimentar, vectorul busy de N octeti de tip char, care determina daca un vector i este liber sau nu. Vectorul busy este construit cu ajutorul functiei arenabusy.
	
	Pentru comanda ALLOCALIGNED, se apeleza functia allocaligned, care citeste datele necesare(size si align) si functioneaza asemanator cu functia alloc, cu deosebirea ca aceasta verifica pentru fiecare zona libera intalnita daca exista posibilitatea ca blocul sa fie alocat la un index "aliniat". Functia intoarce valoare indexului, daca s-a gasit o zona corespunzatoare sau 0 in  caz contrar.
	
	Pentru comanda REALLOC, se apeleaza functia my_realloc care citeste de la tastatura index si size. Functia retine continutul blocului care trebuie realocat intr-un vector copy, elibereaza memoria de la index prin apelul functiei my_free cu parametrul index, apoi aloca blocul de size prin apelul functiei alloc, si incepand cu indicele returna de aceasta din urma copiaza nr(size-ul initial al blocului daca noul size este mai mare sau egal cu el, sau noul size in caz contrar) octeti din vechiul bloc, cu ajutorul vectorului copie. Functia intoarce noul index.

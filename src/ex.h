
extern int rilancia(int argc, char *argv[]);

/**
 * Scrivere il programma nonest_symlink che ha come parametro il pathname di una directory.
 * Il programma deve cercare tutti i link simbolici presenti nella directory e cancelli ogni link simbolico
 * nidificato (link simbolico che punta a link simbolico).
 */
extern int nonest_symlink(int argc, char *argv[]);

/**
 * Scrivere il programma abssymlink che ha come parametro il pathname di una directory.
 * Il programma deve cercare tutti i link simbolici presenti nella directory e trasformare ogni link
 * simbolico in uno equivalente che punti al pathname assoluto e non relativo..
 */
extern int abssymlink(int argc, char **argv);

/**
 * Scrivere un programma searchlink che dati due parametri (nell'ordine un file f ed una directory d)
 metta in output l'elenco dei path all'interno dell'albero che ha radice in d che fanno riferimento ad f
 o come link fisico o come link simbolico.
es:
searchlink myfile mydir
link a
link d/b
symlink e/s
significa che dir/a e dir/d/b si riferisono a f come link fisici mentre dir/e/s e' un link simbolico che punta
ad f.
*/
extern int searchlink(int argc, char **argv);

/**
 * Fare un programma di confronto parallelo di file.
 pcmp file1 file2
 non stampa nulla se i due file sono uguali altrimenti stampa "file1 file2 differ". pcmp deve creare due processi figli;
 mentre un processo controlia la prima meta’ del file, l'altro deve controllare l’altra meta’.
 Se un dei due processi figli trova una differenza, termina immediatamente segnalandolo al padre con opportuno
 valore di ritorno. In questo caso il padre deve terminare l'altro processo.

*/
extern int pcmp(int argc, char **argv);


/**
 * 
 */
extern int dircat(int argc, char **argv);


/**
 * 
 */
extern int fifotext(int argc, char **argv);


/**
 * 
 */
extern int stdin2pipe(int argc, char **argv);


/**
 * 
 */
extern int tree(int argc, char **argv);


/**
 * this is a test
 */
extern int test2(int argc, char **argv);


/**
 * 
 */
extern int tx(int argc, char **argv);


/**
 * 
 */
extern int rx(int argc, char **argv);


/**
 * 
 */
extern int lancia(int argc, char **argv);


/**
 * 
 */
extern int pcp(int argc, char **argv);


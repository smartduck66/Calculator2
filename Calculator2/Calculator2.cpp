//
// Book : Version du chapitre 10 de l'ouvrage.
// "Programming -- Principles and Practice Using C++" de Bjarne Stroustrup (2ème édition : 2014)
// Commit initial : 08/02/2018 
// Commit en cours : Exercices n°6,7,10 des pages 377 et 378 => Extension "Chiffres Romains" de la calculatrice étudiée au chapitre 7
// Caractères spéciaux : [ ]   '\n'   {  }   ||   ~   _     @

// Librairie indispensable sous Windows et VC++
#include "stdafx.h"


// Librairie restreinte fournie par BS
#include "std_lib_facilities.h"


// Définition de constantes symboliques pour clarifier le code
// -------------------------------------------------------------------------------------------------------------------------------------------
constexpr char number = '8';                  // "a value" dans la classe Token
constexpr char romain = 'X';				  // Traitement des chiffres romains saisis	

const char quit = 'q';                    // Quitter
const string exitkey = "exit";            // Exit Keyword, identique dans son comportement à "Quitter"

const char print = ';';                   // "Imprimer" le résultat

const string prompt = "> ";               // Demande d'une saisie
const string result = "= ";               // Indique que ce qui suit est un résultat

constexpr char name = 'a';                // name : token

constexpr char let = 'L';                 // déclaration/initialisation d'une variable : token
const string declkey = "let";             // déclaration/initialisation d'une variable : keyword

constexpr char from = 'F';                 // lecture d'une commande dans un fichier externe : token
const string fromkey = "from";            // lecture d'une commande dans un fichier externe : keyword

constexpr char const_let = 'C';               // déclaration/initialisation d'une constante : token
const string const_letkey = "const_let";  // déclaration/initialisation d'une constante : keyword

constexpr char assign = 'S';                  // assignation d'une variable : token (modification de la valeur d'une variable déjà créée par let)
const string assignkey = "set";           // assignation d'une variable : keyword

constexpr char list_var = 'v';                // Lister les variables à l'écran : token
const string list_varkey = "lv";          // Lister les variables à l'écran : keyword

constexpr char del_var = 'd';                 // Supprimer la table de variables : token
const string del_varkey = "delete";       // Supprimer la table de variables : keyword

constexpr char racine = 'R';                  // racine carrée token
const string sqrtkey = "sqrt";            // racine carrée keyword

constexpr char puissance = 'P';               // puissance token
const string puissancekey = "pow";        // puissance keyword

constexpr char help = 'H';                    // Imprimer l'aide de la calculette : token
const string helpkey = "help";            // Imprimer l'aide de la calculette : keyword

class Roman_int {							// Ajout à calculator v1 : classe gérant les nombres romains
public:
	enum Roman_Numerals {
		I=1,V,X,L,C,D,M
	};
	// Constructeur par défaut
	Roman_int() = default;

	// Constructeur : la valeur entière (=arabique) se déduit à partir du nombre romain
	Roman_int(Roman_Numerals l) :roman_letter(l) 
	{  
	
		switch (l)
		{
		case Roman_int::I:
			roman_value = 1;
			break;
		case Roman_int::V:
			roman_value = 5;
			break;
		case Roman_int::X:
			roman_value = 10;
			break;
		case Roman_int::L:
			roman_value = 50;
			break;
		case Roman_int::C:
			roman_value = 100;
			break;
		case Roman_int::D:
			roman_value = 500;
			break;
		case Roman_int::M:
			roman_value = 1000;
			break;
		default:
			throw Roman_int::Invalid();
			break;
		}
		
	}

	class Invalid { };                 // to throw as exception

									   // non-modifying operations
	int as_int() const noexcept { return roman_value; }
	

	// modifying operations
	// none

private:							// Les variables privées sont initialisées par défaut : CPP CoreGuidelines
	Roman_Numerals roman_letter = I;
	int roman_value =1;

};
										  
Roman_int setValue_Romain(char valeur)	// Retourne une valeur de l'énumération Roman_int en fonction du caractère saisi
{  
	switch (valeur)
	{
	case 'I':
		return Roman_int::I;
		
	case 'V':
		return Roman_int::V;
		
	case 'X':
		return Roman_int::X;
		
	case 'L':
		return Roman_int::L;
		
	case 'C':
		return Roman_int::C;
		
	case 'D':
		return Roman_int::D;
		
	case 'M':
		return Roman_int::M;
		
	default:
		throw Roman_int::Invalid();
		
	}

}									  
										  
										  //----------------------------------------------------------------------------------------------------------------------------------------------
class Variable {            // Cette classe sert de socle à la gestion des variables & constantes dans la calculette (ex : let x=2; ou set x=4;)
public:
	string name;
	double value;
	bool constante;         // True : cette variable est une constante et ne peut être modifiée par une assignation ultérieure

	// Constructeur
	Variable(string n, double v, bool c) :name(n), value(v), constante(c) { }

};

class Symbol_table {        // Cette classe encapsule la classe Variable ainsi que ses fonctions associées (exo 4, p 253)
public:                     // Question : aucun constructeur défini implicitement. Problème ?
							// fonctions-membre
	double get_value(string s);
	double set_value(string s, double d);
	bool is_declared(string var);
	double define_name(string var, double val, bool isConstante);
	void print_value();
	void effacer();

	// Constructeur
	Symbol_table() = default;
	
private:
	//Seules les fonctions-membre peuvent manipuler les données
	vector<Variable>var_table{};

};

// ------------------------------------------------------------------------------------
// Définition des fonctions-membre de la classe Variable
// ------------------------------------------------------------------------------------

void Symbol_table::effacer() {   // On efface toutes les valeurs du tableau de variables - Utilisation de la fonction clear() de la librairie standard
	var_table.clear();
}

// The get_value() member function returns the value of the Variable named s
double Symbol_table::get_value(string s) {

	// Tip : const Variable& v signifie que l'on passe un vecteur par référence (sans le copier) ; const empêche toute modification accidentelle de ce vecteur
	for (const Variable& v : var_table)
		if (v.name == s) return v.value;

	error("get: undefined variable ", s);
	return 1;
}

// The set_value() member function sets the Variable named s to d
double Symbol_table::set_value(string s, double d) {
	for (Variable& v : var_table)
		if (v.name == s) {
			if (v.constante)error("set: cette variable est une constante et ne peut être modifiée : ", s);

			// Sinon, on peut la modifier
			v.value = d;
			return 0;
		}
	error("set: undefined variable ", s);
	return 1;
}

bool Symbol_table::is_declared(string var)   // is var already in var_table ?
{
	for (const Variable& v : var_table)
		if (v.name == var) return true;
	return false;

}

double Symbol_table::define_name(string var, double val, bool isConstante)   // add (var,val, constante) to var_table
{
	if (is_declared(var)) error(var, " declared twice");

	if (isConstante)
		var_table.push_back(Variable{ var,val, true });   // On initialise une constante
	else
		var_table.push_back(Variable{ var,val, false });  // On initialise une variable modifiable par un éventuel set

	return val;

}

void Symbol_table::print_value()   // Imprime à l'écran la liste des variables déclarées
{
	// Aucune valeur ?
	if (var_table.size() == 0)error("set: aucune variable n'a encore été déclarée : ");

	cout << "Nombre de variables déclarées : " << var_table.size() << "\n";
	for (const Variable& v : var_table)
		cout << "Variable " << v.name << " = " << v.value << "\n";

}

//--------------------------------------------------------------------------------------------

Symbol_table sym; // Instanciation de la classe Symbol_table : fourniture des fonctions-membre

//--------------------------------------------------------------------------------------------


class Token {               // Cette classe sert de socle à la gestion des "Tokens"
public:
	char kind{};			// what kind of token
	double value=0.0;		// for numbers: a value
	string name{};			// pour le nom des variables

	// Constructeurs
	Token(char ch) noexcept:kind{ ch } {}                           //initialize kind with ch
	Token(char ch, double val) noexcept:kind{ ch }, value{ val } {}   //initialize kind and value
	Token(char ch, string n) :kind{ ch }, name{ n } {}       //initialize kind and name
	Token(char ch, Roman_int val) noexcept:kind{ ch }, value{ static_cast<double>(val.as_int()) } {}   //initialize kind and value : 'X' pour nombre romain saisi et on récupère sa valeur entière via le membre de l'objet
};

//----------------------------------------------------------------------------------

class Token_stream {
public:
	istream & channel_input;     // On définit le canal de saisie des informations
								 // istream est une classe de la librairie standard
	Token_stream();             // make a Token_stream that reads from channel_input (anciennement cin directement)
	Token get();                // get a Token (get() is defined elsewhere)
	void putback(Token t);      // put a Token back
	void ignore(char c);        // discard characters up to and including a char

private:
	bool full;        // is there a Token in the buffer?
	Token buffer;     // here is where we keep a Token put back using putback()
};

// The constructor just sets "full" to indicate that the buffer is empty and defines "cin" as the input channel
Token_stream::Token_stream()  :channel_input(cin), full(false), buffer(0) {}  // no Token in buffer


//-----------------------------------------------------------------------------------------------

Token_stream ts;        // Instanciation de la classe Token_stream : provides get() and putback()

// ------------------------------------------------------------------------------------
// Définition des 3 fonctions-membre de la classe Token_stream
// ------------------------------------------------------------------------------------
// The putback() member function puts its argument back into the Token_stream's buffer:
void Token_stream::putback(Token t)
{
	if (full) error("putback() into a full buffer");
	buffer = t;       // copy t to buffer
	full = true;      // buffer is now full
}

//------------------------------------------------------------------------------
void Token_stream::ignore(char c)   // c represents the kind of Token
{
	// first look in buffer
	if (full&&c == buffer.kind) {
		full = false;
		return;
	}
	full = false;

	// now search input
	char ch = 0;
	while (channel_input >> ch)       // anciennement cin était indiqué ici
		if (ch == c)return;
}

//------------------------------------------------------------------------------
Token Token_stream::get()
{
	if (full) {       // do we already have a Token ready?
					  // remove token from buffer
		full = false;
		return buffer;
	}

	char ch;
	channel_input >> ch;    // Saisie de l'expression : un CR stoppe le processus - Note that >> skips whitespace (space, newline, tab, etc.)
							// anciennement cin était indiqué ici

	switch (ch) {
	case print:
	case quit:
	case '(':
	case ')':
	case '{':
	case '}':
	case '+':
	case '-':
	case '*':
	case '/':
	case '!':
	case '%':
	case ',':                    // Traitement des paramètres de pow()
	case '=':
		return Token(ch);        // let each character represent itself
	case '.':
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
	{
		channel_input.putback(ch);              // put digit back into the input stream
												// anciennement cin était indiqué ici
		double val;
		channel_input >> val;                   // read a floating-point number
		return Token{ number,val };   // La constante number définie est utilisée
	}
	case 'I':
	case 'V': case 'X': case 'L': case 'C': case 'D': case 'M':	// Gestion des nombres romains saisis
	{
		const Roman_int val=setValue_Romain(ch);		// Crée un objet "chiffre romain"
		return Token{ romain,val };						// La constante romain définie est utilisée
	}
	
	default:
		if (isalpha(ch) || ch == '_') {
			string s;
			s += ch;
			while (channel_input.get(ch) && (isalpha(ch) || isdigit(ch) || ch == '_'))s += ch;    // On gère également le "_" dans les noms de variables (pré ou suffixe)
			channel_input.putback(ch);                                                  // anciennement cin était indiqué ici

			if (s == declkey)
				return Token{ let };          //déclaration initiale d'une variable
			if (s == const_letkey)
				return Token{ const_let };    //déclaration initiale d'une constante
			if (s == assignkey)
				return Token{ assign };       //assignation d'une variable (=modification) après sa déclaration initiale par un let
			if (s == list_varkey)
				return Token{ list_var };     //Impression de la liste des variables
			if (s == del_varkey)
				return Token{ del_var };     //Suppression de la liste des variables

			if (s == helpkey)
				return Token{ help };         //Aide

			if (s == sqrtkey)
				return Token{ racine };       //racine carrée keyword
			if (s == puissancekey)
				return Token{ puissance };    //puissance keyword

			if (s == fromkey)
				return Token{ from };			//lecture d'une commande à partir d'un fichier externe

			if (s == exitkey)
				return Token{ quit };         //Même comportement que le "case quit" plus haut

			return Token{ name,s };

		}

		error("Bad token !!");
		return 1;
	}
}




//-----------------------------------------------------------------------------------------------
// Tip : on passe ts par référence pour éviter de l'avoir en variable globale (chapitre 7)
double expression(Token_stream& ts);    // declaration so that primary() can call expression()
double term(Token_stream& ts);          // declaration so that primary() can call term()
										//-----------------------------------------------------------------------------------------------

										// deal with numbers, parentheses et accolades
double primary(Token_stream& ts)
{
	Token t = ts.get();
	switch (t.kind) {
	case '(':    // handle '(' expression ')'
	{
		const double d = expression(ts);
		t = ts.get();
		if (t.kind != ')') error("')' expected");
		return d;
	}

	case '{':    // handle '{' expression '}'
	{
		const double d = expression(ts);
		t = ts.get();
		if (t.kind != '}') error("'}' expected");
		return d;
	}

	case '-':            // Rajout de la gestion des nombres négatifs...
		return -primary(ts);

	case '+':            // ... et du "+" éventuellement rajouté devant le 1er nombre saisi
		return primary(ts);

	case number:            // we use '8' to represent a number
		return t.value;     // return the number's value

	case romain:            // Gestion des nombres romains
		return t.value;     // On retourne seulement la valeur

	case name:
		return sym.get_value(t.name);

	case racine:        // Traitement de la racine carrée
	{
		const double d = term(ts);  // On attend une expression (ex : 4 ou 4-2)
		if (d<0) error("Une racine carrée ne peut s'appliquer sur un nombre négatif !");
		return sqrt(d);
	}

	case puissance:        // Traitement de la puissance
	{
		const double d = term(ts); // On attend deux paramètres (ex : 2,3 correspond à 2^3 ; 2,-1 correspond à 2^-1)
		return d;
	}

	default:
		error("primary expected");
		return 1;
	}
}

//------------------------------------------------------------------------------

// deal with *, /, !, %, "," pour pow()
double term(Token_stream& ts)
{
	double left = primary(ts);
	Token t = ts.get();        // get the next token from token stream

	while (true) {
		switch (t.kind) {
		case '*':
			left *= primary(ts);
			t = ts.get();
			break;

		case '/':
		{
			const double d = primary(ts);
			if (d == 0) error("divide by zero");
			left /= d;
			t = ts.get();
			break;
		}

		case '%':
		{
			const double d = primary(ts);
			if (d == 0) error("%: divide by zero");
			left = fmod(left, d); // Appel de la librairie "float modulo" de la librairie cmath
			t = ts.get();
			break;
		}

		case ',':
		{
			const double d = primary(ts);
			left = pow(left, d); // Appel de la librairie "puissance" de la librairie cmath (puissances négatives acceptées)
			t = ts.get();
			break;
		}

		case '!':
		{
			// On calcule la factorielle via une boucle simple inverse sur la variable left qui contient l'entier à traiter (ex : 4!)
			// On ne sait pas le faire sur un réel, donc on convertit en int
			// Attention : pas d'erreur générée quand la factorielle est TROP GRANDE, à partir de 171! Bizarre...
			
			const int factorielle = static_cast<int>(left);
			double resultat_fact = factorielle;

			if (factorielle == 0)
				resultat_fact = 1;
			else
				for (int i = factorielle - 1; i >= 1; i--)resultat_fact *= i;

			left = resultat_fact;
			t = ts.get();
			break;
		}

		default:
			ts.putback(t);     // put t back into the token stream
			return left;
		}
	}
}

//------------------------------------------------------------------------------

// deal with +, -
double expression(Token_stream& ts)
{
	double left = term(ts);      // read and evaluate a Term
	Token t = ts.get();        // get the next token from token stream

	while (true) {
		switch (t.kind) {

		case '+':
			left += term(ts);    // evaluate Term and add
			t = ts.get();
			break;
		case '-':
			left -= term(ts);    // evaluate Term and subtract
			t = ts.get();
			break;
		default:
			ts.putback(t);     // put t back into the token stream
			return left;       // finally: no more + or -: return the answer
		}
	}
}

//------------------------------------------------------------------------------
void clean_up_mess()
{
	ts.ignore(print);
}


//------------------------------------------------------------------------------
double declaration(bool isConstante)        // Déclaration d'une variable (ex : let x=2;)
											// assume we have seen "let"
											// handle : name = expression
											// declare a variable called "name" with the initial value "expression"
{
	Token t = ts.get();
	if (t.kind != name) error("name expected in declaration");
	string var_name = t.name;

	Token t2 = ts.get();
	if (t2.kind != '=') error("= missing in declaration of ", var_name);

	const double d = expression(ts);
	sym.define_name(var_name, d, isConstante);
	return d;

}

//------------------------------------------------------------------------------
double assignation(Token_stream& ts)        // Assignation/modification d'une variable (ex : set x=2;)
											// assume we have seen "set"
											// handle : name = expression
											// update a variable called "name" with the initial value "expression"
{
	Token t = ts.get();
	if (t.kind != name) error("name expected in assignation");
	string var_name = t.name;

	Token t2 = ts.get();
	if (t2.kind != '=') error("= missing in assignation of ", var_name);

	const double d = expression(ts);
	sym.set_value(var_name, d);  // L'appel à "set_value" est fait directement sans créer un "define_name" qui vérifie la présence de la variable :
								 // en effet, ce test est déjà fait dans la fonction set_value
	return d;

}

//------------------------------------------------------------------------------
string file_read(Token_stream& ts)			// Lecture d'une commande dans un fichier externe
											// Seul le nom est passé sous la forme "from test" ; on rajoute automatiquement un .txt pour l'ouvrir sous Windows
{
	Token t = ts.get();
	string filename = t.name+ ".txt";

	// Ouverture en lecture du fichier filename
	ifstream ist{ filename };
	ist.exceptions(ist.exceptions() | ios_base::badbit);
	if (!ist)error("Impossible d'ouvrir le fichier ", filename);

	// On remplit la variable de retour 'commande' avec les lignes du fichier : en effet, la calculatrice peut exécuter plusieurs calculs séquentiellement (ex : 4*3;8-3;).
	string commande{};
	string buffer{};
	while (ist>>buffer)
		commande+=buffer;
	ist.close();	// Fermeture du fichier

	return commande;
}

//------------------------------------------------------------------------------
double statement(Token_stream& ts)    // handle expressions and declarations
{
	Token t = ts.get();
	switch (t.kind) {
	case let:                   // Déclaration d'une variable normale
		return declaration(false);

	case const_let:             // Déclaration d'une constante non modifiable
		return declaration(true);

	case assign:                // Assignation/Modification d'une variable déjà créée
		return assignation(ts);

	case from:                // lecture d'une commande dans un fichier externe
	{
							   
		string commande = file_read(ts);
		cout << "La commande contenue dans le fichier : "<<commande<<'\n';
		
		// On réinjecte la commande dans le flux en une suite de "tokens" (= char), en partant de la fin SINON 90*8 serait lu 8*09 !!
		for (int i= commande.size()-1;i>=0;--i)
			ts.channel_input.putback(commande[i]);
		return 0;
	}

	case list_var:              // Impression de la liste des variables
		sym.print_value();
		return 0;

	case help:              // Impression de l'aide
	{
		cout << "Aide de la calculette\n";
		cout << "- opérations courantes (chiffres arabiques ou romains, mélange possible) : *, -, /, +, %\n";
		cout << "- let, set, const_let, lv, delete pour les variables/constantes\n";
		cout << "- factorielle : !  puissance : pow(x,y)  racine carrée : sqrt(x)\n";
		cout << "- from 'filename' permet de lire des commandes placées dans un fichier texte\n";
		cout << "- PS : seuls I, V, X, L, C, D, M sont acceptés pour les chiffres romains\n";
		cout << "--------------------------------------------------------------------------------------\n";
		return 0;
	}

	case del_var:              // Suppression de la liste des variables
	{
		sym.effacer();
		return 0;
	}

	default:
		ts.putback(t);
		return expression(ts);
	}
}

//------------------------------------------------------------------------------
void calculate(Token_stream& ts)    // expression evaluation loop
{
	while (ts.channel_input) {      // anciennement cin était indiqué ici
		try {
			cout << prompt;
			Token t = ts.get();
			while (t.kind == print)t = ts.get();    // eat ';'
			if (t.kind == quit)return;
			ts.putback(t);
			cout << result << statement(ts) << '\n';

		}
		catch (exception& e) {
			cerr << "error: " << e.what() << '\n';
			clean_up_mess();
		}
	}
}

//------------------------------------------------------------------------------
int main()
try
{
	// A mettre dans la 1ère ligne du "main" pour afficher les caractères accentués sous la console Windows
	std::locale::global(std::locale("fr-FR"));
	
	//predefined names:
	sym.define_name("pi", 3.1415926535, true);
	sym.define_name("e", 2.7182818284, true);
	sym.define_name("k", 1000, true);

	cout << "Saisis une expression suivie d'un ; (q ou exit pour quitter, help pour l'aide) :\n";

	calculate(ts);
	
	return 0;

}

catch (Roman_int::Invalid&)
{
	cerr << "error: le chiffre romain est invalide ! \n";
	keep_window_open("q");	// For some Windows(tm) setup
	return 1;
}

catch (runtime_error& e) {
	cerr << "error: " << e.what() << '\n';
	keep_window_open("@");
	return 1;
}
catch (...) {
	cerr << "Oops: unknown exception!\n";
	return 2;
}





#include "DFA.h"
#include <iostream>


DFA::DFA() {
	//startingState = 0;
}

DFA::DFA(const DFA& other) {
	this->startingState = other.startingState;
	this->ALPHABET_SIZE = other.ALPHABET_SIZE;
	//this->transitions = other.transitions;
	this->stateSymbPos = other.stateSymbPos;
	this->finalStates = other.finalStates;
}


void DFA::init(const NFA& nfa){
    ALPHABET_SIZE = nfa.alphabetSize();

	map<DFASet, int> statesMap;
	startingState = 0;

	//transitions.push_back(vector<int>(ALPHABET_SIZE, NULL_STATE));
	//transitions.push_back(vector<int>(ALPHABET_SIZE, NULL_STATE));

	statesMap[DFASet(nfa.getStartingState())] = 0;
	statesMap[DFASet()] = NULL_STATE;

	const vector<vector<set<int> > >& nfaTransitions = nfa.getTransitions();
	const map<int, int>& nfaFinalStates = nfa.getFinalStates();
	const map<int, int>& nfaInsertionOrder = nfa.getInsertionOrder();

	int nfaStartingState = nfa.getStartingState();

	queue<DFASet> Q;
	Q.push(DFASet(nfaStartingState));

	while(!Q.empty()) {
		DFASet a = Q.front();
		int dfastate = statesMap[a];

		map<int, int> symb_pos;
		stateSymbPos[dfastate] = symb_pos;

		Q.pop();
		for(int symbol = 0; symbol < ALPHABET_SIZE; symbol++) {
			DFASet result;

			for(set<int>::iterator j = a.begin(); j != a.end(); j++) {
				result.insert(nfaTransitions[*j][symbol]);
			}

			int resultdfastate = NULL_STATE;
			if(statesMap.count(result)) {
				resultdfastate = statesMap[result];
			} else {
				resultdfastate = statesMap.size();
				statesMap[result] = resultdfastate;
				//transitions.push_back(vector<int>(ALPHABET_SIZE, NULL_STATE));
				Q.push(result);
			}

			//transitions[dfastate][symbol] = resultdfastate;

			if (resultdfastate != NULL_STATE) {
                symb_pos[symbol] = resultdfastate;
			    stateSymbPos[dfastate] = symb_pos;
			    regexSymbols.insert(symbol);
			}
		}
	}

	const int oo = 0x7fffffff;
	for(map<DFASet, int>::iterator i = statesMap.begin(); i != statesMap.end(); i++) {
		int bestId = -1;
		int bestIdOrder = oo;
		for(set<int>::iterator j = i->first.begin(); j != i->first.end(); j++) {
			if(nfaFinalStates.count(*j)) {
				int id = nfaFinalStates.at(*j);
				int idOrder = nfaInsertionOrder.at(id);
				if(idOrder < bestIdOrder) {
					bestIdOrder = idOrder;
					bestId = id;
				}
			}
		}
		if(bestIdOrder != oo) {
			finalStates[i->second] = bestId;
		}
	}
}

DFA::DFA(const NFA& nfa) {
    init(nfa);
}

DFA::DFA(const NFA& nfa, bool add_often_using_symbols_) {
    init(nfa);

    std::wstring basic_latin = L" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~␡";
    std::wstring latin_1_punctuation = L" ¡¢£¤¥¦§¨©ª«¬®¯°±²³´µ¶·¸¹º»¼½¾¿×÷";
    std::wstring kyryllic_often = L"ЁАБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдежзийклмнопрстуфхцчшщъыьэюяё";
    std::wstring control_character = L"\r\n\f\v\t";

    vector<std::wstring> often_using_symbols;
    often_using_symbols.push_back(basic_latin);
    often_using_symbols.push_back(latin_1_punctuation);
    often_using_symbols.push_back(kyryllic_often);
    often_using_symbols.push_back(control_character);

    if (add_often_using_symbols_){
        this->add_often_using_symbols(often_using_symbols);
    }
}

void DFA::add_often_using_symbols(const vector<std::wstring> often_using_symbols){
    for (int state=0; state < stateSymbPos.size(); state++)
      for (auto wstr:often_using_symbols)
        for (int symbol:wstr)
          if (stateSymbPos[state].count(symbol) == 0)
              stateSymbPos[state][symbol] = NULL_STATE;
}

//basic_latin = L"!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~␡"
//basic_latin_punctuation = L"!\"#$%&'()*+,-./:;<=>?@[\]^_`{|}~␡"

//latin_1 = L"¡¢£¤¥¦§¨©ª«¬®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷øùúûüýþÿ";
//latin_1_punctuation = L"¡¢£¤¥¦§¨©ª«¬®¯°±²³´µ¶·¸¹º»¼½¾¿×Ø÷ø";


//modificators = L"ʰʱʲʳʴʵʶʷʸʹʺʻʼʽʾʿˀˁ˂˃˄˅ˆˇˈˉˊˋˌˍˎˏːˑ˒˓˔˕˖˗˘˙˚˛˜˝˞˟ˠˡˢˣˤ˥˦˧˨˩˪˫ˬ˭ˮ˯˰˱˲˳˴˵˶˷˸˹˺˻˼˽˾˿";

//kyryllic = L"ЀЁЂЃЄЅІЇЈЉЊЋЌЍЎЏАБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдежзийклмнопрстуфхцчшщъыьэюяѐёђѓєѕіїјљњћќѝўџѠѡѢѣѤѥѦѧѨѩѪѫѬѭѮѯѰѱѲѳѴѵѶѷѸѹѺѻѼѽѾѿҀҁ҂҃҄҅҆҇҈҉ҊҋҌҍҎҏҐґҒғҔҕҖҗҘҙҚқҜҝҞҟҠҡҢңҤҥҦҧҨҩҪҫҬҭҮүҰұҲҳҴҵҶҷҸҹҺһҼҽҾҿӀӁӂӃӄӅӆӇӈӉӊӋӌӍӎӏӐӑӒӓӔӕӖӗӘәӚӛӜӝӞӟӠӡӢӣӤӥӦӧӨөӪӫӬӭӮӯӰӱӲӳӴӵӶӷӸӹӺӻӼӽӾ";


//punctuation = L"           ​‌‍‎‏‐‑‒–—―‖‗‘’‚‛“”„‟†‡•‣․‥…‧  ‪‫‬‭‮ ‰‱′″‴‵‶‷‸‹›※‼‽‾‿⁀⁁⁂⁃⁄⁅⁆⁇⁈⁉⁊⁋⁌⁍⁎⁏⁐⁑⁒⁓⁔⁕⁖⁗⁘⁙⁚⁛⁜⁝⁞ ⁠⁡⁢⁣⁤⁦⁧⁨⁩⁪⁫⁬⁭⁮⁯"

//matematical =           L"⁰ⁱ⁲⁳⁴⁵⁶⁷⁸⁹⁺⁻⁼⁽⁾ⁿ₀₁₂₃₄₅₆₇₈₉₊₋₌₍₎ₐₑₒₓₔₕₖₗₘₙₚₛₜ"
//matematical_operators = L"∀∁∂∃∄∅∆∇∈∉∊∋∌∍∎∏∐∑−∓∔∕∖∗∘∙√∛∜∝∞∟∠∡∢∣∤∥∦∧∨∩∪∫∬∭∮∯∰∱∲∳∴∵∶∷∸∹∺∻∼∽∾∿≀≁≂≃≄≅≆≇≈≉≊≋≌≍≎≏≐≑≒≓≔≕≖≗≘≙≚≛≜≝≞≟≠≡≢≣≤≥≦≧≨≩≪≫≬≭≮≯≰≱≲≳≴≵≶≷≸≹≺≻≼≽≾≿⊀⊁⊂⊃⊄⊅⊆⊇⊈⊉⊊⊋⊌⊍⊎⊏⊐⊑⊒⊓⊔⊕⊖⊗⊘⊙⊚⊛⊜⊝⊞⊟⊠⊡⊢⊣⊤⊥⊦⊧⊨⊩⊪⊫⊬⊭⊮⊯⊰⊱⊲⊳⊴⊵⊶⊷⊸⊹⊺⊻⊼⊽⊾⊿⋀⋁⋂⋃⋄⋅⋆⋇⋈⋉⋊⋋⋌⋍⋎⋏⋐⋑⋒⋓⋔⋕⋖⋗⋘⋙⋚⋛⋜⋝⋞⋟⋠⋡⋢⋣⋤⋥⋦⋧⋨⋩⋪⋫⋬⋭⋮⋯⋰⋱⋲⋳⋴⋵⋶⋷⋸⋹⋺⋻⋼⋽⋾⋿"
//matematical_operators_2"⨀⨁⨂⨃⨄⨅⨆⨇⨈⨉⨊⨋⨌⨍⨎⨏⨐⨑⨒⨓⨔⨕⨖⨗⨘⨙⨚⨛⨜⨝⨞⨟⨠⨡⨢⨣⨤⨥⨦⨧⨨⨩⨪⨫⨬⨭⨮⨯⨰⨱⨲⨳⨴⨵⨶⨷⨸⨹⨺⨻⨼⨽⨾⨿⩀⩁⩂⩃⩄⩅⩆⩇⩈⩉⩊⩋⩌⩍⩎⩏⩐⩑⩒⩓⩔⩕⩖⩗⩘⩙⩚⩛⩜⩝⩞⩟⩠⩡⩢⩣⩤⩥⩦⩧⩨⩩⩪⩫⩬⩭⩮⩯⩰⩱⩲⩳⩴⩵⩶⩷⩸⩹⩺⩻⩼⩽⩾⩿⪀⪁⪂⪃⪄⪅⪆⪇⪈⪉⪊⪋⪌⪍⪎⪏⪐⪑⪒⪓⪔⪕⪖⪗⪘⪙⪚⪛⪜⪝⪞⪟⪠⪡⪢⪣⪤⪥⪦⪧⪨⪩⪪⪫⪬⪭⪮⪯⪰⪱⪲⪳⪴⪵⪶⪷⪸⪹⪺⪻⪼⪽⪾⪿⫀⫁⫂⫃⫄⫅⫆⫇⫈⫉⫊⫋⫌⫍⫎⫏⫐⫑⫒⫓⫔⫕⫖⫗⫘⫙⫚⫛⫝̸⫝⫞⫟⫠⫡⫢⫣⫤⫥⫦⫧⫨⫩⫪⫫⫬⫭⫮⫯⫰⫱⫲⫳⫴⫵⫶⫷⫸⫹⫺⫻⫼⫽⫾⫿"

//valute = L"₠₡₢₣₤₥₦₧₨₩₪₫€₭₮₯₰₱₲₳₴₵₶₷₸₹₺₻₼₽₾"
//numerical_forms = L"⅐⅑⅒⅓⅔⅕⅖⅗⅘⅙⅚⅛⅜⅝⅞⅟ⅠⅡⅢⅣⅤⅥⅦⅧⅨⅩⅪⅫⅬⅭⅮⅯⅰⅱⅲⅳⅴⅵⅶⅷⅸⅹⅺⅻⅼⅽⅾⅿↀↁↂↃↄↅↆↇↈ↉↊↋"



DFA::~DFA() {

}

const DFA& DFA::operator =(const DFA& other) {
	if(this != &other) {
		this->startingState = other.startingState;
		this->stateSymbPos = other.stateSymbPos;
		this->finalStates = other.finalStates;
	}
	return *this;
}

void DFA::print() {
	cout << "--------------------------------------------\n";
	cout << "states: " << nStates() << endl;
	cout << "finals: " << finalStates.size() << endl;
	cout << "start:  " << startingState << endl;
	for (int i = 0; i < nStates(); i++) {
		cout << "    " << i;
		if (finalStates.count(i))
			cout << "\tF\n";
		else
			cout << "\tN\n";
	}

	std::cout << "------stateSymbPos START ------------\n";
	for (int state=0; state < stateSymbPos.size(); state++){
        std::cout << "STATE:" << state << '\n';
	    for(int symbol = 0; symbol < ALPHABET_SIZE; symbol++) {
            map<int, int>::const_iterator it;
            it = stateSymbPos.find(state)->second.find(symbol);
            if (it != stateSymbPos.find(state)->second.end()){
                std::cout << it->second;
            }
            else{
                std::cout << NULL_STATE;
            }
        }
        cout << endl;
	}
	std::cout << "------stateSymbPos END ------------\n";

/*
	for(int i = 0; i < nStates(); i++) {
		for(int j = 0; j < ALPHABET_SIZE; j++) {
			cout << transitions[i][j];
		}
		cout << endl;
	}
*/
}

/*
set<int> DFA::getRegexSymbols(){
    return regexSymbols;
}

void DFA::setRegexSymbols(set<int>& regexSymbols){
    this->regexSymbols = regexSymbols;
}
*/

map<int, map<int,int>> DFA::getStateSymbPos(){
    return stateSymbPos;
}

void DFA::setStateSymbPos(map<int, map<int,int>>& stateSymbPos){
    this->stateSymbPos = stateSymbPos;
}


void DFA::minimizeStates() {
	//this->transitions.clear();
	//print();
}

int DFA::delta(int state, int symbol) const {
    map<int, map<int,int>>::const_iterator it_state = stateSymbPos.find(state);
    map<int, int>::const_iterator it_symbol;
    it_symbol = it_state->second.find(symbol);
    if (it_symbol != it_state->second.end())
        return it_symbol->second;
    else
        return NULL_STATE;
}

int DFA::getStartingState() const {
	return startingState;
}

bool DFA::isFinalState(int state) const {
	return finalStates.count(state);
}

int DFA::getFinalStateId(int state) const {
	if(finalStates.count(state)) {
		return finalStates.at(state);
	} else {
		return -1;
	}
}

int DFA::nStates() const {
	return stateSymbPos.size();
}

bool DFA::isNullState(int state) const {
	return (state == NULL_STATE);
}





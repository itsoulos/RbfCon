# ifndef __SYMBOL__H
# include <string>
# include <vector>
using namespace std;

class Rule;

/**
 * @brief The Symbol class provided the symbols (terminal and non terminal)
 * for the used BNF grammar.
 */
class Symbol
{
	private:
    /**
         * @brief name the name of the symbol
         */
		string name;
        /**
         * @brief rule the list of production rules for this symbol
         */
		vector<Rule*> rule;
        /**
         * @brief count_rules the count of production rules
         */
		int	count_rules;
        /**
         * @brief is_terminal determines if the symbol is terminal or not
         */
		int	is_terminal;
	public:
        /**
         * @brief Symbol the default constructor of the grammar
         */
		Symbol();

        /**
         * @brief set sets the name and the terminal status of the symbol
         * @param s
         * @param status
         */
		void	set(string s,int status);
        /**
         * @brief setName alters the name of this symbol
         * @param s
         */
		void	setName(string s);
        /**
         * @brief getName
         * @return  the name of this symbol
         */
		string	getName() const;
		
        /**
         * @brief setTerminalStatus alters the terminal status of the symbol
         * @param status
         */
		void	setTerminalStatus(int status);
        /**
         * @brief getTerminalStatus
         * @return  the terminal status of the symbol
         */
		int	getTerminalStatus() const;

        /**
         * @brief addRule adds a new production rule
         * @param r
         */
		void	addRule(Rule *r);
        /**
         * @brief cut reduces the number of terminal symbols to N
         * @param N
         */
		void	cut(int N);
        /**
         * @brief getRule
         * @param pos
         * @return  a vector to the production rule at position pos
         */
		Rule	*getRule(int pos) const;
        /**
         * @brief getCountRules
         * @return  the number of production rules
         */
		int	getCountRules() const;
		~Symbol();
		
};

# define __SYMBOL__H
# endif

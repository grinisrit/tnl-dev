// Generated by Bisonc++ V2.4.0 on Tue Aug 11 13:14:04 2009 +0000

#ifndef tnlDebugParser_h_included
#define tnlDebugParser_h_included

// $insert baseclass
#include "tnlDebugParserbase.h"


#undef tnlDebugParser

#include <istream>
#include "tnlDebugStructure.h"

using namespace std;

class tnlDebugFlexLexer;

class tnlDebugParser: public tnlDebugParserBase
{
        
    public:
        int parse();

    private:
        void error(char const *msg);    // called on (syntax) errors
        int lex();                      // returns the next token from the
                                        // lexical scanner. 
        void print();                   // use, e.g., d_token, d_loc

    // support functions for parse():
        void executeAction(int ruleNr);
        void errorRecovery();
        int lookup(bool recovery);
        void nextToken();

   public:

       tnlDebugParser();

       void setScanner( std::istream* in_stream );

       int runParsing( tnlDebugStructure* _debug_structure );

       // Methods called by the scanner
       void newLine();

       void setSVal( char* s );

       void setBVal( bool b );
   
       //Methods for the parsing 
       void AddCurrentGroup();
       
       void AddCurrentEntry();

       void SetCurrentClassId( char* s );

       void SetCurrentFunctionId( char* s );

       void SetBool( bool v );

       void SetDebugValue( );

       void SetDefaultDebugValue( );

       void SetClassDebugSettings();
       

       ~tnlDebugParser();
      
       static tnlDebugParser* current_parser;

       bool bool_value;

       bool debug_value;

       bool default_debug_value;

   protected:
       tnlDebugStructure* debug_structure;

       tnlDebugFlexLexer* scanner;

       int line;

       tnlDebugGroup* current_group;

       tnlDebugEntry* current_entry;
      
};

inline void tnlDebugParser::error(char const *msg)
{
    std::cerr << msg << std::endl;
}

// $insert lex

inline void tnlDebugParser::print()      // use d_token, d_loc
{}


#endif

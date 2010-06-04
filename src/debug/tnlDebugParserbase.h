// Generated by Bisonc++ V2.4.7 on Fri, 04 Jun 2010 14:28:08 +0200

#ifndef tnlDebugParserBase_h_included
#define tnlDebugParserBase_h_included

#include <vector>
#include <iostream>


namespace // anonymous
{
    struct PI__;
}


class tnlDebugParserBase
{
    public:
// $insert tokens

    // Symbolic tokens:
    enum Tokens__
    {
        __CLASS__ = 257,
        __DEBUG__,
        __DEFAULT__,
        __BOOL_TRUE__,
        __BOOL_FALSE__,
        __IDENTIFIER__,
    };

// $insert STYPE
union STYPE__
{
 char* s_val;
 char c_val;
 int i_val;
 double d_val;
 bool b_val;
};


    private:
        int d_stackIdx__;
        std::vector<size_t>   d_stateStack__;
        std::vector<STYPE__>  d_valueStack__;

    protected:
        enum Return__
        {
            PARSE_ACCEPT__ = 0,   // values used as parse()'s return values
            PARSE_ABORT__  = 1
        };
        enum ErrorRecovery__
        {
            DEFAULT_RECOVERY_MODE__,
            UNEXPECTED_TOKEN__,
        };
        bool        d_debug__;
        size_t      d_nErrors__;
        size_t      d_requiredTokens__;
        size_t      d_acceptedTokens__;
        int         d_token__;
        int         d_nextToken__;
        size_t      d_state__;
        STYPE__    *d_vsp__;
        STYPE__     d_val__;
        STYPE__     d_nextVal__;

        tnlDebugParserBase();

        void ABORT() const;
        void ACCEPT() const;
        void ERROR() const;
        void clearin();
        bool debug() const;
        void pop__(size_t count = 1);
        void push__(size_t nextState);
        void popToken__();
        void pushToken__(int token);
        void reduce__(PI__ const &productionInfo);
        void errorVerbose__();
        size_t top__() const;

    public:
        void setDebug(bool mode);
}; 

inline bool tnlDebugParserBase::debug() const
{
    return d_debug__;
}

inline void tnlDebugParserBase::setDebug(bool mode)
{
    d_debug__ = mode;
}

inline void tnlDebugParserBase::ABORT() const
{
    throw PARSE_ABORT__;
}

inline void tnlDebugParserBase::ACCEPT() const
{
    throw PARSE_ACCEPT__;
}

inline void tnlDebugParserBase::ERROR() const
{
    throw UNEXPECTED_TOKEN__;
}


// As a convenience, when including ParserBase.h its symbols are available as
// symbols in the class Parser, too.
#define tnlDebugParser tnlDebugParserBase


#endif



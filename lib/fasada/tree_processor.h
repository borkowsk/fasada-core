/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @footnote Technical mail: <fasada.wb@gmail.com>
/// @copyright 2019-2020
/// @version 0.02
///
///

#ifndef TREE_PROCESSOR_H
#define TREE_PROCESSOR_H (1)

#if defined(BOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "format_toolbox.h"
#include "fasada_types.h"
#include "fasada_tree.h"
#include <string>

namespace fasada
{
    void init(bool WithWriting);//INITIALIZE THE FASADA_TREE LIBRARY
                                //mostly registers default set of procesors
    void register_processors(bool WithWriters); //Function for registering default set of tree_processors
                                //located in fasada.a/lib/so
    bool writing_enabled();     //Is writing to tree (?) allowed?

    /// The boost::ptree have its own exception class ptree_error
    /// and derived class of exceptions.
    /// but sometimes tree_processor have to throw on error
    /// which is not connected with ptree
    /// In such cases it use this exception class
    // TODO - Consider this: https://www.boost.org/doc/libs/1_59_0/libs/exception/doc/frequently_asked_questions.html
    class tree_processor_exception:public pt::ptree_error
    {
        public:
            tree_processor_exception(std::string msg):pt::ptree_error(msg)
            {}
            ~tree_processor_exception() throw() {}
    };

    /// This class is basic class for all tree processors
    class tree_processor : public format_toolbox
    {
    public: //SUBTYPES
        enum Category {CONTROL=4,WRITER_READER=3,WRITER=2,READER=1};

    protected://Configuration
        std::string  procName;
        Category procCategory;

    protected://deferred implementation
    virtual
        void _implement_read(ShmString& o,const pt::ptree& top,URLparser& request)=0;
    virtual
        void _implement_write(ShmString& o,pt::ptree& top,URLparser& request)=0;
    virtual
        void _implement_attributes(ShmString& o,const pt::ptree& top,URLparser& request,std::string nameOfTop="");

    virtual//Replacing ${variable_name} with variables from request introducing procName as ${proc}
        std::string replace_all_variables(std::string template_version,URLparser& request);

    public://Construction, destruction etc.
        tree_processor(Category cat,const char* name);
    virtual
        ~tree_processor();
    static
        tree_processor& getReadProcessor (const char* name);
    static
        tree_processor& getWriteProcessor(const char* name);
    static
        tree_processor& getReadProcessor (const std::string& name)
        {return getReadProcessor(name.c_str());}
    static
        tree_processor& getWriteProcessor(const std::string& name)
        {return getWriteProcessor(name.c_str());}

    public://Interface
        //Does some work, calls _implement_read, cleans, adds MEM_END & returns
        void read_tree(ShmString& o,const pt::ptree& top,URLparser& request);//may throw TODO readFromTree?
        //Does some work, calls _implement_read, cleans, adds MEM_END & returns
        void write_tree(ShmString& o,pt::ptree& top,URLparser& request);//may throw TODO writeToTree?
};

    //Category operator + (Category one,Category two) ???
    //{ return Category(One+Two);}
    //
    //error: ‘fasada::tree_processor::Category
    //        fasada::tree_processor::operator+(fasada::tree_processor::Category,
    //                                          fasada::tree_processor::Category)’
    //
    // must take either zero or one argument ???
    //W sumie można i tak się bez niego obyć

}//namespace "fasada"

/**********************************************************************************************/
/* Fasada-core is an open source shareware                                                    */
/* Please contact the author about licensing                                                  */
/* 2019 - 2020 (c) Wojciech Borkowski wborkowsk@gmail.com                                     */
/**********************************************************************************************/
#endif // TREE_PROCESSOR_H

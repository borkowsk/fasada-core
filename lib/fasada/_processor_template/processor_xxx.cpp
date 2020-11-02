/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @footnote Technical mail: <fasada.wb@gmail.com>
/// @copyright 2019-2020
/// @version 0.02
///
/// Fasada-core is an open source shareware
/// Please contact the author about licensing
/// 2019 - 2020 (c) Wojciech Borkowski wborkowsk@gmail.com

#include "fasada.hpp"
#include "processor_xxx.h"
#include <boost/lexical_cast.hpp>

namespace fasada
{

processor_xxx::processor_xxx(const char* name):
    yyy_class(WRITER_READER/*READER? WRITER?*/,name)
{

}

processor_xxx::~processor_xxx()
{}

void processor_xxx::_implement_read(ShmString& o,const pt::ptree& top,URLparser& request)
{
    throw(tree_processor_exception("PTREE PROCESSOR "+procName+" IS NOT IMPLEMENTED AS A READER!"));
}


void processor_xxx::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)
{
    throw(tree_processor_exception("PTREE PROCESSOR "+procName+" IS NOT IMPLEMENTED AS A WRITER!"));
}

}//namespace "fasada"

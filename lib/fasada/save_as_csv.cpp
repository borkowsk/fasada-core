/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @footnote Technical mail: <fasada.wb@gmail.com>
/// @copyright 2019-2020
/// @version 0.02
///
///
#define UNIT_IDENTIFIER "save_as_csv"
#include "fasada.hpp"
#include "save_as_csv.h"
#include <boost/lexical_cast.hpp>

namespace fasada
{

save_as_csv::save_as_csv(const char* name):
    saver_processor(name)
{}

save_as_csv::~save_as_csv()
{}

void save_as_csv::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)    //TODO
{
    throw(tree_processor_exception("PTREE PROCESSOR "+procName+" IS NOT IMPLEMENTED YET!"));
}

}//namespace "fasada"


/**********************************************************************************************/
/* Fasada-core is an open source shareware                                                    */
/* Please contact the author about licensing                                                  */
/* 2019 - 2020 (c) Wojciech Borkowski wborkowsk@gmail.com                                     */
/**********************************************************************************************/


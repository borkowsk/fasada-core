#ifndef PROCESSOR_JSON_H
#define PROCESSOR_JSON_H
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @footnote Technical mail: <fasada.wb@gmail.com>
/// @copyright 2019-2020
/// @version 0.02
///
#if defined(BOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "fasada.hpp"
#include "loader_processor.h"

namespace fasada
{

class loader_json : public loader_processor
{
public:
    loader_json(const char* name="Json");
    ~loader_json(){}

protected://implementation
    //virtual
    //    void _implement_read(ShmString& o,const pt::ptree& top,URLparser& request);
    virtual
    void _implement_write(ShmString& o,pt::ptree& top,URLparser& request);
};

}

/**********************************************************************************************/
/* Fasada-core is an open source shareware                                                    */
/* Please contact the author about licensing                                                  */
/* 2019 - 2020 (c) Wojciech Borkowski wborkowsk@gmail.com                                     */
/**********************************************************************************************/
#endif // PROCESSOR_JSON_H

#include "fasada.hpp"
#include "memory_pool.h"
#include "tree_processor.h"

namespace fasada
{
const float _version=FASADA_VERSION;
const char* _version_str=FASADA_VERSION_STR;

static int _fasflags=0;

void init(bool WithWriters)
//Initialize the fasada::library
{
    register_processors(WithWriters);
    _fasflags |= tree_processor::READER;
    if(WithWriters)
         _fasflags |= tree_processor::WRITER;
}

bool writing_enabled()
{
    return (_fasflags & tree_processor::WRITER)!=0;
}

}

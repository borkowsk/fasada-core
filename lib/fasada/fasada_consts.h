/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @footnote Technical mail: <fasada.wb@gmail.com>
/// @copyright 2019-2020
/// @version 0.02
///

#ifndef TREE_CONSTS_H
#define TREE_CONSTS_H

#if defined(BOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

namespace fasada
{
///Defaults of IPC queue
const char* const MSGQ_NAME="FasadaDefMemoryPoolQ";
const unsigned    MSG_MAX_LEN=0xfff;
const unsigned    MSGQ_SIZE=1000;

///Defaults of IPC shared memory
const char* const MEM_POOL_NAME="FasadaDefMemoryPool";
const unsigned    MEM_POOL_SIZE=0xfffffff;

///Default Emp
const char* const PRIMARY_EMP="PrimaryServerEmp";

///Value of this const mark finish of any of tree-processors tasks.
const char* const POSTED_BLOCK_MARK="HTTP_POSTED_";//Block marker for POSTed http data in memeory pool
const char* const MEM_END="@T@h@e@E@n@d@";
const char* const EXT_PRE="@E@X@T=";//See for MIME types:
      //https://developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/MIME_types/Complete_list_of_MIME_types

const unsigned    EXPECTED_HTTP_REQ_BUFFER_SIZE=1024*1024*2;//WWW server must handle at least such size of POST request!

///Some values for html output
#define STR_DEFAULT_LEN_OF_NAME "14"
const unsigned UINT_DEFAULT_LEN_OF_NAME=14;

#define STR_DEFAULT_LEN_OF_SUBPATH "64"
const unsigned UINT_DEFAULT_LEN_OF_SUBPATH=64;

#define STR_WIDTH_MAX_OF_FIELD "80"
const unsigned UINT_WIDTH_MAX_OF_FIELD=80;

#define STR_DEFAULT_FILTER "*"

}//namespace "fasada"

/**********************************************************************************************/
/* Fasada-core is an open source shareware                                                    */
/* Please contact the author about licensing                                                  */
/* 2019 - 2020 (c) Wojciech Borkowski wborkowsk@gmail.com                                     */
/**********************************************************************************************/
#endif // TREE_CONSTS_H

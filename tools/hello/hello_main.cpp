/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @footnote Technical mail: <fasada.wb@gmail.com>
/// @copyright 2019
/// @version 0.01
///
///  See CURRENT licence file!
///

//Local demo server of "fasada"
//Just answer for READ REQUEST with message
//set up in previous WRITE REQUEST

#define UNIT_IDENTIFIER "hello"

#include "facebookspec.h"
#include "fasada.hpp"
#include "memory_pool.h"
#include "http/URLparser.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/algorithm/string/case_conv.hpp>

#include <iostream>
#include <time.h>       /* time_t, struct tm, time, localtime */

using namespace fasada;

string MyName("HELLO-");//Process name
const char* MyMemPoolName=fasada::PRIMARY_EMP;

std::string hello_msg="Hello from fasada demo application!";
std::string private_dir="../private/";
std::string public_dir="../public/";

// Control if it is more to do
unsigned NumberOfClients=0;

inline URLparser split_request(const string& request)//May throw exceptions
{                                                    //TODO - move reseult?
    URLparser URL(request.c_str());
    //some extra work
    URL["&public_directory"]=public_dir;
    if(URL["&domain"]=="localhost")//only localhost serwer has ability to see private directory of fasada
        URL["&private_directory"]=private_dir;
    return URL;
}

void _do_RorW_request(const string& request,fasada::MemoryPool& MyPool,bool isWriter)//May throw exceptions
{
    ShmCharAllocator charallocator(MyPool->get_segment_manager());
    ShmString *stringToShare = nullptr;
    try
    {
        stringToShare = MyPool->construct<ShmString>(request.c_str())(charallocator);
    }
    catch(...)
    {
        std::cerr <<MyName<<
            ": Unexpected exception, diagnostic information follows:\n" <<
            boost::current_exception_diagnostic_information();
        stringToShare=nullptr;
    }
    if(stringToShare==nullptr)
        throw( interprocess_exception("Server was not able to construct shared memory string!") );

    URLparser URL=split_request(request);//May throw exceptions
    if(URL.find("&path")!= URL.end())//Sciezka musi byc zawsze
    {
        //ShmStream outstr(*stringToShare);charallocator? //to jednak nie tak dziala jakbym chcial
        //outstr << URL["protocol"] << '\n'<< URL["domain"] << '\n'<< URL["path"] << '\n'<< URL["processor"] << '\n'<< URL["query"] << std::endl;
        for(auto p:URL)
        {
            (*stringToShare)+=p.first.c_str();
            (*stringToShare)+=" = ";
            (*stringToShare)+=p.second.c_str();
            (*stringToShare)+="\n";
        }
        ////////////////////////////////////////////////////////////////////
        try{
            if(!isWriter)
            {
                val_string& path=URL["&path"];
                (*stringToShare)+=ipc::string(EXT_PRE)+"htm\n";
                (*stringToShare)+="<HTML>\n<H1>"+hello_msg+"</H1>\n</HTML>"+MEM_END;
            }
            else
            {
                if(URL["&processor"][0]=='!')
                    URL["&processor"]=URL["&processor"].substr(1);
                hello_msg=URL["&processor"];//URL["&path"];
                //Pierszy slash ścieżki stanowi problem...
                //pt::ptree& branch =( path=="/" ? root : root.get_child(pt::ptree::path_type{path.c_str()+1, '/'}) );
                (*stringToShare)+="\n"+hello_msg+"\n"+MEM_END;
            }
        }
        /////////////////////////////////////////////////////////////////////
        catch(const std::runtime_error& exc)
        {
            *stringToShare+=exc.what();
            *stringToShare+=MEM_END;
        }
        catch(...)
        {
             *stringToShare+=("UNEXPECTED ERROR @" + boost::lexical_cast<std::string>( __LINE__ )
                              +MEM_END);
        }
    }
    else
    {
        if(stringToShare!=nullptr)
        {
            *stringToShare=(MyName+": invalid request, split failed").c_str();
            *stringToShare+=MEM_END;
        }
    }
}

//W rzeczywistości różnica w obsłudze readera i writera jest minimalna - dopiero w momencie
//próby wywołania odpowiedniego procesora, więc rozróznienie nie bedzie potrzebne
//Zachowujemy tymczasowo
void do_reader_request(const string& request,fasada::MemoryPool& MyPool)//May throw exceptions
{
    _do_RorW_request(request,MyPool,false);
}

void do_writer_request(const string& request,fasada::MemoryPool& MyPool)//May throw exceptions
{
    _do_RorW_request(request,MyPool,true);
}

void do_writer_request_debug(const string& request,fasada::MemoryPool& MyPool)//May throw exceptions
{   //SUBJECT TO CHANGE! TODO
    ShmCharAllocator charallocator(MyPool->get_segment_manager());
    ShmString *stringToShare = MyPool->construct<ShmString>(request.c_str())(charallocator);
    if(stringToShare==nullptr)
        throw( interprocess_exception("One cannot construct shared memory string!") );

    URLparser URL=split_request(request);//May throw exceptions
    if(URL.find("&path")!= URL.end())//Sciezka musi byc zawsze
    {
        if(URL["&processor"][0]=='!')
            URL["&processor"]=URL["&processor"].substr(1);
        *stringToShare=(URL["&protocol"]+'\n'
                        +URL["&domain"]+'\n'
                        +URL["&path"]+'\n'
                        +URL["&processor"]+'\n'
                        +URL["&query"]).c_str();
        *stringToShare+=MEM_END;
    }
    else //a wyjątki? TODO!?!?!
    {
        if(stringToShare!=nullptr)
        {
            *stringToShare=(MyName+": invalid request, split failed").c_str();
            *stringToShare+=MEM_END;
        }
    }
}

void do_local_processing(string& request, MemoryPool::ContentType msgType,fasada::MemoryPool& MyPool)//May throw exceptions
{
    int position;//Pomocnicza pozycja
    switch(msgType)
    {
    case MemoryPool::ContentType::Control://There are no responses for control messages
        if((position=request.find("HelloFrom",0))==0)
        {
            int position=request.find("-",7);
            if(position!=request.npos)
            {
                NumberOfClients++;
                std::cout<<MyName<<" has registered client PID"<<request.c_str()+position<<std::endl;
            }
        }
        else if(request.find("ByeFrom",0)==0)
        {
            int position=request.find("-",7);
            if(position!=request.npos)
            {
                NumberOfClients--;
                std::cout<<MyName<<" has unregistered client PID"<<request.c_str()+position<<std::endl;
            }
        }
        else
        {
            std::cerr<<MyName<<" has recivied invalid control request "<<request.c_str()+position<<std::endl;
        }
        break;
    case MemoryPool::ContentType::Write:
        do_writer_request(request,MyPool);
        break;
    case MemoryPool::ContentType::Read:
        do_reader_request(request,MyPool);
        break;
    default: //Exception?
        std::cerr<<MyName<<" recived message of unexpected type "<<msgType
                <<", with content '"<<request<<"'"<<std::endl;
        break;
    }
}

int main(int argc, char* argv[])
{
    MyName+=boost::lexical_cast<string>(getpid());
    std::cout<<"\n"<<MyName;
    std::cout<<": "<<argc<<" parameters: ";for(int a=0;a<argc;a++) std::cout<<argv[a]<<' ';//DEBUG
    std::cout<<std::endl;

    if( (argc>1 && (string("--force"))!=argv[1]) || (argc>2 &&(string("--force"))!=argv[2] ) )
    try{
        fasada::MemoryPool TestPool;//Próbuje się podłączyć jako klient
        //Jesli się uda to znaczy że server już działa
        std::cerr<<"Only one such FASADA server is alloved!\nKill the not responding server and start again with --force"<<std::endl;
        return 1;
    }
    catch(const interprocess_exception& exc)//Raczej spodziewamy się tego błędu
    {
        //To exception jest OK
        exc.get_error_code();
    }

    //Teraz dopiero uznaje że może być serwerem
    try{
        std::cerr<<"Making communication pool & request queue"<<std::endl;//To jest serwer odpowiedzialny za ten obszar pamięci
        fasada::MemoryPool MyMemPool(MemoryPool::IsServer::True);                  assert(MyMemPool.is_server());

        if(argc > 1 && argv[1][0]!='-')
        {
            hello_msg=argv[1];
        }

        ShmCharAllocator charallocator(MyMemPool.segm().get_segment_manager());
        //do{
        ShmString *stringToShare = MyMemPool->construct<ShmString>(MyMemPoolName)(charallocator);
        *stringToShare=
                (
                    string("FASADA HELLO version 0.01; PID:")
                        +boost::lexical_cast<string>(getpid())
                    ).c_str();

        //receive & process the request!
        do{
            std::cerr<<MyName<<" receiving..."<<std::endl;
            string data;
            MemoryPool::ContentType msgType;
            try{
                data=MyMemPool.receive(msgType);//Tu poczeka na pierwszego klienta przynajmniej jakiś czas
                std::cerr<<MyName<<" received '"<<data<<"'"<<std::endl;
                do_local_processing(data,msgType,MyMemPool);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));//https://stackoverflow.com/questions/4184468/sleep-for-milliseconds/10613664#10613664?newreg=6841aea0490b47baa3c6a7ea2bebaa30
            }
            catch(const interprocess_exception& exc)
            {
                std::cerr<<MyName<<": in loop communication exception:'"<<exc.what()<<"'"<<std::endl;
            }
       }while(NumberOfClients>0);

        MyMemPool.free_data(MyMemPoolName);

        std::cerr<<MyName<<": I'm finished."<<std::endl;
        return 0;
    }
    catch(const interprocess_exception& exc)
    {
        std::cerr<<MyName<<" recive an unexpected communication exception:'"<<exc.what()<<"'"<<std::endl;
        return -2;
    }

    return -9999;//Nie powinien tu trafić
}


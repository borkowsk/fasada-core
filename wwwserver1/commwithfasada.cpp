/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @footnote Technical mail: <fasada.wb@gmail.com>
/// @copyright 2019/2020
/// @version 0.02.2020.11
///
///  See CURRENT licence file!
///
/// This file:
/// @file  commwithfasada.cpp
/// @brief Implementation of connection between fasada server & wwwserver
///
/// FASADA is a simple interface for communicating any user side application
/// thru WWW . Mostly based on boost examples & open source solutions from StackOverflow
///
/// This version work correctly with txt & html input from fasada and most type of common
/// file types used in WWW communication.
///
/// Fasada-core is an open source shareware
/// Please contact the author about licensing
/// 2019 - 2020 (c) Wojciech Borkowski wborkowsk@gmail.com

#define UNIT_IDENTIFIER "wwwserver-for-fasada_v02_20201101"

#include "request.hpp"
#include "reply.hpp"
#include "fasada_types.h"
#include "fasada_consts.h"
#include "memory_pool.h"
#include "mime_types.hpp"
#include <boost/lexical_cast.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <iostream>
#include <string>
#include <cctype>

namespace http { //TO DO KASACJI?
namespace server { //DO ZAMIANY NA "fasada"?

using namespace fasada;

const unsigned MILLISECONDS_BW=200;//ms for waiting for continuation of precessing outside wwwserver
const char     FINAL_CMD[128]="/!!!!";//SPECIAL!!!

extern "C" //Te dwie funcje do eksportowania jako gole nazwy
{
    void set_host_and_port_for_fasada(const char* iHost,const char* iPort);
    bool communicate_with_fasada(const request& curr_request, reply& curr_reply);
}

class FasadaKeepAndExit
{
    string  MyName;
    string  MemPoolName;
    fasada::MemoryPool& MyMemPool;//Udostępnialna kopia zabezpieczanej zmiennej
public:
    FasadaKeepAndExit(fasada::MemoryPool& MyPool):
        MyMemPool(MyPool),MyName("WWWSERVER1-"),MemPoolName(PRIMARY_EMP)
    {
        MyName+=boost::lexical_cast<string>(getpid());
        //The first data may be used for checking if it is a valid treeserver
        std::cerr<<"Connecting to fasada server..."<<std::endl;
        std::pair<ShmString*, managed_shared_memory::size_type> res;

        res = MyMemPool->find<ShmString>(MemPoolName.c_str());

        if(res.first==nullptr)
            throw interprocess_exception("A proper fasada server not found!");

        if(res.second!=1)
            throw interprocess_exception("Incompatible fasada server found!");//Nie spodziewa się tablicy!

        std::cerr<<"Fasada server connected"<<std::endl;

        //Send Hello to server
        string Msg("HelloFrom:");
        Msg+=MyName.c_str();
        std::cerr<<"Sending '"<<Msg<<"' to fasada"<<std::endl;
        MyMemPool.send_request(Msg,MemoryPool::ContentType::Control);//Control message
    }
    ~FasadaKeepAndExit()
    {
        //Send BYE to server
        std::cerr<<MyName<<": bye, bye!"<<std::endl;
        string Msg("ByeFrom:");
        Msg+=MyName;
        MyMemPool.send_request(Msg,MemoryPool::ContentType::Control);//Control message
    }
    const string& Name()
    {
        return MyName;
    }
    fasada::MemoryPool& Pool()
    {
        return MyMemPool;
    }
};

static ipc::string host="localhost";
static ipc::string port="8000";
static std::shared_ptr<FasadaKeepAndExit> FasadaConnection;

static fasada::MemoryPool& do_when_first_time() //DZIWNA KONSTRUKCJA, ALE TYMCZASOWA BO BĘDZIE WIĘCEJ KOLEJEK i PROGRAMÓW Z Fasadą
{
    std::cout<<string(UNIT_IDENTIFIER)<<" is initializing connection with SERVICE"<<std::endl;
    static fasada::MemoryPool MyHiddenPool; //To jest klient! Konstruktor bez parametru (NA RAZIE)
    FasadaConnection=std::shared_ptr<FasadaKeepAndExit>(new FasadaKeepAndExit(MyHiddenPool));
    return MyHiddenPool;
}

static bool findExt(const ShmString* resp,string& extension,string::size_type& extpos)
{
    extpos=resp->find(fasada::EXT_PRE,0);
    //std::cout<<'{'<<extpos<<'}'<<std::endl;

    if(extpos!=string::npos)
    {
        //std::cout<<'['<<extpos<<']';
        extension.clear(); extension.reserve(12);
        extpos+=strlen(fasada::EXT_PRE);

        const char* ext=resp->c_str()+extpos;
        while(!isblank(*ext) && !iscntrl(*ext))
        {
            //std::cout<<(*ext);
            extension+=(*ext);
            ext++;
        }

        extpos+=extension.size();
        //std::cout<<std::endl;
        return true;
    }
    else
        return false;
}

static void read_answer(reply& repl,ShmString* resp,const string& uri)
{
    string  extension="txt";//Plain/text is default MIME type
    string::size_type endpos=string::npos;
    string::size_type extpos=0;
    string::size_type begpos=0;

    unsigned replays=0;
    do
    {
      //endpos=resp->find(string_view(fasada::MEM_END),begpos);//C++17
      endpos=resp->find(fasada::MEM_END,begpos,strlen(fasada::MEM_END));

      if(endpos!=string::npos)
      {
          //std::cout<<"("<<endpos<<")"<<std::endl;
          //std::cout<<"("<<resp->c_str()+endpos<<")"<<std::endl;
          resp->erase(endpos);
          replays=0;
      }
      else
      {
          replays++;
      }

      const char* lastpos=resp->c_str()+begpos;
      //std::cout<<lastpos;//DEBUG

      if(replays!=0)//Jesli trzeba poczekac
      {
        begpos+=strlen(lastpos);//Przesuwamy o to co juz "wydrukowane"
        //https://stackoverflow.com/questions/4184468/sleep-for-milliseconds/10613664#10613664?newreg=6841aea0490b47baa3c6a7ea2bebaa30
        std::this_thread::sleep_for(std::chrono::milliseconds(MILLISECONDS_BW));
      }

    }while(endpos==string::npos && replays<20);

    if(replays!=0)//Nie doczekal sie
    {
       std::cerr<<"\n"<<FasadaConnection->Name()<<" waiting "<<replays*MILLISECONDS_BW<<"ms for continuation"<<std::endl;
       std::cerr<<"Shared memory for '"<<uri<<"' potentially remain available for reread"<<std::endl;
       (*resp)+="\n\nAnswer incomplete! Reload the same request after some time!\n";
    }

    if( findExt(resp,extension,extpos) )
    {
        extpos++;//Tym razem znak separatora też
        std::cout<<"("<<extpos<<") '"<<extension<<"'"<<std::endl;
    }
    else
        extpos=0;

    // Fill out the reply to be sent to the client.
    repl.status = reply::ok;
    repl.content.assign(resp->c_str()+extpos,resp->size()-extpos);//Czy takie assign jest wrazliwe na koniec c-stringa?
                                                    //Tak bezpieczniej? Wszystkie dane, nawet jak są w srodku znaki \0?

    repl.headers.resize(2);
    repl.headers[0].name = "Content-Length";
    repl.headers[0].value = std::to_string(repl.content.size());
    repl.headers[1].name = "Content-Type";
    repl.headers[1].value = mime_types::extension_to_type(extension.c_str()).c_str();

    std::cout<<"\nRequest: "<<uri<<" done."<<std::endl;
    std::cout<<repl.headers[0].name<<" : "<<repl.headers[0].value<<std::endl;
    std::cout<<repl.headers[1].name<<" : "<<repl.headers[1].value<<std::endl;

    //Mozna ewentualnie posprzatac
    if(replays==0) //Bylo dokonczone, mozna usunac blok komunikacyjny
        FasadaConnection->Pool().free_data(uri.c_str());//Tylko wtedy mozna usunac obszar gdy produkcja została zakończona
}

void do_exit()
{
    sleep(2);
    FasadaConnection=nullptr;//deallocation means close connection with treeserver!
    exit(-9999);//DEBUG exit of wwwserver
}


//Only these 2 functions are globally visible:
bool communicate_with_fasada(const request& curr_request, reply& curr_reply) // extern "C"
{
    try
    {
        static fasada::MemoryPool& MyMemPool=do_when_first_time();

        if(curr_request.uri==FINAL_CMD)//SPECIAL!!!
        {
            std::thread spawn_wait_and_exit(do_exit);
            spawn_wait_and_exit.detach();
            curr_reply.status = reply::service_unavailable;
            curr_reply.content="Server is going to close in 2 sec";
            return true;
        }


        //Właściwa obsługa zapytania
        string req_uri="http://"+host+":"+port+curr_request.uri.c_str();

        //Jeśli POST to trzeba umieścić dane MemoryPool
        if(curr_request.method=="POST" || curr_request.method=="post")
        {
           ShmCharAllocator charallocator(MyMemPool->get_segment_manager());
           std::cout<<"POST method detected!"<<std::endl;
           if(curr_request.posted_content.size()==0)
               throw interprocess_exception("POST data not detected!");

           req_uri+="?method=POST";

           //TRANSFERING THE POSTED DATA
           ShmString *postedDataString = MyMemPool->construct<ShmString>( (POSTED_BLOCK_MARK+req_uri).c_str() )(charallocator);
           *postedDataString+=curr_request.posted_content.c_str();
           //for(auto a:*postedDataString)std::cout<<a;std::cout<<std::endl;//DEBUG
        }

        if( req_uri.find("!",0)!=req_uri.npos )//marker że "zapis" to ! przed nazwą processora
        {
            std::cerr<<FasadaConnection->Name()<<" sending WRITE request <<<"<<req_uri<<">>>"<<std::endl;
            MyMemPool.send_request(req_uri,MemoryPool::ContentType::Write);
        }
        else
        {
            std::cerr<<FasadaConnection->Name()<<" sending READ request <<<"<<req_uri<<">>>"<<std::endl;
            MyMemPool.send_request(req_uri,MemoryPool::ContentType::Read);
        }

        //if(verbose)
            std::cerr<<FasadaConnection->Name()<<" waiting for response from 'fasada'..."<<std::endl;

        ShmString* response=MyMemPool.wait_for_data(req_uri);//Odebranie odpowiedzi z pamięci dzielonej

        if(response==nullptr)
        {
            std::cout<<FasadaConnection->Name()<<": Response unavailable"<<std::endl;
            return false;//TOTALLY FAILED
        }
        else
        {
            read_answer(curr_reply,response,req_uri);
            //Usuniecie danych POST (a jeśli przedtem były usunięte?)
            //Albo nie zdążył odczytać?
            if(curr_request.method=="POST" || curr_request.method=="post")
            {
                string bname=POSTED_BLOCK_MARK;
                bname+=req_uri;
                MyMemPool.free_data( bname.c_str() );//SAFE. Jak już skasowane to po prostu zwraca false
            }
            return true; //COŚ ZOSTAŁO ODEBRANE - Z PUNKTU WIDZENIA SERWERA OK
                         //bo ma co wysyłać, więc może kontynuować LANG?
        }
    }
    catch(const interprocess_exception& exc)
    {
        std::cerr<<FasadaConnection->Name()<<" communication failed because '"<<exc.what()<<"'"<<std::endl;
        return false;//TOTALLY FAILED
    }
    catch(...)
    {
        std::cerr <<FasadaConnection->Name()<<
            ": Unexpected exception, diagnostic information follows:\n" <<
            boost::current_exception_diagnostic_information();
        return false;//TOTALLY FAILED
    }
    return false;//SHOULD NEVER BE USED!
}

void set_host_and_port_for_fasada(const char* iHost,const char* iPort)// extern "C"
{
    host=iHost;
    port=iPort;
    std::cout<<"'"<<string(UNIT_IDENTIFIER)<<"' Host:"<<host<<" Port:"<<port<<std::endl;
}

} // namespace server
} // namespace http


/* CHROME INCOMPATIBILITY:
==========================

REQUEST-URI: /mouse-1734d86fbb844549d!
METHOD: POST
HTTP: 1.1
Processed: 518
HEADERS:
        'Host' : localhost:8889
        'Connection' : keep-alive
        'Content-Length' : 1549
        'User-Agent' : Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/83.0.4103.116 Safari/537.36
        'content-type' : text/plain
        'Accept' : * / *
        'Origin' : http://localhost:8889
        'Sec-Fetch-Site' : same-origin
        'Sec-Fetch-Mode' : cors
        'Sec-Fetch-Dest' : empty
        'Referer' : http://localhost:8889/MouseRegJs/index.html
        'Accept-Encoding' : gzip, deflate, br
        'Accept-Language' : pl-PL,pl;q=0.9,en-US;q=0.8,en;q=0.7
Request: /mouse-1734d86fbb844549d! is for FACJATA
POST method detected!
WWWSERVER1-18494 communication failed because 'POST data not detected!'
Request: /mouse-1734d86fbb844549d! not handled

REQUEST-URI: /POSTtest!
METHOD: POST
HTTP: 1.1
Processed: 721
HEADERS:
        'Host' : localhost:8889
        'Connection' : keep-alive
        'Content-Length' : 29
        'Cache-Control' : max-age=0
        'Upgrade-Insecure-Requests' : 1
        'Origin' : http://localhost:8889
        'Content-Type' : application/x-www-form-urlencoded
        'User-Agent' : Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/83.0.4103.116 Safari/537.36
        'Accept' : text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,* / *;q=0.8,application/signed-exchange;v=b3;q=0.9
        'Sec-Fetch-Site' : same-origin
        'Sec-Fetch-Mode' : navigate
        'Sec-Fetch-User' : ?1
        'Sec-Fetch-Dest' : document
        'Referer' : http://localhost:8889/post_test.html
        'Accept-Encoding' : gzip, deflate, br
        'Accept-Language' : pl-PL,pl;q=0.9,en-US;q=0.8,en;q=0.7
Request: /POSTtest! is for FACJATA
POST method detected!
WWWSERVER1-18494 sending WRITE request <<<http://localhost:8889/POSTtest!?method=POST>>>
WWWSERVER1-18494 waiting for response from 'fasada'...
(193) 'htm'

Request: http://localhost:8889/POSTtest!?method=POST done.
Content-Length : 62
Content-Type : text/html

      */





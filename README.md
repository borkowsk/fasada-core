# fasada
File: _Fasada-core README.txt_


Project FASADA version 0.011 
====================================================
FASADA to napisany w C++ zestaw bibliotek i aplikacji (mini-framework) umożliwiający komunikację na wzór WWW 
pomiędzy przystosowanymi programami, a za pośrednictwem własnego mini-serwera WWW także z dowolnymi programami 
rozumiejącymi protokół HTTP, przynajmniej w sieci lokalnej.
Komunikacja pomiędzy procesami w obrębie hosta odbywa się za pomoca pamięci współdzielonej więc jest bardzo szybka.

LINKI ZEWNĘTRZNE PROJEKTU
=========================

Twitter Fasada: 
https://twitter.com/fasada02964935

Nieco podobny, ale dużo bardziej zaawansowany projekt hierarchiczno-obiektowej bazy danych to: 
https://www.mongodb.com/

Wymagania
=========

Projekt wymaga do kompilacji i działania biblioteki BOOST.
Testowany na UBUNTU 16.04 i 18.04

Instalacja boost na UBUNTU: 

*__sudo apt-get install libboost-all-dev__*

Przydatna jest też biblioteka LIBMAGIC do dedukowania typów plików.
(można wyłączyć korzystanie z niej) 
Warto ją jednak też zainstalować:

*__sudo apt-get install libmagic-dev__*

Inne technikalia
================

Ta wersja oddzielona od starszej wersji wg. poniższej receptury: 
https://help.github.com/en/github/using-git/splitting-a-subfolder-out-into-a-new-repository

CO ZROBIĆ GDY ZMIENI SIĘ NAZWA REPOZYTORIUM? 
https://www.linuxquestions.org/questions/programming-9/git-my-remote-repository-moved-how-do-i-tell-my-local-repo-to-use-the-new-one-781785/


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

Alternatywny serwer WWW do przetestowania
=========================================

https://www.boost.org/doc/libs/1_73_0/libs/beast/example/advanced/server-flex/advanced_server_flex.cpp

I inne przyklady:

https://www.boost.org/doc/libs/1_73_0/libs/beast/doc/html/beast/examples.html

https://www.boost.org/doc/libs/1_73_0/libs/beast/example/http/server/flex/http_server_flex.cpp


Inne technikalia
================

Ta wersja oddzielona od starszej wersji wg. poniższej receptury: 
https://help.github.com/en/github/using-git/splitting-a-subfolder-out-into-a-new-repository

Zwykle używane jako submodule innych projektów. Zatem poniższe linki mogą się przydać:

https://git-scm.com/docs/git-submodule#Documentation/git-submodule.txt-update--init--remote-N--no-fetch--no-recommend-shallow-f--force--checkout--rebase--merge--referenceltrepositorygt--depthltdepthgt--recursive--jobsltngt--ltpathgt82308203

https://stackoverflow.com/questions/1777854/how-can-i-specify-a-branch-tag-when-adding-a-git-submodule



CO ZROBIĆ GDY ZMIENI SIĘ NAZWA REPOZYTORIUM? 
https://www.linuxquestions.org/questions/programming-9/git-my-remote-repository-moved-how-do-i-tell-my-local-repo-to-use-the-new-one-781785/


**Licencing**

# Fasada-core is an open source shareware
# Please contact the author about licensing for scientific & commercial purposes
# 2019 - 2020 (c) Wojciech Borkowski wborkowsk@gmail.com

#You can use this software freely for educational and private purposes, but if you feel that it would be appropriate to repay somehow, please finance me a big coffee :-)

https://www.paypal.com/paypalme/wborkowsk








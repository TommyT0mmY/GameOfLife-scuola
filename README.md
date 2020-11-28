cmd

help // scrive i comndi



t xx yy //cambia la casella alle coordinte x,y

sq xx yy ww hh b //imposta l'area con spigolo x,y di larghezza w e altezza h con il valore b
tsq xx yy ww hh //inverte il valore nell'area con pigolo x,y di larghezza w e altezza h

line xx1 yy1 xx2 yy2 b //imposta con il valore b una line (rasterizer) da xx1,yy1 a xx2,yy2
tline xx1 yy1 xx2 yy2 //inverte il valore in una linea (rasteizer) da xx1,yy1 a xx2,yy2

poli4 xx1 yy1 xx2 yy2 xx3 yy3 xx4 yy4 b //imposta il valore b nel quadrilatero creato dalle coordinate (1...4)
tpoli4 xx1 yy1 xx2 yy2 xx3 yy3 xx4 yy4 //inverte il valore nel quadrilatero creato dalle coordinate(1...4)

load "filename" // carica il file con il nome "filename" eseguendo

snipet xx yy "filename" // carica e sovrappone il file mantenendo gli spazzi vuoti del file


settings:
    frametime (int) // implementato con cronos per mantenere la funzionalità multipiattaforma (in millisecondi)
    fullChar (char) // imposta il carattere delle celle piene default '#'
    empityChar (char) // imposta il carattere delle celle vuote default ' '
    help // scrive tutti i comandi settings

ex:
    settings frameTime 10 //imposta il frametime a 10 millisecondi
    settings fullChar 1 //imposte il carattere '1' per le celle piene
    settings empityChar 0 //imposte il carattere '0' per le celle vuote



\ 000000000011111111112222222222
 \012345678901234567890123456789
00                               
01                               
02                               
04           # #                 
05            ##                 
06            #                  
07                               
08                               
09                               
10                               

﻿# GameOfLife-scuola

cmd

t xx yy //cambia la casella alle coordinte x,y

sq xx yy ww hh b //imposta l'area con spigolo x,y di larghezza w e altezza h con il valore b
tsq xx yy ww hh //inverte il valore nell'area con pigolo x,y di larghezza w e altezza h

line xx1 yy1 xx2 yy2 b //imposta con il valore b una line (rasterizer) da xx1,yy1 a xx2,yy2
tline xx1 yy1 xx2 yy2 //inverte il valore in una linea (rasteizer) da xx1,yy1 a xx2,yy2

poli4 xx1 yy1 xx2 yy2 xx3 yy3 xx4 yy4 b //imposta il valore b nel quadrilatero creato dalle coordinate (1...4)
tpoli4 xx1 yy1 xx2 yy2 xx3 yy3 xx4 yy4 //inverte il valore nel quadrilatero creato dalle coordinate(1...4)

load "filename" // carica il file con il nome "filename" eseguendo